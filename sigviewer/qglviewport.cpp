#include "qglviewport.h"
#include <sgr_seedgen.h>
#include "sgr_interface.h"

QGLViewport& QGLViewport::getInst()
{
    static QGLViewport inst("viewport");
    return inst;
}

float QGLViewport::full_view ()
{
    return 1;
}

float QGLViewport::find_view ( const SGR::vec3f& minvec, const SGR::vec3f& maxvec, float percentOfView )
{
    return 1;
}

void QGLViewport::paintGL ()
{
    viewport_update2 ( _viewportid );
}

void QGLViewport::resizeGL ( int width, int height )
{
    viewport_geometry ( _viewportid, 0, 0, width, height );
    projection_ortho ( _projid, -1, 1, -1, 1, 0.1, 3 );
}


QGLViewport::QGLViewport( const char* title )
{
    _viewportid = SGR::SeedGenerator::getInst().maxseed();
    _camid = SGR::SeedGenerator::getInst().maxseed();
    _projid = SGR::SeedGenerator::getInst().maxseed();
    
    viewport_create ( _viewportid, "default viewport" );
    camera_create ( _camid, "default camera" );
    projection_create ( _projid );

    viewport_attachcamera ( _viewportid, _camid );
    viewport_attachprojection ( _viewportid, _projid );
}

void QGLViewport::mousePressEvent ( QMouseEvent * event )
{
}

void QGLViewport::mouseReleaseEvent ( QMouseEvent * event )
{
}

void QGLViewport::mouseMoveEvent ( QMouseEvent * event  )
{
}

void QGLViewport::wheelEvent ( QWheelEvent * event )
{
}

void QGLViewport::keyPressEvent ( QKeyEvent * event )
{
}

void QGLViewport::keyReleaseEvent ( QKeyEvent * event )
{
}

