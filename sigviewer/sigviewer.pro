######################################################################
# Automatically generated by qmake (2.01a) ??? ?? 18 22:53:47 2010
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
DESTDIR = ../bin
INCLUDEPATH += . ./.. ./../graphbase ./../renderer /local/include ../../stratumSimulatior/tinylog/src c:/Qt/2010.02.1/qt/include/QtOpenGL
LIBS += -L./../bin -L/local/lib -lsigrenderer -ltcl85 -ltk85 ../../stratumSimulatior/tinylog/src/tinylog.a -lQtOpenGLd4

# Input
HEADERS += view.h qviewport.h sgv_tools.h tcltkconsole.h qglviewport.h examine.h flytool.h handtool.h lookat.h walktool.h zoomtool.h coordquery.h locatetool.h keyboardmanipulator.h
SOURCES += view.cpp main.cpp qviewport.cpp sgv_tools.cpp tcltkconsole.cpp qglviewport.cpp handtool.cpp zoomtool.cpp coordquery.cpp locatetool.cpp keyboardmanipulator.cpp
