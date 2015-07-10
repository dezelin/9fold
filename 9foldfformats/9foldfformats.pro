#-------------------------------------------------
#
# Project created by QtCreator 2015-07-10T18:20:30
#
#-------------------------------------------------

QT       += sql xml xmlpatterns testlib

QT       -= gui

TARGET = 9foldfformats
TEMPLATE = lib

DEFINES += _9FOLDFFORMATS_LIBRARY

SOURCES += _9foldfformats.cpp

HEADERS += _9foldfformats.h\
        9foldfformats_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
