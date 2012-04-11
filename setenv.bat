@echo off

rem If Projects Home directory is not defined
if not defined PRJS_HOME (
  set PRJS_HOME=%HOME%
)

rem QmkBld related variables
set QMKBLD=%PRJS_HOME%\qmkbld
set QMK_DEBUG=0
set QMAKEFEATURES=%QMKBLD%\prf


rem Other projects that this projects depend on:
set QDECIMAL=%PRJS_HOME%/qdecimal
set QT_CVT=%PRJS_HOME%\qt-cvt
set QT_BDB=%PRJS_HOME%\qt-bdb
set PATH=%PATH%;%QT_BDB%\bdb\db-5.3.15\build_windows\Win32\Debug

rem The project that uses QmkBld = ftsdb
set PRJ_ROOT=%PRJS_HOME%\ftsdb

@echo PRJ_ROOT=%PRJ_ROOT%
