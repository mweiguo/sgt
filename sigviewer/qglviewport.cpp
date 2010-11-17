#include "qglviewport.h"
#include "sgr_interface.h"

QGLViewport::QGLViewport( const char* title ) : View()
{
    // initialize tools
    _tools.initialize ( this );
    _tools.addTool ( SGVTools::LOCATE_TOOL );
}

QGLViewport::~QGLViewport()
{
}

void QGLViewport::updateWindow ()
{
    updateGL ();
}

int QGLViewport::getHeight()
{
    return width();
}

int QGLViewport::getWidth()
{
    return height();
}

void QGLViewport::paintGL ()
{
    viewport_update2 ( vpid() );
}

void QGLViewport::resizeGL ( int width, int height )
{
    viewport_geometry ( vpid(), 0, 0, width, height );
    projection_ortho ( projid(), -1, 1, -1, 1, 0.1, 10000 );
}

