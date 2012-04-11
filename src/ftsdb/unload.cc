
#include "unload.hh"
#include <QStringList>
#include <QDebug>
#include <QDate>
#include <db_cxx.h>

using namespace std;
using namespace TC;


namespace TC {


int ShowAllDBNames(FtsDb& ftsdb, QStringList& slist)
{
  Db db(ftsdb.getDbEnv(), 0);
  Dbc *cursorp;
  
  try {
    db.open(0, ftsdb.getDbFile().data(), 0, DB_UNKNOWN, DB_RDONLY, 0);
    Dbt key, data;
    int ret;
    db.cursor(NULL, &cursorp, 0);
    slist.clear();
    while ((ret = cursorp->get(&key, &data, DB_NEXT)) == 0 ) {
      QByteArray k((const char*)key.get_data(), key.get_size());
      slist.push_back(k); // A database name
    }
    qDebug() << "ret=" << ret;
    qDebug() << slist.size() ;
    
    db.close(0);
  }
  catch(DbException& e) {
    db.err(e.get_errno(), "Error in show_all_records");
    cursorp->close();
    throw e;
  }

  return slist.size();
}


int ShowAllRecordsBySecurity(const QByteArray& sec, FtsDb& ftsdb)
{
  Db db(ftsdb.getDbEnv(), 0);
  Dbc *cursorp;
  try {
    db.open(0, ftsdb.getDbFile().data(), sec.data(), DB_BTREE,
            DB_RDONLY, 0);
    db.cursor(NULL, &cursorp, 0);
    // Iterate over the inventory database, from the first record
    // to the last, displaying each in turn
    Dbt key, data;
    int ret;
    while ((ret = cursorp->get(&key, &data, DB_NEXT)) == 0 )
    {
      QByteArray k((const char*)key.get_data(), key.get_size());
      QByteArray d((const char*)data.get_data(), data.get_size());

      int jday = *((int*)k.data());
      QDate date = QDate::fromJulianDay(jday);
      qDebug() << jday << date.toString() << d.size();
    }
    qDebug() << "Dbc::get()=" << ret;
  } catch(DbException &e) {
    db.err(e.get_errno(), "Error in ShowAllRecordsBySecurity");
    cursorp->close();
    throw e;
  } catch(std::exception &e) {
    cursorp->close();
    throw e;
  }

  cursorp->close();
  return 0;
}



int ShowAllRecordsByDate(const QDate& date, FtsDb& ftsdb)
{
  QStringList slist;
  int rv = ShowAllDBNames(ftsdb, slist);
  if(rv==0) return 0;
  if(!date.isValid()) {
    qWarning() << "Not a valid date";
    return 0;
  }
  
  int jday = date.toJulianDay();
  Dbt key(&jday, sizeof(jday));
  const int BfrMax = 512; //sizeof(esec_quad)+16;
  char bfr[BfrMax];
  Dbt data(bfr, BfrMax);
  PriceTuple_t pt;
  bool ret;
  
  try {

    foreach(QString s, slist) {
      if(s.isEmpty()) continue;
      Db db(ftsdb.getDbEnv(), 0);
      db.open(0, ftsdb.getDbFile().data(), s.toAscii().data(),
              DB_BTREE, DB_RDONLY|DB_THREAD, 0644);
      data.set_data(bfr);
      data.set_ulen(BfrMax);
      data.set_flags(DB_DBT_USERMEM);
      //data.set_flags(DB_DBT_MALLOC);
      rv = db.get(0, &key, &data, 0);
      if(rv==0) {
        qDebug() << s << date << data.get_size();
        pt.clear();
        ret = ConvertToSec(data.get_data(), data.get_size(), pt);
        if(!ret)
          qWarning() << "ConvertToSec() failed." ;
        else
          qDebug() << pt.at(PTOpen) << pt.at(PTClose) << pt.at(PTVolume);
      }
      db.close(0);
    } // foreach
  } // try
  catch(DbException& e) {
    qWarning() << e.what();
    //db.err(e.get_errno(), "Error in ShowAllRecordsByDate");
  }

  return slist.size();
}


sec_enum PickSecContainer(int size)
{
  switch(size) {
    case sizeof(sec_single):   return SecSingle;
    case sizeof(sec_single2):  return SecSingle2;
    case sizeof(sec_double):   return SecDouble;
    case sizeof(sec_double2):  return SecDouble2;
    case sizeof(sec_quad):     return SecQuad;
      
    case sizeof(esec_single):  return ESecSingle;
    case sizeof(esec_single2): return ESecSingle2;
    case sizeof(esec_double):  return ESecDouble;
    case sizeof(esec_double2): return ESecDouble2;
    case sizeof(esec_quad):    return ESecQuad;
  } // end switch

  return SecEnumMax;
}


template <typename S>
void SecToTuple(const S& s, PriceTuple_t& pt)
{
  pt.resize(PTVolume+1);
  pt[PTOpen] = s.opn.toDouble();
  pt[PTHigh] = s.hi.toDouble();
  pt[PTLow] = s.lo.toDouble();
  pt[PTClose] = s.cls.toDouble();
  pt[PTVolume] = s.vol.toDouble();  
}


template <typename ES>
void ESecToTuple(const ES& es, PriceTuple_t& pt)
{
  SecToTuple(es, pt);
  pt.resize(PTAsk+1);
  pt[PTOpenInterest] = es.opi.toDouble();
  pt[PTBid] = es.bid.toDouble();  
  pt[PTAsk] = es.ask.toDouble();  
}


bool ConvertToSec(void* data, int size, PriceTuple_t& pt)
{
  sec_enum sec_type = PickSecContainer(size);

  if(sec_type == SecEnumMax)
    return false;

  /* Not necessary:
  sec_single s;
  sec_single2 s2;
  sec_double d;
  sec_double2 d2;
  sec_quad q;
  esec_single es;
  esec_single2 es2;
  esec_double  ed;
  esec_double2 ed2;
  esec_quad eq;
  */
  
  switch(sec_type) {
    case SecSingle:
      SecToTuple(*(sec_single*)data, pt);
      return true;
    case SecSingle2:
      SecToTuple(*(sec_single2*)data, pt);
      return true;
    case SecDouble:
      SecToTuple(*(sec_double*)data, pt);
      return true;
    case SecDouble2:
      SecToTuple(*(sec_double2*)data, pt);
      return true;
    case SecQuad:
      SecToTuple(*(sec_quad*)data, pt);
      return true;
    case ESecSingle:
      SecToTuple(*(esec_single*)data, pt);
      return true;
    case ESecSingle2:
      SecToTuple(*(esec_single2*)data, pt);
      return true;
    case ESecDouble:
      SecToTuple(*(esec_double*)data, pt);
      return true;
    case ESecDouble2:
      SecToTuple(*(esec_double2*)data, pt);
      return true;
    case ESecQuad:
      SecToTuple(*(esec_quad*)data, pt);
      return true;
    default: break;
      // No-op
  }

  return false;
}


} // end namespace

