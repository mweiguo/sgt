######################################################################
# Automatically generated by qmake (2.01a) ??? ?? 15 16:27:34 2011
######################################################################

TEMPLATE = app
TARGET = layout
DEPENDPATH += . debug tmp\rcc\release_shared
INCLUDEPATH += . ../renderer/renderer2d ../graphbase ../viewer
#LIBS += -L../renderer/renderer2d -lrender2d
win32:LIBS += ../renderer/renderer2d/librender2d.so ../viewer/debug/libviewer.a
QT += opengl
CONFIG += console
binary.path = ../build/bin
binary.files = debug/layout.exe
INSTALLS += binary

# Input
#HEADERS += glwidget.h layoutmainwindow.h document.h tools.h layermanagerwidget.h
HEADERS += layoutmainwindow.h centerwidget.h layoutdocument.h layouttools.h
SOURCES += main.cpp layoutmainwindow.cpp centerwidget.cpp layoutdocument.cpp layouttools.cpp ../graphbase/mat4f.c
#RESOURCES += dockwidgets.qrc