#include "qglviewport.h"
#include <sgr_seedgen.h>
#include "sgr_interface.h"
#include <sgr_mat4.h>
#include <sgr_vec4.h>
#include "sgv_tools.h"
#include <tinyLog.h>

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

QGLViewport::~QGLViewport()
{
    node_delete ( _viewportid );
    node_delete ( _camid );
    node_delete ( _projid );
}

void QGLViewport::updateWindow ()
{
    viewport_update2 ( _viewportid );
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

SGR::vec2f QGLViewport::viewportToScene ( SGR::vec2f vpxy )
{
    float imv[16], ipr[16], ivp[16];
    get_camerainversematrix ( camid(), imv );
    get_projectioninversematrix ( projid(), ipr );
    get_viewportinversematrix ( vpid(), ivp );
    SGR::mat4f imvmat(imv), iprmat(ipr), ivpmat(ivp);
    SGR::mat4f mat = imvmat * iprmat * ivpmat;

    SGR::vec4f scenepos = mat * SGR::vec4f ( vpxy );
    return scenepos.xy();
}

void QGLViewport::mousePressEvent ( QMouseEvent * event )
{
    try
    {
	SGR::vec2f scenepos = viewportToScene ( SGR::vec2f(event->x(), event->y()) );
	SGVTool* ptool = _tools->currentTool ();
	if ( ptool )
	{
	    Qt::KeyboardModifiers ms = event->modifiers ();
	    ptool->setCtrlPressed ( ms & Qt::ControlModifier ? true : false );
	    ptool->setAltPressed ( ms & Qt::AltModifier ? true : false );
	    ptool->setShiftPressed ( ms & Qt::ShiftModifier ? true : false );
	    if ( event->buttons() & Qt::LeftButton )
		ptool->lbuttondown ( scenepos.x(), scenepos.y() );
	    else if ( event->buttons() & Qt::RightButton )
		ptool->rbuttondown ( scenepos.x(), scenepos.y() );
	    else if ( event->buttons() & Qt::MidButton )
		ptool->mbuttondown ( scenepos.x(), scenepos.y() );
	}
    }
    catch ( std::exception& ex )
    {
	LOG_INFO ( ex.what () );
    }    
}

void QGLViewport::mouseReleaseEvent ( QMouseEvent * event )
{
    try
    {
	SGR::vec2f scenepos = viewportToScene ( SGR::vec2f(event->x(), event->y()) );
	SGVTool* ptool = _tools->currentTool ();
	if ( ptool )
	{
	    Qt::KeyboardModifiers ms = event->modifiers ();
	    ptool->setCtrlPressed ( ms & Qt::ControlModifier ? true : false );
	    ptool->setAltPressed ( ms & Qt::AltModifier ? true : false );
	    ptool->setShiftPressed ( ms & Qt::ShiftModifier ? true : false );
	    if ( event->buttons() & Qt::LeftButton )
		ptool->lbuttonup ( scenepos.x(), scenepos.y() );
	    else if ( event->buttons() & Qt::RightButton )
		ptool->rbuttonup ( scenepos.x(), scenepos.y() );
	    else if ( event->buttons() & Qt::MidButton )
		ptool->mbuttonup ( scenepos.x(), scenepos.y() );
	}
    }
    catch ( std::exception& ex )
    {
	LOG_INFO ( ex.what () );
    }    
}

void QGLViewport::mouseMoveEvent ( QMouseEvent * event  )
{
    try
    {
	SGR::vec2f scenepos = viewportToScene ( SGR::vec2f(event->x(), event->y()) );
	SGVTool* ptool = _tools->currentTool ();
	if ( ptool )
	{
	    Qt::KeyboardModifiers ms = event->modifiers ();
	    ptool->setCtrlPressed ( ms & Qt::ControlModifier ? true : false );
	    ptool->setAltPressed ( ms & Qt::AltModifier ? true : false );
	    ptool->setShiftPressed ( ms & Qt::ShiftModifier ? true : false );

	    if ( event->buttons() & Qt::LeftButton )
		ptool->lbuttonmove ( scenepos.x(), scenepos.y() );
	    else if ( event->buttons() & Qt::RightButton )
		ptool->rbuttonmove ( scenepos.x(), scenepos.y() );
	    else if ( event->buttons() & Qt::MidButton )
		ptool->mbuttonmove ( scenepos.x(), scenepos.y() );
	    else if ( event->buttons() & Qt::NoModifier )
		ptool->mousemove ( scenepos.x(), scenepos.y() );
	}
    }
    catch ( std::exception& ex )
    {
	LOG_INFO ( ex.what () );
    }    
}

void QGLViewport::wheelEvent ( QWheelEvent * event )
{
    try
    {
	SGR::vec2f scenepos = viewportToScene ( SGR::vec2f(event->x(), event->y()) );
	SGVTool* ptool = _tools->currentTool ();
	if ( NULL == ptool )
	{
	    Qt::KeyboardModifiers ms = event->modifiers ();
	    ptool->setCtrlPressed ( ms & Qt::ControlModifier ? true : false );
	    ptool->setAltPressed ( ms & Qt::AltModifier ? true : false );
	    ptool->setShiftPressed ( ms & Qt::ShiftModifier ? true : false );
	    ptool->mousewheel ( scenepos.x(), scenepos.y(), event->delta() );
	}
    }
    catch ( std::exception& ex )
    {
	LOG_INFO ( ex.what () );
    }    
}

void QGLViewport::keyPressEvent ( QKeyEvent * event )
{
    try
    {
	SGVTool* ptool = _tools->currentTool ();
	if ( NULL == ptool )
	{
	    Qt::KeyboardModifiers ms = event->modifiers ();
	    ptool->setCtrlPressed ( ms & Qt::ControlModifier ? true : false );
	    ptool->setAltPressed ( ms & Qt::AltModifier ? true : false );
	    ptool->setShiftPressed ( ms & Qt::ShiftModifier ? true : false );
	    ptool->keydown ( event->key() );
	}
    }
    catch ( std::exception& ex )
    {
	LOG_INFO ( ex.what () );
    }    
}

void QGLViewport::keyReleaseEvent ( QKeyEvent * event )
{
    try
    {
	SGVTool* ptool = _tools->currentTool ();
	if ( NULL == ptool )
	{
	    Qt::KeyboardModifiers ms = event->modifiers ();
	    ptool->setCtrlPressed ( ms & Qt::ControlModifier ? true : false );
	    ptool->setAltPressed ( ms & Qt::AltModifier ? true : false );
	    ptool->setShiftPressed ( ms & Qt::ShiftModifier ? true : false );
	    ptool->keyup ( event->key() );
	}
    }
    catch ( std::exception& ex )
    {
	LOG_INFO ( ex.what () );
    }
}

