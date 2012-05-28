#include "unload.hh"
#include <QByteArray>
#include <QDebug>
#include <QSharedPointer>
#include <QCoreApplication>
#include <QMap>
#include <QDate>
#include <QStringList>
#include <db_cxx.h>

using namespace std;
using namespace TC;


namespace {

QSharedPointer<FtsDb> ftsdb;

} // end of namespace

int main(int argc, char** argv)
{
  QCoreApplication qcore(argc, argv);
  // Map of arguments
  QMap<QString, QString> argsMap;

  QRegExp flagre("--(\\w+)(=?)(.*)");
  QStringList args = qcore.arguments();
  foreach(QString flag, args) {
    if(flagre.exactMatch(flag)) {
      QString fkey = flagre.cap(1);
      QString fval = flagre.cap(3);
      argsMap.insert(fkey.toLower(), fval);
      qDebug() << fkey << fval;
    }
  }

  QByteArray homeDir = argsMap.value("exchange","ASX").toAscii();
  QByteArray dbFile = argsMap.value("dbfile","asx.db").toAscii();
  QString expFile = argsMap.value("export");
  QString expDate = argsMap.value("date");
  QString expSec = argsMap.value("security"); 
  QString expSList = argsMap.contains("seclist") ? "1" : "";

  if(expFile.isEmpty()) {
    qWarning() << "No export file is specified" ;
    //return -1;
  }

  
  ftsdb = QSharedPointer<FtsDb>(new FtsDb(homeDir, dbFile));
  int ret = 0;

  if(!expDate.isEmpty()) {
    QDate qdate = QDate::fromString(expDate, "yyyyMMdd");
    ret = ShowAllRecordsByDate(qdate, *ftsdb);
    return ret;
  }

  if(!expSec.isEmpty()) {
    //ret = ShowAllRecordsBySecurity(expSec.toAscii(), *ftsdb); 
    ret = ShowAllRecordsBySecSTL(expSec.toAscii(), *ftsdb);
    return ret;
  }

  if(!expSList.isEmpty()) {
    QStringList slist;
    ret = ShowAllDBNames(*ftsdb, slist);
    foreach(QString s, slist) {
      qDebug() << s;
    }
    return ret;
  }
  
  return ret;
}

