######################################################################
# Automatically generated by qmake (2.01a) ??? ?? 15 16:27:34 2011
######################################################################

TEMPLATE = app
TARGET = viewer
DEPENDPATH += . debug tmp\rcc\release_shared
INCLUDEPATH += . ../renderer/renderer2d
#LIBS += -L../renderer/renderer2d -lrender2d
win32:LIBS += ../renderer/renderer2d/librender2d.so
QT += opengl
CONFIG += console
binary.path = ../build/bin
binary.files = debug/viewer.exe
INSTALLS += binary

# Input
HEADERS += glwidget.h mainwindow.h document.h tools.h layermanagerwidget.h
SOURCES += glwidget.cpp main.cpp mainwindow.cpp document.cpp tools.cpp layermanagerwidget.cpp
RESOURCES += dockwidgets.qrc
