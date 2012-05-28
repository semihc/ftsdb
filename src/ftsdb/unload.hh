#ifndef TC_UNLOAD_HH
#define TC_UNLOAD_HH

// Qt
// Project
#include "sec.hh"
#include "load.hh"

// FORWARDS
class QStringList;
class QByteArray;
class QDate;

namespace TC {

int ShowAllDBNames(FtsDb& ftsdb, QStringList& slist);
int ShowAllRecordsBySecSTL(const QByteArray& sec, FtsDb& db);
int ShowAllRecordsBySecurity(const QByteArray& sec, FtsDb& db);
int ShowAllRecordsByDate(const QDate& date, FtsDb& db);

sec_enum PickSecContainer(int size);

template <typename S>
void SecToTuple(const S& s, PriceTuple_t& pt);

template <typename ES>
void ESecToTuple(const ES& es, PriceTuple_t& pt);

bool  ConvertToSec(void* data, int size, PriceTuple_t& pt);


} // end namespace

#endif
