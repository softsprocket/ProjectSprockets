#-------------------------------------------------
#
# Project created by QtCreator 2017-04-30T07:37:20
#
#-------------------------------------------------

QT       += core gui qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sprockets
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    projectwizard.cpp \
    filetreedock.cpp \
    jshighlighter.cpp \
    editorlinenumbers.cpp \
    codeeditor.cpp \
    scratchpad.cpp \
    snippetdb.cpp \
    snippettree.cpp \
    tooltabs.cpp \
    snippetwidget.cpp \
    snippetdialog.cpp

HEADERS  += mainwindow.h \
    projectwizard.h \
    filetreedock.h \
    jshighlighter.h \
    editorlinenumbers.h \
    codeeditor.h \
    scratchpad.h \
    snippetdb.h \
    snippettree.h \
    tooltabs.h \
    snippetwidget.h \
    snippetdialog.h

FORMS    +=


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LibSprockets/release/ -lLibSprockets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LibSprockets/debug/ -lLibSprockets
else:unix: LIBS += -L$$OUT_PWD/../LibSprockets/ -lLibSprockets

INCLUDEPATH += $$PWD/../LibSprockets
DEPENDPATH += $$PWD/../LibSprockets

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../fakevim/release/ -lfakevim
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../fakevim/debug/ -lfakevim
else:unix: LIBS += -L$$OUT_PWD/../fakevim/ -lfakevim

INCLUDEPATH += $$PWD/../fakevim
DEPENDPATH += $$PWD/../fakevim
