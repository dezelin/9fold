#-------------------------------------------------
#
# Project created by QtCreator 2015-07-10T07:31:33
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = 9fold
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -Wall


SOURCES += main.cpp\
        mainwindow.cpp \
    document.cpp \
    documentview.cpp \
    documentpresenter.cpp \
    documentmanager.cpp \
    documentviewmanager.cpp \
    _9folddocument.cpp \
    _9folddocumentpresenter.cpp \
    _9folddocumentview.cpp \
    _9folddocumentmanager.cpp \
    _9folddocumentviewmanager.cpp \
    workspace.cpp \
    actionmanager.cpp \
    toolbarmanager.cpp \
    dockmanager.cpp \
    _9folddockmanager.cpp \
    _9foldactionmanager.cpp \
    _9foldtoolbarmanager.cpp \
    _9foldworkspace.cpp \
    _9foldmainwindow.cpp \
    menumanager.cpp \
    _9foldmenumanager.cpp \
    commandmanager.cpp \
    _9foldcommandmanager.cpp \
    _9foldaction.cpp \
    _9foldjavascriptdocument.cpp \
    _9foldjavascriptdocumentpresenter.cpp \
    _9foldjavascriptdocumentview.cpp

HEADERS  += mainwindow.h \
    document.h \
    documentview.h \
    documentpresenter.h \
    documentmanager.h \
    documentviewmanager.h \
    _9folddocument.h \
    _9folddocumentpresenter.h \
    _9folddocumentview.h \
    _9folddocumentmanager.h \
    _9folddocumentviewmanager.h \
    workspace.h \
    actionmanager.h \
    toolbarmanager.h \
    dockmanager.h \
    _9folddockmanager.h \
    _9foldactionmanager.h \
    _9foldtoolbarmanager.h \
    _9foldworkspace.h \
    _9foldmainwindow.h \
    menumanager.h \
    _9foldmenumanager.h \
    commandmanager.h \
    _9foldcommandmanager.h \
    _9foldaction.h \
    _9foldjavascriptdocument.h \
    _9foldjavascriptdocumentpresenter.h \
    _9foldjavascriptdocumentview.h

FORMS    += mainwindow.ui

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
