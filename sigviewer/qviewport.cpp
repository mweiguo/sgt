#include "qviewport.h"
#include "sgr_interface.h"
#include "sgr_seedgen.h"
#include "sgv_tools.h"
#include <tinylog.h>
#include <QApplication>
#include <QMessageBox>
#include <sgr_mat4.h>

QViewport::QViewport( const char* title, QWidget* parent )
{
    try 
    {
	// init window states
	setWindowTitle ( title );
	setFocusPolicy ( Qt::StrongFocus );
	if ( !hasFocus() )
	    setFocus(Qt::ActiveWindowFocusReason);

	// init sigrenderer lib
	_viewport = SGR::SeedGenerator::getInst().maxseed ();
	viewport_create ( _viewport, title );
	_camid = SGR::SeedGenerator::getInst().maxseed ();
	camera_create ( _camid, title );
	_projid = SGR::SeedGenerator::getInst().maxseed ();
	projection_create ( _projid );

	viewport_attachcamera ( _viewport, _camid );
	viewport_attachprojection ( _viewport, _projid );
	LOG_INFO ("sigviewer initialize ok.\n");

	// init tools
	SGVTools::getInst().initialize ( this );	

	// init data member
	_scale = 1.0f;
	_cameraPos[0] = 0;
	_cameraPos[1] = 0;
	_cameraPos[2] = 0;

    }
    catch ( std::exception& ex )
    {
	LOG_ERROR (ex.what());
    }
}

float QViewport::full_view ()
{
    try 
    {
	float minvec[3], maxvec[3];
	get_bbox ( 0, minvec, maxvec );
	_scale = find_view ( SGR::vec3f(minvec), SGR::vec3f(maxvec), 0.6 );
	update ();
	return _scale;
    }
    catch ( std::exception& ex )
    {
	LOG_ERROR (ex.what());
    }
}

float QViewport::find_view ( const SGR::vec3f& minvec, const SGR::vec3f& maxvec, float percentOfView )
{
    try 
    {
	// translate
	SGR::vec3f center = ( minvec + maxvec ) / 2.0f;
	// scale
	float maxdimension = ( maxvec - minvec ).max_element ();
	QSize wndsize = size();
	if ( wndsize.width() > wndsize.height() )
	{
	    _scale = percentOfView / ( maxvec - minvec ).y();
// 	    _scale = (wndsize.width() * percentOfView) / ( maxdimension );
	}
	else
	{
	    _scale = percentOfView / ( maxvec - minvec ).x();
// 	    _scale = (wndsize.height() * percentOfView) / ( maxdimension );
	}
	camera_reset ( _camid );
	camera_translate ( _camid, center.x(), center.y(), center.z() );
	camera_scale ( _camid, _scale );
	_cameraPos[0] = center.x();
	_cameraPos[1] = center.y();
	_cameraPos[2] = center.z();
	return _scale;
    }
    catch ( std::exception& ex )
    {
	LOG_ERROR (ex.what());
    }
}

void QViewport::left ()
{
    try
    {
	CoordQueryTool* ptool = SGVTools::getInst().getTool<CoordQueryTool> (SGVTools::COORDQUERY_TOOL);
 	SGR::vec2f p1 = ptool->viewportToScene ( SGR::vec2f(0,0) );
 	SGR::vec2f p2 = ptool->viewportToScene ( SGR::vec2f(size().width(), size().height()) );
	SGR::vec2f scenesize = p2 - p1;
	// calculate move step
	float delta = fabs(scenesize.x()) / 20.f;
	_cameraPos[0] -= delta;
	// camera translate
	camera_translate ( _camid, _cameraPos[0], _cameraPos[1], _cameraPos[2] );
	update ();
    }
    catch ( std::exception& ex )
    {
	LOG_ERROR (ex.what());
    }
}

void QViewport::right ()
{
    try
    {
	CoordQueryTool* ptool = SGVTools::getInst().getTool<CoordQueryTool> (SGVTools::COORDQUERY_TOOL);
 	SGR::vec2f p1 = ptool->viewportToScene ( SGR::vec2f(0,0) );
 	SGR::vec2f p2 = ptool->viewportToScene ( SGR::vec2f(size().width(), size().height()) );
	SGR::vec2f scenesize = p2 - p1;
	// calculate move step
	float delta = fabs(scenesize.x()) / 20.f;
	_cameraPos[0] += delta;
	// camera translate
	camera_translate ( _camid, _cameraPos[0], _cameraPos[1], _cameraPos[2] );
	update ();
    }
    catch ( std::exception& ex )
    {
	LOG_ERROR (ex.what());
    }
}

