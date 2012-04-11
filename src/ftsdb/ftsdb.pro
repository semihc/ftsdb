include( $$(QMKBLD)/root.pri )

QT -= gui
TEMPLATE = lib
CONFIG += static
TARGET = ftsdb
DESTDIR = $$PRJ_LIB
DEPENDPATH += . 

#message( CONFIG = $$CONFIG )
INCLUDEPATH += . 
INCLUDEPATH += $$(QDECIMAL)/decnumber $$(QDECIMAL)/src
#unix:INCLUDEPATH += $$(QT_BDB)/bdb/include
#win32:INCLUDEPATH += $$(QT_BDB)/bdb/db-5.3.15/build_windows

# Input
HEADERS += sec.hh FtsDb.hh load.hh unload.hh
SOURCES += sec.cc FtsDb.cc load.cc unload.cc

