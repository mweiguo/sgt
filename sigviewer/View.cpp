#include "view.h"
#include <sgr_seedgen.h>
#include "sgr_interface.h"
#include <sgr_mat4.h>
#include <sgr_vec4.h>
#include "sgv_tools.h"
#include <tinyLog.h>

View::View ()
{
    // init window states
    _viewportid = SGR::SeedGenerator::getInst().maxseed();
    _camid = SGR::SeedGenerator::getInst().maxseed();
    _projid = SGR::SeedGenerator::getInst().maxseed();
    
    viewport_create ( _viewportid, "default viewport" );
    camera_create ( _camid, "default camera" );
    projection_create ( _projid );

    viewport_attachcamera ( _viewportid, _camid );
    viewport_attachprojection ( _viewportid, _projid );
}

View::~View ()
{
    node_delete ( _viewportid );
    node_delete ( _camid );
    node_delete ( _projid );
}

void View::updateWindow ()
{
}

SGR::mat4f View::getVPM () const
{
    SGR::mat4f mat4camera, mat4proj, mat4vp;
    get_cameramatrix ( _camid, &(mat4camera.m00()) );
    get_projectionmatrix ( _projid, &(mat4proj.m00()) );
    get_viewportmatrix ( _viewportid, &(mat4vp.m00()) );

    return mat4vp * mat4proj * mat4camera;
}

SGR::mat4f View::getIMPV () const
{
    SGR::mat4f imvmat, iprmat, ivpmat;
    get_camerainversematrix ( _camid, &(imvmat.m00()) );
    get_projectioninversematrix ( _projid, &(iprmat.m00()) );
    get_viewportinversematrix ( _viewportid, &(ivpmat.m00()) );
    return imvmat * iprmat * ivpmat;
}


SGR::vec2f View::viewportToScene ( SGR::vec2f vpxy ) const
{
    SGR::mat4f mat = getIMPV ();
    SGR::vec4f scenepos = mat * SGR::vec4f ( vpxy );
    return scenepos.xy();
}

void View::mousePressEvent ( QMouseEvent * event )
{
    try
    {
	SGR::vec2f scenepos = viewportToScene ( SGR::vec2f(event->x(), event->y()) );
	SGVTool* ptool = _tools.currentTool ();
	if ( NULL != ptool )
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

void View::mouseReleaseEvent ( QMouseEvent * event )
{
    try
    {
	SGR::vec2f scenepos = viewportToScene ( SGR::vec2f(event->x(), event->y()) );
	SGVTool* ptool = _tools.currentTool ();
	if ( NULL != ptool )
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

void View::mouseMoveEvent ( QMouseEvent * event  )
{
    try
    {
	SGR::vec2f scenepos = viewportToScene ( SGR::vec2f(event->x(), event->y()) );
	SGVTool* ptool = _tools.currentTool ();
	if ( NULL != ptool )
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

void View::wheelEvent ( QWheelEvent * event )
{
    try
    {
	SGR::vec2f scenepos = viewportToScene ( SGR::vec2f(event->x(), event->y()) );
	SGVTool* ptool = _tools.currentTool ();
	if ( NULL != ptool )
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

void View::keyPressEvent ( QKeyEvent * event )
{
    try
    {
	SGVTool* ptool = _tools.currentTool ();
	if ( NULL != ptool )
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

void View::keyReleaseEvent ( QKeyEvent * event )
{
    try
    {
	SGVTool* ptool = _tools.currentTool ();
	if ( NULL != ptool )
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

