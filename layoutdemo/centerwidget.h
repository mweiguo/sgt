#ifndef _CENTER_WIDGET_H_
#define _CENTER_WIDGET_H_

#include <QtOpenGL/QGLWidget>
#include <glwidget.h>
class Document;
class Tools;
class MainWindow;
class QScrollBar;
class GLScrollWidget;


struct ViewerContext;

class GLItemsWidget : public GLMainView
{
public:
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    GLItemsWidget ( MainWindow* context, int* mainSceneId, const QGLFormat& fmt, QWidget* parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
=======
    GLItemsWidget ( ViewerContext* context, Tools* tools, int* mainSceneId, const QGLFormat& fmt, QWidget* parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
>>>>>>> 47b45ba... fix some bugs
=======
    GLItemsWidget ( ViewerContext* context, Tools* tools, int* mainSceneId, const QGLFormat& fmt, QWidget* parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
>>>>>>> 47b45ba... fix some bugs
=======
    GLItemsWidget ( ViewerContext* context, Tools* tools, int* mainSceneId, const QGLFormat& fmt, QWidget* parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
>>>>>>> layoutdemo
    virtual void paintGL ();
    virtual void resizeGL ( int width, int height );
};

// class GLScrollItemsWidget : public GLScrollWidget
// {
//     Q_OBJECT

// public:
//     GLScrollItemsWidget ( MainWindow* context, GLMainView* widget );
// };


class CenterWidget : public QWidget
{
    Q_OBJECT

public:
<<<<<<< HEAD
<<<<<<< HEAD
    CenterWidget ( MainWindow* context, GLMainView* t, GLMainView* b, QWidget* p=0 );
=======
//    CenterWidget ( MainWindow* context, GLMainView* t, GLMainView* b, QWidget* p=0 );
    CenterWidget ( ViewerContext* context, GLScrollWidget* t, GLScrollWidget* b, QWidget* p=0 );
<<<<<<< HEAD
>>>>>>> 47b45ba... fix some bugs
=======
>>>>>>> 47b45ba... fix some bugs
=======
//    CenterWidget ( MainWindow* context, GLMainView* t, GLMainView* b, QWidget* p=0 );
    CenterWidget ( ViewerContext* context, GLScrollWidget* t, GLScrollWidget* b, QWidget* p=0 );
>>>>>>> layoutdemo
    GLScrollWidget *top;
    GLScrollWidget *bottom;
    ViewerContext* context;
};

#endif // _CENTER_WIDGET_H_
