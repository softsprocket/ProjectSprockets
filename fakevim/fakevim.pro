#-------------------------------------------------
#
# Project created by QtCreator 2017-08-06T04:46:22
#
#-------------------------------------------------

QT       -= gui
QT += widgets

TARGET = fakevim
TEMPLATE = lib

DEFINES += FAKEVIM_LIBRARY

SOURCES += \
    fakevimactions.cpp \
    fakevimhandler.cpp \
    utils/qtcassert.cpp

HEADERS += \
    fakevimactions.h \
    fakevimhandler.h \
    fakevimtr.h \
    private/fakevim_export.h \
    utils/hostosinfo.h \
    utils/qtcassert.h \
    utils/utils_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    utils/utils.pri \
    fakevim.pri

