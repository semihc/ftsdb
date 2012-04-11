#ifndef TC_SEC_HH
#define TC_SEC_HH

// Qt
#include <QMetaType>
#include <QVector>

// From QDecimal project
#include <QDecDouble.hh>
#include <QDecSingle.hh>
#include <QDecQuad.hh>


namespace TC {

//! Basic security data structure

struct sec_single {
  QDecSingle opn,hi,lo,cls; //!< 7-digits, 4 bytes
  QDecSingle vol; //!< 7-digits, 4 bytes
};

struct sec_single2 {
  QDecSingle opn,hi,lo,cls; //!< 7-digits, 4 bytes
  QDecDouble vol; //!< 16-digits, 8 bytes
};

struct sec_double {
  QDecDouble opn,hi,lo,cls;  //!< 16-digits, 8 bytes
  QDecDouble vol; //!< 16-digits, 8 bytes
};

struct sec_double2 {
  QDecDouble opn,hi,lo,cls;  //!< 16-digits, 8 bytes
  QDecQuad vol; //!< 32-digits, 16 bytes
};

struct sec_quad {
  QDecQuad opn,hi,lo,cls; //!< 32-digits, 16 bytes
  QDecQuad vol; //!< 32-digits, 16 bytes
};

//TODO;
template <typename S>
bool IsSecValid(const S& s)
{
  if(s.vol.isNaN() )
     //s.opn.isNaN() ||
     //s.cls.isNaN() )
    return false;
  else
    return true;
}

//! Extended security data structure
struct esec_single : public sec_single {
  QDecSingle opi,bid,ask;
};

struct esec_single2 : public sec_single2 {
  QDecSingle opi,bid,ask;
};

struct esec_double : public sec_double {
  QDecDouble opi,bid,ask;
};

struct esec_double2 : public sec_double2 {
  QDecDouble opi,bid,ask;
};

struct esec_quad : public sec_quad {
  QDecQuad opi,bid,ask;
};


//! Unumerated valued for security types
enum sec_enum {
  Unknown      = 0,
  SecSingle    = 1,
  SecSingle2   = 2,
  SecDouble    = 3,
  SecDouble2   = 4,
  SecQuad      = 5,
  ESecSingle   = 6,
  ESecSingle2  = 7,
  ESecDouble   = 8,
  ESecDouble2  = 9,
  ESecQuad     = 10,
  SecEnumMax   = 11
};


// Position ordinals for the 
enum price_enum {
  PTOpen   = 0,
  PTHigh   = 1,
  PTLow    = 2,
  PTClose  = 3,
  PTVolume = 4,
  PTOpenInterest = 5,
  PTBid    = 6,
  PTAsk    = 7,
  PTMax    = 8
};
  
typedef QVector<double> PriceTuple_t;

} // end namespace


// Preparation for QVariant use
Q_DECLARE_METATYPE(TC::sec_single);
Q_DECLARE_METATYPE(TC::sec_single2);
Q_DECLARE_METATYPE(TC::sec_double);
Q_DECLARE_METATYPE(TC::sec_double2);
Q_DECLARE_METATYPE(TC::sec_quad);

// Preparation for QVariant use
Q_DECLARE_METATYPE(TC::esec_single);
Q_DECLARE_METATYPE(TC::esec_single2);
Q_DECLARE_METATYPE(TC::esec_double);
Q_DECLARE_METATYPE(TC::esec_double2);
Q_DECLARE_METATYPE(TC::esec_quad);


#endif
