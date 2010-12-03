#include "qglviewport.h"
#include "sgr_interface.h"
#include <tinylog.h>

QGLViewport::QGLViewport( const char* title ) : View()
{
    // initialize tools
    _tools.initialize ( this );
    _tools.addTool ( SGVTools::CAMERA_TOOL );
    _tools.addTool ( SGVTools::KEYBOARDMANIPULATER_TOOL );
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
    LOG_INFO ( "QGLViewport::paintGL" );
    viewport_update2 ( vpid() );
}

void QGLViewport::resizeGL ( int width, int height )
{
    viewport_geometry ( vpid(), 0, 0, width, height );
    projection_ortho ( projid(), -1, 1, -1, 1, 0.1, 10000 );
}

void QGLViewport::keyPressEvent ( QKeyEvent * event )
{
    View::keyPressEvent ( event );
}

void QGLViewport::keyReleaseEvent ( QKeyEvent * event )
{
    View::keyPressEvent ( event );
}
