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
    socks6util_packet.cc \
    socks6util_idempotence.cc \
    socks6util_socket.cc \
    socks6util_cbindings.cc

HEADERS += \
    socks6util_idempotence.hh \
    socks6util.h \
    socks6util.hh \
    socks6util_packet.hh \
    socks6util_socket.hh \
    socks6util_socketaddress.hh

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
