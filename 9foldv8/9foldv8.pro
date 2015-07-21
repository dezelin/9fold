#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T05:25:43
#
#-------------------------------------------------

QT       -= gui

TARGET = 9foldv8
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra

DEFINES += _9FOLDV8_LIBRARY

SOURCES += _9foldv8.cpp \
    v8scriptingengine.cpp \
    v8scriptingengineworker.cpp

HEADERS += _9foldv8.h\
        9foldv8_global.h \
    v8scriptingengine.h \
    v8scriptingengineworker.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldscripting/release/ -l9foldscripting
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldscripting/debug/ -l9foldscripting
else:unix: LIBS += -L$$OUT_PWD/../9foldscripting/ -l9foldscripting

INCLUDEPATH += $$PWD/../9foldscripting
DEPENDPATH += $$PWD/../9foldscripting

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldpatterns/release/ -l9foldpatterns
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldpatterns/debug/ -l9foldpatterns
else:unix: LIBS += -L$$OUT_PWD/../9foldpatterns/ -l9foldpatterns

INCLUDEPATH += $$PWD/../9foldpatterns
DEPENDPATH += $$PWD/../9foldpatterns

INCLUDEPATH += /home/dezelin/Workspace/v8
LIBS += -L/home/dezelin/Workspace/v8/out/x64.debug/obj.target/tools/gyp -lv8_libplatform
LIBS += -L/home/dezelin/Workspace/v8/out/x64.debug/lib.target/ -lv8
LIBS += -L/home/dezelin/Workspace/v8/out/x64.debug/lib.target/ -licuuc
LIBS += -L/home/dezelin/Workspace/v8/out/x64.debug/lib.target/ -licuuc
