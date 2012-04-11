
#include <iostream>
#include <QDate>
#include <QDateTime>

#include "sec.hh"
#include "gtest/gtest.h"

using namespace std;
using namespace TC;

TEST(FtsdbBasic, Sizes) {
  //EXPECT_EQ()

  cout << "sec_single=" << sizeof(sec_single) << endl;
  cout << "sec_single2=" << sizeof(sec_single2) << endl;
  cout << "sec_double=" << sizeof(sec_double) << endl;
  cout << "sec_double2=" << sizeof(sec_double2) << endl;
  cout << "sec_quad=" << sizeof(sec_quad) << endl;

  cout << "esec_single=" << sizeof(esec_single) << endl;
  cout << "esec_single2=" << sizeof(esec_single2) << endl;
  cout << "esec_double=" << sizeof(esec_double) << endl;
  cout << "esec_double2=" << sizeof(esec_double2) << endl;
  cout << "esec_quad=" << sizeof(esec_quad) << endl;

  cout << "QDate=" << sizeof(QDate) << endl;
  cout << "QDateTime=" << sizeof(QDateTime) << endl;
  QDate today = QDate::currentDate();
  QDateTime now = QDateTime::currentDateTime();

  cout << "julianDay=" << today.toJulianDay() << endl;

  QByteArray b = QByteArray((const char*)&today, sizeof(today));
  cout << "today=" << b.toHex().data() << endl;
  cout << "today=" << *((quint32*)&today) << endl;

  b = QByteArray((const char*)&now, sizeof(now));
  cout << "now=" << b.toHex().data() << endl;
  cout << "now=" << *((quint64*)&now) << endl;
  cout << "now=" << now.toMSecsSinceEpoch() << endl;
  
  EXPECT_TRUE(1);
}
