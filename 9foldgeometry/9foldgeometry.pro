#-------------------------------------------------
#
# Project created by QtCreator 2015-07-10T18:19:11
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = 9foldgeometry
TEMPLATE = lib

DEFINES += _9FOLDGEOMETRY_LIBRARY

SOURCES += _9foldgeometry.cpp

HEADERS += _9foldgeometry.h\
        9foldgeometry_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
