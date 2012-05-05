/*-
 *
 * Copyright (c) Semih Cemiloglu
 *
 * $Id$ 
 */

#ifndef TC_FTSDB_HH
#define TC_FTSDB_HH

#include <QByteArray>

// FORWARDS
class Db;
class DbEnv;

namespace TC {

class FtsDb
{
  
public:
  // CREATORS
  // Constructor requires a path to the database,
  // and a database name.
  FtsDb(const QByteArray& homeDir,
        const QByteArray& dbFile,
        const QByteArray& dbName = "CATALOG",
        unsigned pagesize = 4096);

  ~FtsDb();


  // ACCESSORS
  DbEnv* getDbEnv()
  { return m_dbenv; }
  
  Db* getDb() const
  { return m_db;}

  Db* getMDb() const
  { return m_mdb; }

  QByteArray getHomeDir() const
  { return m_homeDir; }

  QByteArray getDbFile() const
  { return m_dbFile; }
    
  // MODIFIERS
  void close();

  void setPageSize(unsigned ps)
  { m_pagesize = ps; }

private:
  // DB environment
  DbEnv* m_dbenv;
  // FTS database
  Db* m_db;
  // Meta database for FTS
  Db* m_mdb;
  // Flags for the database (u_int32_t)
  quint32 m_flags;
  unsigned m_pagesize;
  // Location of DB files
  QByteArray m_homeDir;
  QByteArray m_dbFile;
  QByteArray m_dbName;
  

  // Make sure the default constructor is private
  // We don't want it used.
  FtsDb() : m_dbenv(0), m_db(0), m_mdb(0) {}

  
};

} // end namespace

#endif
