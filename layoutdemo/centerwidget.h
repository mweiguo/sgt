#ifndef _CENTER_WIDGET_H_
#define _CENTER_WIDGET_H_

#include <QtOpenGL/QGLWidget>
#include <glwidget.h>
class Document;
class Tools;
class MainWindow;
class QScrollBar;
class GLScrollWidget;


class GLItemsWidget : public GLMainView
{
public:
    GLItemsWidget ( MainWindow* context, int* mainSceneId, const QGLFormat& fmt, QWidget* parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
    virtual void paintGL ();
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
    CenterWidget ( MainWindow* context, GLMainView* t, GLMainView* b, QWidget* p=0 );
    GLScrollWidget *top;
    GLScrollWidget *bottom;
    MainWindow* context;
};

#endif // _CENTER_WIDGET_H_
