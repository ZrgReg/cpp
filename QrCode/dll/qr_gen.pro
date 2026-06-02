#-------------------------------------------------
# Project created by QtCreator 2017-08-23T19:47:37
#-------------------------------------------------

UNT = ../../_unt
LIB = ../../_lib

QT += core gui
CONFIG += c++14

TARGET = qr_gen
TEMPLATE = lib
CONFIG += dll

INCLUDEPATH += ../../
DEPENDPATH  += ../../

HEADERS += \
    $$LIB/QrCodeGenerator/QrCode.hpp    \
    $$LIB/QrCodeGenerator/BitBuffer.hpp \
    $$LIB/QrCodeGenerator/QrSegment.hpp \
    $$UNT/QrGen.h   \
    $$UNT/QrMng.h   \
    qr_gen.h        \

SOURCES += \
    $$LIB/QrCodeGenerator/QrCode.cpp    \
    $$LIB/QrCodeGenerator/BitBuffer.cpp \
    $$LIB/QrCodeGenerator/QrSegment.cpp \
    $$UNT/QrGen.cpp \
    $$UNT/QrMng.cpp \
    qr_gen.cpp      \
