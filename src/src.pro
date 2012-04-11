
include( $$(QMKBLD)/root.pri )


TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = ftsdb import export view
