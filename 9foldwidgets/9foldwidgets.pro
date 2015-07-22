#-------------------------------------------------
#
# Project created by QtCreator 2015-07-10T18:18:32
#
#-------------------------------------------------

QT       += widgets network opengl sql svg webkit webkitwidgets xml xmlpatterns multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TARGET = 9foldwidgets
TEMPLATE = lib

CONFIG += qscintilla2

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wno-unused-parameter

DEFINES += _9FOLDWIDGETS_LIBRARY

SOURCES += _9foldwidgets.cpp \
    centralwidget.cpp \
    _9foldcentralwidget.cpp \
    v8scriptingengineconsole.cpp \
    texteditor.cpp \
    scriptingconsole.cpp \
    javascripttexteditor.cpp \
    scriptingconsoleeditor.cpp \
    javascriptconsoleeditor.cpp

HEADERS += _9foldwidgets.h\
        9foldwidgets_global.h \
    centralwidget.h \
    _9foldcentralwidget.h \
    v8scriptingengineconsole.h \
    texteditor.h \
    scriptingconsole.h \
    javascripttexteditor.h \
    scriptingconsoleeditor.h \
    javascriptconsoleeditor.h

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldmultimedia/release/ -l9foldmultimedia
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldmultimedia/debug/ -l9foldmultimedia
else:unix: LIBS += -L$$OUT_PWD/../9foldmultimedia/ -l9foldmultimedia

INCLUDEPATH += $$PWD/../9foldmultimedia
DEPENDPATH += $$PWD/../9foldmultimedia

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldpatterns/release/ -l9foldpatterns
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldpatterns/debug/ -l9foldpatterns
else:unix: LIBS += -L$$OUT_PWD/../9foldpatterns/ -l9foldpatterns

INCLUDEPATH += $$PWD/../9foldpatterns
DEPENDPATH += $$PWD/../9foldpatterns

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldscripting/release/ -l9foldscripting
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldscripting/debug/ -l9foldscripting
else:unix: LIBS += -L$$OUT_PWD/../9foldscripting/ -l9foldscripting

INCLUDEPATH += $$PWD/../9foldscripting
DEPENDPATH += $$PWD/../9foldscripting

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldv8/release/ -l9foldv8
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldv8/debug/ -l9foldv8
else:unix: LIBS += -L$$OUT_PWD/../9foldv8/ -l9foldv8

INCLUDEPATH += $$PWD/../9foldv8
DEPENDPATH += $$PWD/../9foldv8

LIBS+= -lqt5scintilla2
