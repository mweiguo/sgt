#ifndef _GL_WIDGET_H_
#define _GL_WIDGET_H_

#include <QtOpenGL/QGLWidget>

class Document;
class GLWidget : public QGLWidget
{
public:
    GLWidget ( const QGLFormat& fmt, QWidget* parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
    Document* document;
protected:
    virtual void initializeGL ();
    virtual void paintGL ();
    virtual void resizeGL ( int width, int height );
};

#endif // _GL_WIDGET_H_
