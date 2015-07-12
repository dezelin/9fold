#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T03:29:25
#
#-------------------------------------------------

QT       += widgets

TARGET = 9foldpython
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11

DEFINES += _9FOLDPYTHON_LIBRARY

SOURCES += _9foldpython.cpp

HEADERS += _9foldpython.h\
        9foldpython_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldscene/release/ -l9foldscene
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldscene/debug/ -l9foldscene
else:unix: LIBS += -L$$OUT_PWD/../9foldscene/ -l9foldscene

INCLUDEPATH += $$PWD/../9foldscene
DEPENDPATH += $$PWD/../9foldscene

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldpatterns/release/ -l9foldpatterns
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldpatterns/debug/ -l9foldpatterns
else:unix: LIBS += -L$$OUT_PWD/../9foldpatterns/ -l9foldpatterns

INCLUDEPATH += $$PWD/../9foldpatterns
DEPENDPATH += $$PWD/../9foldpatterns

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldfformats/release/ -l9foldfformats
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldfformats/debug/ -l9foldfformats
else:unix: LIBS += -L$$OUT_PWD/../9foldfformats/ -l9foldfformats

INCLUDEPATH += $$PWD/../9foldfformats
DEPENDPATH += $$PWD/../9foldfformats

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldmultimedia/release/ -l9foldmultimedia
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldmultimedia/debug/ -l9foldmultimedia
else:unix: LIBS += -L$$OUT_PWD/../9foldmultimedia/ -l9foldmultimedia

INCLUDEPATH += $$PWD/../9foldmultimedia
DEPENDPATH += $$PWD/../9foldmultimedia

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldgeometry/release/ -l9foldgeometry
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldgeometry/debug/ -l9foldgeometry
else:unix: LIBS += -L$$OUT_PWD/../9foldgeometry/ -l9foldgeometry

INCLUDEPATH += $$PWD/../9foldgeometry
DEPENDPATH += $$PWD/../9foldgeometry