void QViewport::up ()
{
    try
    {
	CoordQueryTool* ptool = SGVTools::getInst().getTool<CoordQueryTool> (SGVTools::COORDQUERY_TOOL);
 	SGR::vec2f p1 = ptool->viewportToScene ( SGR::vec2f(0,0) );
 	SGR::vec2f p2 = ptool->viewportToScene ( SGR::vec2f(size().width(), size().height()) );
	SGR::vec2f scenesize = p2 - p1;
	// calculate move step
	float delta = fabs(scenesize.x()) / 20.f;
	_cameraPos[1] += delta;
	// camera translate
	camera_translate ( _camid, _cameraPos[0], _cameraPos[1], _cameraPos[2] );
	update ();
    }
    catch ( std::exception& ex )
    {
	LOG_ERROR (ex.what());
    }
}

void QViewport::down ()
{
    try
    {
	CoordQueryTool* ptool = SGVTools::getInst().getTool<CoordQueryTool> (SGVTools::COORDQUERY_TOOL);
 	SGR::vec2f p1 = ptool->viewportToScene ( SGR::vec2f(0,0) );
 	SGR::vec2f p2 = ptool->viewportToScene ( SGR::vec2f(size().width(), size().height()) );
	SGR::vec2f scenesize = p2 - p1;
	// calculate move step
	float delta = fabs(scenesize.x()) / 20.f;
	_cameraPos[1] -= delta;
	// camera translate
	camera_translate ( _camid, _cameraPos[0], _cameraPos[1], _cameraPos[2] );
	update ();
    }
    catch ( std::exception& ex )
    {
	LOG_ERROR (ex.what());
    }
}

void QViewport::zoomin ()
{
    try
    {
	_scale *= 1.2;
	camera_scale ( _camid, _scale );
	update ();
    }
    catch ( std::exception& ex )
    {
	LOG_ERROR (ex.what());
    }
}

void QViewport::zoomout ()
{
    try
    {
	_scale /= 1.2;
	camera_scale ( _camid, _scale );
	update ();
    }
    catch ( std::exception& ex )
    {
	LOG_ERROR (ex.what());
    }
}

void QViewport::resizeEvent ( QResizeEvent* event )
{
    try 
    {
	float w = event->size().width();
	float h = event->size().height();
	if ( w > h )
	    viewport_geometry ( _viewport, w/2, h/2, w, -w );
	else
	    viewport_geometry ( _viewport, w/2, h/2, h, -h );
    }
    catch ( std::exception& ex )
    {
	LOG_ERROR (ex.what());
    }
}

void QViewport::paintEvent ( QPaintEvent* event )
{
    try 
    {
	QPainter painter(this);    
	viewport_update ( _viewport, painter );
    }
    catch ( std::exception& ex )
    {
	LOG_ERROR (ex.what());
    }

}

void QViewport::mousePressEvent ( QMouseEvent * event )
{
//     float viewportCoord[3], sceneCoord[3];
//     viewportCoord[0] = event->x();
//     viewportCoord[1] = event->y();
//     viewportCoord[2] = 0;

//     get_scenepos ( 2, viewportCoord, sceneCoord );
//     SGVTool* ptool = SGVTools::getInst().currentTool ();
//     if ( NULL == ptool )
// 	return;

//     Qt::KeyboardModifiers ms = event->modifiers ();
//     ptool->setCtrlPressed ( ms & Qt::ControlModifier ? true : false );
//     ptool->setAltPressed ( ms & Qt::AltModifier ? true : false );
//     ptool->setShiftPressed ( ms & Qt::ShiftModifier ? true : false );

//     if ( event->buttons() & Qt::LeftButton )
// 	ptool->lbuttondown ( sceneCoord[0], sceneCoord[1] );
//     else if ( event->buttons() & Qt::RightButton )
// 	ptool->rbuttondown ( sceneCoord[0], sceneCoord[1] );
//     else if ( event->buttons() & Qt::MidButton )
// 	ptool->mbuttondown ( sceneCoord[0], sceneCoord[1] );

    QWidget::mousePressEvent ( event );
}

