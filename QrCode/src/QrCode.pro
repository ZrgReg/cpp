#-------------------------------------------------
# Project created by QtCreator 2017-08-17T19:10:53
#-------------------------------------------------

WGT = ../../_wgt
SRV = ../../_srv
UNT = ../../_unt
LIB = ../../_lib

QT += core gui widgets sql
CONFIG += c++14

TARGET = QrCode
TEMPLATE = app

include($$SRV/Srv/Srv.pri)
include($$WGT/Log/WLog.pri)

INCLUDEPATH += ../../
DEPENDPATH  += ../../

SOURCES += \
    $$LIB/QrCodeGenerator/QrCode.cpp    \
    $$LIB/QrCodeGenerator/BitBuffer.cpp \
    $$LIB/QrCodeGenerator/QrSegment.cpp \
    $$UNT/QrGen.cpp     \
    $$UNT/QrMng.cpp     \
    main.cpp            \
    FMain.cpp           \
    ../dll/qr_gen.cpp   \
    _tst/XmlParse.cpp   \
    _wgt/WImg/WImg.cpp


HEADERS += \
    $$LIB/QrCodeGenerator/QrCode.hpp    \
    $$LIB/QrCodeGenerator/BitBuffer.hpp \
    $$LIB/QrCodeGenerator/QrSegment.hpp \
    $$UNT/QrGen.h       \
    $$UNT/QrMng.h       \
    FMain.h             \
    ../dll/qr_gen.h     \
    _tst/XmlParse.h     \
    _wgt/WImg/WImg.h

FORMS += \
    FMain.ui            \
    _wgt/WImg/WImg.ui

#Профилирование.
#QMAKE_CXXFLAGS_DEBUG += -g3 -pg
#QMAKE_LFLAGS_DEBUG += -pg -lgmon
