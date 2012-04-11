#include "unload.hh"
#include <QByteArray>
#include <QDebug>
#include <QSharedPointer>
#include <QThreadPool>
#include <QCoreApplication>
#include <QAtomicInt>
#include <QMap>
#include <QDate>
#include <QStringList>
#include <db_cxx.h>

using namespace std;
using namespace TC;


namespace {

QSharedPointer<FtsDb> ftsdb;
QAtomicInt recnt;

struct InsertTask : public QRunnable {
  // MEMBERS
  Tokens_t m_tokens;
  DbEnv* m_dbenv;
  QByteArray m_dbFile;

  // CREATORS
  InsertTask(const Tokens_t& tokens, DbEnv* dbenv,
             const QByteArray& dbFile)
      : m_tokens(tokens), m_dbenv(dbenv), m_dbFile(dbFile) {}

  void run() {
    try {
    QByteArray code = m_tokens.at(CodePos).trimmed();
    if(code.isEmpty()) {
      qWarning() << " Cannot determine code of the security";
      return;
    }

    // m_dbFile.append(code.mid(0,1).toUpper());
    
    Db db(m_dbenv, 0);
    db.open(0, m_dbFile.data(), code.data(), DB_BTREE,
            DB_CREATE|DB_THREAD, 0644);
    saveEODline(db, m_tokens);
    db.close(0);
    int rval = recnt.fetchAndAddAcquire(-1); // Decrement;
    // Are we the last task?
    if(rval==1) // Value before the decrement
      QCoreApplication::quit();
    }
    catch(DbException& e) {
      cerr << e.what() << endl;
    }
  }
  
}; // end of struct

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
    ret = ShowAllRecordsBySecurity(expSec.toAscii(), *ftsdb);
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

