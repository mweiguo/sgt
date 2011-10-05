#ifndef _GL_WIDGET_H_
#define _GL_WIDGET_H_

#include <QtOpenGL/QGLWidget>

class Document;
class Tools;
class MainWindow;
class GLWidget : public QGLWidget
{
public:
    GLWidget ( MainWindow* context, const QGLFormat& fmt, QWidget* parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
    Document* document;
    Tools* tools;
protected:
    virtual void initializeGL ();
    virtual void paintGL ();
    virtual void resizeGL ( int width, int height );

    virtual void keyPressEvent ( QKeyEvent * event );
    virtual void keyReleaseEvent ( QKeyEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );

    int _oldTool;
};

#endif // _GL_WIDGET_H_
