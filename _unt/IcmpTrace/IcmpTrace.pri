INCLUDEPATH +=  $$PWD
DEPENDPATH  +=  $$PWD

HEADERS += \
    $$PWD/IcmpTrace.h   \

SOURCES += \
    $$PWD/IcmpTrace.cpp \

LIBS += -lws2_32
LIBS += -liphlpapi
