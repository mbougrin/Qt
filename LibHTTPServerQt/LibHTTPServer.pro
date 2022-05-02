#-------------------------------------------------
#
# Project created by mbougrin 2019-01-24T10:30:49
#
#-------------------------------------------------

QT       -= gui
TARGET = HTTPServer
TEMPLATE = lib
QT += network

CONFIG += c++11 console

DEFINES += LIBHTTPSERVER_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        serverhttp.cpp
HEADERS += \
        serverhttp.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}