void QViewport::mouseReleaseEvent ( QMouseEvent * event )
{
//     float viewportCoord[3], sceneCoord[3];
//     viewportCoord[0] = event->x();
//     viewportCoord[1] = event->y();
//     viewportCoord[2] = 0;

//     get_scenepos ( 2, viewportCoord, sceneCoord );
//     SGVTool* ptool = SGVTools::getInst().currentTool ();
//     if ( NULL == ptool )
// 	return;

//     Qt::KeyboardModifiers ms = event->modifiers ();
//     ptool->setCtrlPressed ( ms & Qt::ControlModifier ? true : false );
//     ptool->setAltPressed ( ms & Qt::AltModifier ? true : false );
//     ptool->setShiftPressed ( ms & Qt::ShiftModifier ? true : false );

//     if ( event->buttons() & Qt::LeftButton )
// 	ptool->lbuttonup ( sceneCoord[0], sceneCoord[1] );
//     else if ( event->buttons() & Qt::RightButton )
// 	ptool->rbuttonup ( sceneCoord[0], sceneCoord[1] );
//     else if ( event->buttons() & Qt::MidButton )
// 	ptool->mbuttonup ( sceneCoord[0], sceneCoord[1] );

    QWidget::mouseReleaseEvent ( event );
}

void QViewport::mouseMoveEvent ( QMouseEvent * event  )
{
//     float viewportCoord[3], sceneCoord[3];
//     viewportCoord[0] = event->x();
//     viewportCoord[1] = event->y();
//     viewportCoord[2] = 0;

//     get_scenepos ( 2, viewportCoord, sceneCoord );
//     SGVTool* ptool = SGVTools::getInst().currentTool ();
//     if ( NULL == ptool )
// 	return;

//     Qt::KeyboardModifiers ms = event->modifiers ();
//     ptool->setCtrlPressed ( ms & Qt::ControlModifier ? true : false );
//     ptool->setAltPressed ( ms & Qt::AltModifier ? true : false );
//     ptool->setShiftPressed ( ms & Qt::ShiftModifier ? true : false );

//     if ( event->buttons() & Qt::LeftButton )
// 	ptool->lbuttonmove ( sceneCoord[0], sceneCoord[1] );
//     else if ( event->buttons() & Qt::RightButton )
// 	ptool->rbuttonmove ( sceneCoord[0], sceneCoord[1] );
//     else if ( event->buttons() & Qt::MidButton )
// 	ptool->mbuttonmove ( sceneCoord[0], sceneCoord[1] );
//     else if ( event->buttons() & Qt::NoModifier )
// 	ptool->mousemove ( sceneCoord[0], sceneCoord[1] );

    QWidget::mouseMoveEvent ( event );
}

void QViewport::wheelEvent ( QWheelEvent * event )
{
//     float viewportCoord[3], sceneCoord[3];
//     viewportCoord[0] = event->x();
//     viewportCoord[1] = event->y();
//     viewportCoord[2] = 0;

//     get_scenepos ( 2, viewportCoord, sceneCoord );
//     SGVTool* ptool = SGVTools::getInst().currentTool ();
//     if ( NULL == ptool )
// 	return;

//     Qt::KeyboardModifiers ms = event->modifiers ();
//     ptool->setCtrlPressed ( ms & Qt::ControlModifier ? true : false );
//     ptool->setAltPressed ( ms & Qt::AltModifier ? true : false );
//     ptool->setShiftPressed ( ms & Qt::ShiftModifier ? true : false );


//     ptool->mousewheel ( sceneCoord[0], sceneCoord[1], event->delta() );

    QWidget::wheelEvent ( event );
}

void QViewport::keyPressEvent ( QKeyEvent * event )
{
    switch ( event->key() )
    {
    case Qt::Key_F:
	full_view ();
	break;
    case Qt::Key_Left:
	left ();
	break;
    case Qt::Key_Right:
	right ();
	break;
    case Qt::Key_Up:
	up ();
	break;
    case Qt::Key_Down:
	down ();
	break;
    case Qt::Key_PageDown:
	zoomin ();
	break;
    case Qt::Key_PageUp:
	zoomout ();
	break;
    case Qt::Key_Q:
    case Qt::Key_Escape:
    {
	QApplication::instance()->quit ();
	break;
    }
    case Qt::Key_Z:
	SGVTools::getInst().selectTool ( SGVTools::ZOOM_TOOL );
	LOG_INFO ( "Zoom Tool is Selected\n" );
	break;
    default:
	SGVTool* ptool = SGVTools::getInst().currentTool ();
	if ( ptool )
	{
	    ptool->keydown ( event->key() );
	}
	else
	{
	    QWidget::keyPressEvent ( event );
	}
    }
}

void QViewport::keyReleaseEvent ( QKeyEvent * event )
{
    QWidget::keyReleaseEvent ( event );
}
