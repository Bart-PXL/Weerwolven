TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += ZMQ_STATIC
LIBS += -L$$PWD/../lib -lzmq -lws2_32 -lIphlpapi
INCLUDEPATH += D:\School\NP\include

SOURCES += \
        main.c
