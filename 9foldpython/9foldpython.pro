#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T03:29:25
#
#-------------------------------------------------

QT       -= gui

TARGET = 9foldpython
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11 -Wall

DEFINES += _9FOLDPYTHON_LIBRARY

SOURCES += _9foldpython.cpp

HEADERS += _9foldpython.h\
        9foldpython_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldscripting/release/ -l9foldscripting
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldscripting/debug/ -l9foldscripting
else:unix: LIBS += -L$$OUT_PWD/../9foldscripting/ -l9foldscripting

INCLUDEPATH += $$PWD/../9foldscripting
DEPENDPATH += $$PWD/../9foldscripting
