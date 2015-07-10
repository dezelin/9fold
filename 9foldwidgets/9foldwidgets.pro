#-------------------------------------------------
#
# Project created by QtCreator 2015-07-10T18:18:32
#
#-------------------------------------------------

QT       += widgets network opengl sql svg webkit webkitwidgets xml xmlpatterns multimedia testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TARGET = 9foldwidgets
TEMPLATE = lib

DEFINES += _9FOLDWIDGETS_LIBRARY

SOURCES += _9foldwidgets.cpp

HEADERS += _9foldwidgets.h\
        9foldwidgets_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
