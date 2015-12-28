include( $$(QMKBLD)/root.pri )

QT += widgets
TEMPLATE = app
TARGET = ftsdb_view
DESTDIR = $$PRJ_BIN
DEPENDPATH += . 
INCLUDEPATH += . ../ftsdb
INCLUDEPATH += $$(QDECIMAL)/decnumber $$(QDECIMAL)/src
INCLUDEPATH += $$(QT_BDB)/bdb/include
LIBS += -L$$PRJ_LIB -lftsdb
LIBS += -L$$(QDECIMAL)/lib -lqdecimal -ldecnumber


win32 {
 LIBS += $$PRJ_BDB_LIB/libdb_stl53d.lib $$PRJ_BDB_LIB/libdb53d.lib
 SOURCES += $$PRJ_BDB_SRC/clib/getopt.c
 DEFINES -= UNICODE
}

unix {
 LIBS += -L$$(QT_BDB)/bdb/lib -ldb_cxx -ldb
 POST_TARGETDEPS += $$PRJ_LIB/libftsdb.a
}

# Input
HEADERS += MainWindow.hh 
SOURCES += main.cc MainWindow.cc
RESOURCES += main.qrc
