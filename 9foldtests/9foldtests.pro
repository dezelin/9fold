#-------------------------------------------------
#
# Project created by QtCreator 2015-07-10T23:17:52
#
#-------------------------------------------------

QT       += widgets declarative qml quick network opengl sql script scripttools svg webkit webkitwidgets xml xmlpatterns multimedia testlib dbus printsupport

TARGET = tst__9foldteststest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    main.cpp \
    9foldtest.cpp \
    9foldpatterns/singletontest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldpatterns/release/ -l9foldpatterns
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldpatterns/debug/ -l9foldpatterns
else:unix: LIBS += -L$$OUT_PWD/../9foldpatterns/ -l9foldpatterns

INCLUDEPATH += $$PWD/../9foldpatterns
DEPENDPATH += $$PWD/../9foldpatterns

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldscene/release/ -l9foldscene
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldscene/debug/ -l9foldscene
else:unix: LIBS += -L$$OUT_PWD/../9foldscene/ -l9foldscene

INCLUDEPATH += $$PWD/../9foldscene
DEPENDPATH += $$PWD/../9foldscene

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldwidgets/release/ -l9foldwidgets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldwidgets/debug/ -l9foldwidgets
else:unix: LIBS += -L$$OUT_PWD/../9foldwidgets/ -l9foldwidgets

INCLUDEPATH += $$PWD/../9foldwidgets
DEPENDPATH += $$PWD/../9foldwidgets

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../9foldgeometry/release/ -l9foldgeometry
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../9foldgeometry/debug/ -l9foldgeometry
else:unix: LIBS += -L$$OUT_PWD/../9foldgeometry/ -l9foldgeometry

INCLUDEPATH += $$PWD/../9foldgeometry
DEPENDPATH += $$PWD/../9foldgeometry

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

HEADERS += \
    9foldtestrunner.h \
    9foldtest.h \
    9foldpatterns/singletontest.h
