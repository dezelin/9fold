#-------------------------------------------------
#
# Project created by QtCreator 2015-07-10T18:19:47
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = 9foldscene
TEMPLATE = lib

DEFINES += _9FOLDSCENE_LIBRARY

SOURCES += _9foldscene.cpp

HEADERS += _9foldscene.h\
        9foldscene_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
