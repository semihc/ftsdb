/*-
 * Copyright (c) Semih Cemiloglu
 *
 * $Id$ 
 */

#include "FtsDb.hh"
#include <iostream>
#include <db_cxx.h>
#include <QDir>

using namespace std;


namespace TC {

// Class constructor. Requires a path to the location
// where the database is located, and a database name
FtsDb::FtsDb(const QByteArray& homeDir,
             const QByteArray& dbFile,
             const QByteArray& dbName,
             unsigned pagesize) :
    m_dbenv(0), m_db(0), m_mdb(0),
    m_flags(0), m_pagesize(pagesize),
    m_homeDir(homeDir),
    m_dbFile(dbFile),
    m_dbName(dbName)
{
  bool isSecondary = false;
  
  try {

    QDir dir(homeDir);
    if(!dir.exists())
      dir.mkpath(".");
    
    m_dbenv = new DbEnv(0);
    m_dbenv->set_error_stream(&cerr);
    m_dbenv->set_message_stream(&cout);
    //m_dbenv->set_cachesize(0, 64 * 1024, 0);

    // Open the environment with full transactional support.
    m_dbenv->open(homeDir.data(),
                  DB_INIT_LOCK | DB_INIT_LOG | DB_INIT_MPOOL | DB_INIT_TXN
                  | DB_THREAD | DB_CREATE, 0644);

    m_flags = DB_CREATE|DB_THREAD;
    m_db = new Db(m_dbenv, 0);
    // Redirect debugging information to std::cerr
    m_db->set_error_stream(&cerr);

    // If this is a secondary database, support
    // sorted duplicates
    if(isSecondary)
      m_db->set_flags(DB_DUPSORT);

    // Set desired pagesize if set
    if(m_pagesize > 0) 
      m_db->set_pagesize(m_pagesize);

    
    // Open the database
    m_db->open(0, m_dbFile.data(), m_dbName.data(), DB_BTREE, m_flags, 0644);
  }
  // DbException is not a subclass of std::exception, so we
  // need to catch them both.
  catch(DbException& e) {
    cerr << "Error opening database: " << m_dbFile.data() << "\n";
    cerr << e.what() << endl;
    throw e;
  }
  catch(std::exception& e) {
    cerr << "Error opening database: " << m_dbFile.data() << "\n";
    cerr << e.what() << endl;
    throw e;
  }
  
}


FtsDb::~FtsDb()
{
  close();
  delete m_db;
  delete m_mdb;
  delete m_dbenv;
}


// Close a database. Called from the class
void FtsDb::close()
{
  // Close the db
  try
  {
    m_db->close(0);
    cout << "Database " << m_dbFile.data()  << " is closed." << endl;
    m_dbenv->close(0);
  }
  catch(DbException& e) {
    cerr << "Error closing database: " << m_dbFile.data() << "\n";
    cerr << e.what() << endl;
  }
  catch(std::exception& e) {
    cerr << "Error closing database: " << m_dbFile.data() << "\n";
    cerr << e.what() << endl;
  }
}

} // end namespace


// Btree key comparison function
int CompareInt(Db* db, const Dbt* a, const Dbt* b)
{
  int ai, bi;

  memcpy(&ai, a->get_data(), sizeof(int));
  memcpy(&bi, b->get_data(), sizeof(int));
  return (ai - bi);
}

