
// C++
#include <iostream>
#include <typeinfo>

// Qt
#include <QFile>
#include <QString>
#include <QtDebug>
#include <QDate>
#include <QDateTime>

// Project
#include <db_cxx.h>
#include "load.hh"


using namespace std;
using namespace TC;


namespace TC {

bool readFile(const QString& fn, Lines_t& lines)
{
  QFile file(fn);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    cerr << "Could not open file " << fn.toAscii().data();
    return false;
  }

  while (!file.atEnd()) {
    QByteArray line = file.readLine();
    lines.push_back(line.trimmed());
  }

  return true;
}


bool splitLine(const QByteArray& line, Tokens_t& tokens)
{
  QList<QByteArray> tlist;

  tlist = line.split(',');
  if(tlist.size() < 7)
    return false;

  for(int i=0; i<tlist.size(); ++i)
    tlist[i] = tlist[i].trimmed();
  
  tokens = tlist;
  return true;
}


int NumberOfDigits(const QByteArray& token)
{
  const char* p = token.data();
  const int size = token.size();
  int digits = 0;

  for(int i=0; i<size && *p; ++i)
    if(p[i] >= '0' && p[i] <= '9')
      digits++;

  return digits;
}


sec_enum BestDecimalContainer(const QByteArray& token)
{
  if(token.size() <= QDecSingleDigits)
    return SecSingle;
  
  int digits = NumberOfDigits(token);

  if(digits <= QDecSingleDigits)
    return SecSingle;
  if(digits <= QDecDoubleDigits)
    return SecDouble;
  if(digits <= QDecQuadDigits)
    return SecQuad;

  return SecQuad;
}




sec_enum PickSecContainer(const Tokens_t& tokens)
{
  QByteArray close = tokens.at(ClosePos);
  QByteArray volume = tokens.at(VolumePos);

  sec_enum sec_cls = BestDecimalContainer(close);
  sec_enum sec_vol = BestDecimalContainer(volume);

  // Heuristic to determine smallest type to store
  // the price/vol tuple
  
  if(sec_cls==SecSingle && sec_vol<=SecSingle)
    return SecSingle;

  if(sec_cls==SecSingle && sec_vol<=SecDouble)
    return SecSingle2;

  if(sec_cls==SecDouble && sec_vol<=SecDouble)
    return SecDouble;

  if(sec_cls==SecDouble && sec_vol<=SecQuad)
    return SecDouble2;

  return SecQuad;
}


template <typename S>
void StoreOHLCV(const Tokens_t& tokens, S& sd)
{
  //qDebug() << typeid(sd).name();
  
  QByteArray open = tokens.at(OpenPos);
  QByteArray high = tokens.at(HighPos);
  QByteArray low = tokens.at(LowPos);
  QByteArray close = tokens.at(ClosePos);
  QByteArray volume = tokens.at(VolumePos);

  sd.opn.fromString(open.data());
  sd.hi.fromString(high.data());
  sd.lo.fromString(low.data());
  sd.cls.fromString(close.data());
  sd.vol.fromString(volume.data());

  //TODO: Enable if more debug is needed.
  //qDebug() << volume << sd.vol.toString();
}


sec_enum StorePrice(const Tokens_t tokens, QByteArray& ba)
{
  sec_single s;
  sec_single2 s2;
  sec_double d;
  sec_double2 d2;
  sec_quad q;
  
  sec_enum id = PickSecContainer(tokens);

  switch(id) {
    case SecSingle:
      StoreOHLCV(tokens, s);
      ba = QByteArray((const char*)&s, sizeof(s));
      break;
    case SecSingle2:
      StoreOHLCV(tokens, s2);
      ba = QByteArray((const char*)&s2, sizeof(s2));
      break;
    case SecDouble:
      StoreOHLCV(tokens, d);
      ba = QByteArray((const char*)&d, sizeof(d));
      break;
    case SecDouble2:
      StoreOHLCV(tokens, d2);
      ba = QByteArray((const char*)&d2, sizeof(d2));
      break;
    case SecQuad:
      StoreOHLCV(tokens, q);
      ba = QByteArray((const char*)&q, sizeof(q));
      break;
    default:
      return Unknown;
  }
  
  return id;
}


// Convert YYYMMDD format to QDate
bool ConvertToQDate(const QByteArray& sdate, QDate& qdate)
{
  bool ok;
  int year = sdate.mid(0,4).toInt(&ok);
  int month = sdate.mid(4,2).toInt(&ok);
  int day = sdate.mid(6,2).toInt(&ok);

  return qdate.setDate(year, month, day);
}

sec_enum returnSec(Tokens_t& tokens, QByteArray& sv)
{
  QByteArray code = tokens.at(CodePos);
  QByteArray date = tokens.at(DatePos);

  // Due to a race condition following is deprecated:
  //QDate qdate = QDate::fromString(date, "yyyyMMdd");
  QDate qdate;
  ConvertToQDate(date, qdate);
  
  if(!qdate.isValid()) {
    qDebug() << "Date couldn't be parsed:" << date;
    return Unknown;
  }
  
  qDebug() << code << qdate.toString(Qt::ISODate);

  return StorePrice(tokens, sv);
}



bool saveEODline(Db& db, Tokens_t& tokens)
{
  QByteArray sv;
  if(Unknown == returnSec(tokens, sv))
    return false;

  QByteArray date = tokens.at(DatePos);
  QDate qdate; //RaceCond: = QDate::fromString(date, "yyyyMMdd");
  ConvertToQDate(date, qdate);
  
  if(!qdate.isValid()) {
    QByteArray code = tokens.at(CodePos);
    qWarning() << "Invalid date" << date << code;
    return false;
  }
  int jday = qdate.toJulianDay();
  
  Dbt key(&jday, sizeof(jday));
  Dbt data(sv.data(), sv.size());
  //data.set_flags(DB_DBT_USERMEM);

  db.put(NULL, &key, &data, 0);
  return true;
}


} // end namespace
