#-------------------------------------------------
#
# Project created by QtCreator 2015-07-10T19:32:27
#
#-------------------------------------------------

QT       -= gui

TARGET = 9foldpatterns
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11

DEFINES += _9FOLDPATTERNS_LIBRARY

SOURCES += _9foldpatterns.cpp \
    mvpp.cpp

HEADERS += _9foldpatterns.h\
        9foldpatterns_global.h \
    mvpp.h \
    observer.h \
    singleton.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
