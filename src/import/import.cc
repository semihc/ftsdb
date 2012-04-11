#include "load.hh"
#include <QByteArray>
#include <QDebug>
#include <QSharedPointer>
#include <QThreadPool>
#include <QCoreApplication>
#include <QAtomicInt>
#include <QMap>
#include <QDir>
#include <QStringList>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <db_cxx.h>

using namespace std;
using namespace TC;


namespace {

QSharedPointer<FtsDb> ftsdb;
QAtomicInt recnt(0);
QQueue<QByteArray> line_queue;
QWaitCondition q_empty;
QWaitCondition q_nonempty;
QMutex q_mutex;

/*
struct InsTask : public QRunnable {

  int m_id;
  InsTask(int id) : m_id(id) {}

  void run() {
    QByteArray line;
    while(true) {
      try {
        line.clear();
        q_mutex.lock();
        q_nonempty.wait(&q_mutex);
        if(!line_queue.isEmpty())
          line = line_queue.dequeue();
        q_mutex.unlock();
        if(line.isEmpty())
          continue;
        
        Tokens_t tokens;
        bool b = splitLine(line, tokens);
        if(!b) {
          qDebug() << "Cannot split line:" << line;
          continue;
        }
        //TODO
        qDebug() << m_id << line;
      }
      catch(DbException& e) {
        qWarning() << e.what();
      }
      catch(exception& e) {
        qWarning() << e.what();
      }
    }    
  } // end of run()
  
};
*/

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
      qDebug() << "Calling quit()";
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

  QRegExp flagre("--(\\S+)=(.*)");
  QStringList args = qcore.arguments();
  foreach(QString flag, args) {
    if(flagre.exactMatch(flag)) {
      QString fkey = flagre.cap(1);
      QString fval = flagre.cap(2);
      argsMap.insert(fkey.toLower(), fval);
    }
  }

  QByteArray homeDir = argsMap.value("exchange","ASX").toAscii();
  QByteArray dbFile = argsMap.value("dbfile","asx.db").toAscii();
  QString impFile = argsMap.value("import-file");
  QString impDir = argsMap.value("import-dir");
  QString impFilter = argsMap.value("import-filter");
  QStringList impFileList;
  QStringList impFilterList;
  
  QDir dir(impDir);

  if(!dir.exists()) {
    qWarning() << dir.dirName() << "does not exist";
    return 1;
  }

  if(!impFilter.isEmpty()) {
    impFilterList += impFilter;
    impFileList += dir.entryList(impFilterList, QDir::Files);
  }

  if(!impFile.isEmpty()) {
    impFileList += impFile;
  }

  if(impFileList.isEmpty()) {
    qWarning() << "No import file is specified.";
    return -1;
  }

  qDebug() << "import file list=" << impFileList;
  
  ftsdb = QSharedPointer<FtsDb>(new FtsDb(homeDir, dbFile));

  foreach(impFile, impFileList) {
    qDebug() << "importing" << impFile;
    QList<QByteArray> lines, tokens;
    bool b = readFile(impFile, lines);
    if(!b) {
      qDebug() << "Reading " << impFile << "failed" ;
      return -1;
    }

    foreach(QByteArray line, lines) {
      b = splitLine(line, tokens);
      if(!b) {
        qDebug() << "Cannot split line:" << line;
        continue;
      }
      //1
      InsertTask* it = new InsertTask(tokens, ftsdb->getDbEnv(), dbFile);
      recnt.fetchAndAddAcquire(1); // Increment;
      QThreadPool::globalInstance()->start(it);
      //2
      // InsertTask it(tokens, ftsdb->getDbEnv(), dbFile);
      // it.run();
    } // foreach


  } //foreach

  int ret = qcore.exec();

  if(!argsMap.value("stat").isEmpty()) {
    ftsdb->getDbEnv()->stat_print(DB_STAT_ALL);
  }

  return ret;
}

