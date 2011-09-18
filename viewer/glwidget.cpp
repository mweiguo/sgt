#include "glwidget.h"
#include <sgr_render2d.h>
#include "document.h"

GLWidget::GLWidget ( const QGLFormat& fmt, QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags f ) :
    QGLWidget ( fmt, parent, shareWidget, f )
{
}

void GLWidget::initializeGL ()
{
    r2d_init ();
}

void GLWidget::paintGL ()
{
    r2d_update_scenes ( &document->sceneid, 1 );
    swapBuffers ();
}

void GLWidget::resizeGL ( int width, int height )
{
    r2d_resize ( width, height );
}

