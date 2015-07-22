#-------------------------------------------------
#
# Project created by QtCreator 2015-07-10T18:28:39
#
#-------------------------------------------------

QT       += network opengl svg multimedia

QT       -= gui

TARGET = 9foldmultimedia
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wno-unused-parameter

DEFINES += _9FOLDMULTIMEDIA_LIBRARY

SOURCES += _9foldmultimedia.cpp

HEADERS += _9foldmultimedia.h\
        9foldmultimedia_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
