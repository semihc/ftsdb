include( $$(QMKBLD)/root.pri )

QT -= gui
TEMPLATE = app
TARGET = ftsdb_import
DESTDIR = $$PRJ_BIN
DEPENDPATH += . 
INCLUDEPATH += . ../ftsdb
INCLUDEPATH += $$(QDECIMAL)/decnumber $$(QDECIMAL)/src

LIBS += -L$$PRJ_LIB -lftsdb
LIBS += -L$$(QDECIMAL)/lib -lqdecimal -ldecnumber

win32 {
 LIBS += $$PRJ_BDB_LIB/libdb53d.lib
 SOURCES += $$PRJ_BDB_SRC/clib/getopt.c
 DEFINES -= UNICODE
}

unix {
 INCLUDEPATH += $$(QT_BDB)/bdb/include
 LIBS += -L$$(QT_BDB)/bdb/lib -ldb_cxx -ldb
 #LIBS += -L/opt_TC/lib -lprofiler -ltcmalloc
 POST_TARGETDEPS += $$PRJ_LIB/libftsdb.a
}

# Input
#HEADERS += 
SOURCES += import.cc
