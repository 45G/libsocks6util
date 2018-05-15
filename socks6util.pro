#-------------------------------------------------
#
# Project created by QtCreator 2018-05-10T04:20:11
#
#-------------------------------------------------

TARGET = socks6util
TEMPLATE = lib
CONFIG += staticlib
CONFIG -= qt
CONFIG -= app_bundle
CONFIG += c++11
CONFIG += c99

SOURCES += \
    socks6util.cc

HEADERS += \
        socks6util.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
