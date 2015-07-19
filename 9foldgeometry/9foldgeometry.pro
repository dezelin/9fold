#-------------------------------------------------
#
# Project created by QtCreator 2015-07-10T18:19:11
#
#-------------------------------------------------

QT       -= gui

TARGET = 9foldgeometry
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11 -Wall

DEFINES += _9FOLDGEOMETRY_LIBRARY

SOURCES += _9foldgeometry.cpp

HEADERS += _9foldgeometry.h\
        9foldgeometry_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
