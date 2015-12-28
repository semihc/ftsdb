include( $$(QMKBLD)/root.pri )

QT -= gui
TEMPLATE = app
#CONFIG += qtestlib
QT += testlib
TARGET = ftsdb_test
DESTDIR = $$PRJ_BIN
DEPENDPATH += . 
INCLUDEPATH += . ../src/ftsdb
INCLUDEPATH += $$(QDECIMAL)/decnumber $$(QDECIMAL)/src
INCLUDEPATH += $$(QT_CVT)/inc
LIBS += -L$$(QT_CVT)/lib -lgtm
LIBS += -L$$PRJ_LIB -lftsdb

# Use cvt runner, integrated with Qt qtestlib
LIBS += -lcvt
# Use gtest/gmock runner
#LIBS += -lgtm_runner

win32 {
 LIBS += $$PRJ_BDB_LIB/libdb53d.lib
 SOURCES += $$PRJ_BDB_SRC/clib/getopt.c
 DEFINES -= UNICODE
}

unix {
 LIBS += -L$$(QT_BDB)/bdb/lib -ldb_cxx -ldb
 POST_TARGETDEPS += $$PRJ_LIB/libftsdb.a
}

# Input
#HEADERS += 
SOURCES += ftsdb_test.cc
