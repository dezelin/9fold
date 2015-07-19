#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T05:31:37
#
#-------------------------------------------------

QT       -= gui

TARGET = 9foldscripting
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11 -Wall

DEFINES += _9FOLDSCRIPTING_LIBRARY

SOURCES += _9foldscripting.cpp \
    scriptingengine.cpp

HEADERS += _9foldscripting.h\
        9foldscripting_global.h \
    scriptingengine.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldgeometry/release/ -l9foldgeometry
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldgeometry/debug/ -l9foldgeometry
else:unix: LIBS += -L$$OUT_PWD/../9foldgeometry/ -l9foldgeometry

INCLUDEPATH += $$PWD/../9foldgeometry
DEPENDPATH += $$PWD/../9foldgeometry

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldscene/release/ -l9foldscene
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldscene/debug/ -l9foldscene
else:unix: LIBS += -L$$OUT_PWD/../9foldscene/ -l9foldscene

INCLUDEPATH += $$PWD/../9foldscene
DEPENDPATH += $$PWD/../9foldscene
