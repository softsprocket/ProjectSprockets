#-------------------------------------------------
#
# Project created by QtCreator 2017-07-16T07:57:35
#
#-------------------------------------------------

QT       -= gui
QT += qml
TARGET = LibSprockets
TEMPLATE = lib

DEFINES += LIBSPROCKETS_LIBRARY

SOURCES += libsprockets.cpp

HEADERS += libsprockets.h\
        libsprockets_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
