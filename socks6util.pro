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
    cbindings.cc \
    idempotence.cc \
    packet.cc \
    socket.cc \
    requestsafety.cc

HEADERS += \
    socks6util.h \
    socks6util.hh \
    idempotence.hh \
    packet.hh \
    socket.hh \
    socketaddress.hh \
    requestsafety.hh

unix {
    headers.path = /usr/local/include/socks6util
    headers.files += $$HEADERS
    INSTALLS += headers
    exists(/usr/local/lib64) {
        target.path = /usr/local/lib64
    }
    else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}
