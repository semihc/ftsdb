#ifndef TC_LOAD_HH
#define TC_LOAD_HH

#include <QList>
#include <QByteArray>
#include <db_cxx.h>
#include "sec.hh"
#include "FtsDb.hh"

namespace TC {

// TYPES
//! List of lines
typedef QList<QByteArray> Lines_t;
//! List of tokens
typedef QList<QByteArray> Tokens_t;


//! Position ordinals
enum OHLCVFields {
  CodePos   = 0,
  DatePos   = 1,
  OpenPos   = 2,
  HighPos   = 3,
  LowPos    = 4,  
  ClosePos  = 5,
  VolumePos = 6,
  FieldMax  = 7
};

bool readFile(const QString& fn, Lines_t& lines);
bool splitLine(const QByteArray& line, Tokens_t& tokens);
int NumberOfDigits(const QByteArray& token);
sec_enum BestDecimalContainer(const QByteArray& token);
sec_enum PickSecContainer(const Tokens_t& tokens);
template <typename S>
void StoreOHLCV(const Tokens_t& tokens, S& sd);
sec_enum StorePrice(const Tokens_t tokens, QByteArray& ba);
sec_enum returnSec(Tokens_t& tokens, QByteArray& sv);
bool saveEODline(Db& db, Tokens_t& tokens);


} // end namespace

#endif
