#
#
#

isEmpty(QMKBLD_PRJCFG_INCLUDED) { # include guard
QMKBLD_PRJCFG_INCLUDED = 1

#
# Main project build modes
#
PRJ_CONFIG = debug
#PRJ_CONFIG = release


# Project wide coverage support
#PRJ_COVERAGE = 1

# Project wide profiling support
#PRJ_PROFILE = 1


#
# Sun Studio specific items
#
contains(QMK_COMP,Sunpro) {

# Printing compiler steps
#PRJ_CONFIG *= ss_compstep

# Stack overflow check
#PRJ_CONFIG *= ss_stkovf

# Coverage support
#PRJ_CONFIG *= ss_tcov

# Thread analyzer support
#PRJ_CONFIG *= ss_tha

!isEmpty(PRJ_COVERAGE): PRJ_CONFIG *= ss_tcov

}

#
# Windows specific configuration
#
win32 {
	PRJ_CONFIG += console
}

unix {
 #-fno-omit-frame-pointer -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
 #QMAKE_CXXFLAGS += -fno-omit-frame-pointer
}

isQmkDebug() {
	message(PRJ_CONFIG = $$PRJ_CONFIG)
}

# Now transfer project configuration tokens into CONFIG
CONFIG += $$PRJ_CONFIG

# Use PRJ_BDB_ROOT variable as the pointer to qt-bdb root directory
PRJ_BDB_ROOT = $$(QT_BDB)/bdb
PRJ_BDB = $$PRJ_BDB_ROOT/db-5.3.15
isQmkDebug() {
 message(PRJ_BDB_ROOT = $$PRJ_BDB_ROOT)
 message(PRJ_BDB = $$PRJ_BDB_BASE)
}
win32 {
 PRJ_BDB_SRC = $$PRJ_BDB/src
 PRJ_BDB_LANG = $$PRJ_BDB/lang	
 PRJ_BDB_LIB = $$PRJ_BDB/build_windows/Win32/Debug
 INCLUDEPATH += . $$PRJ_BDB/build_windows $$PRJ_BDB/lang/cxx/stl
 LIBS += -L$$PRJ_BDB_LIB
}
else {
 DEPENDPATH += . $$PRJ_BDB/include
 INCLUDEPATH += . $$PRJ_BDB/include
}


} # end of include guard