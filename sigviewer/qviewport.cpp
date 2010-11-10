#include "qviewport.h"
#include "sgr_interface.h"
#include "sgr_seedgen.h"
#include "sgv_tools.h"
#include <tinylog.h>
#include <QApplication>
#include <QMessageBox>
#include <sgr_mat4.h>
#include "coordquery.h"


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
	_minTranslate = SGR::vec3f(-1e6, -1e6, -1e6);
	_maxTranslate = SGR::vec3f(1e6, 1e6, 1e6);
	_minScale     = 1e-6;
	_maxScale     = 1e6;

	_isInConstraintMode = false;
    }
    catch ( std::exception& ex )
    {
	LOG_ERROR (ex.what());
    }
}

QViewport::~QViewport()
{
}

void QViewport::updateWindow ()
{
    update ();
}

float QViewport::full_view ()
{
    SGR::vec3f center = ( _minTranslate + _maxTranslate ) / 2.0f;

    camera_reset ( _camid );
    camera_translate ( _camid, center.x(), center.y(), center.z() );
    camera_scale ( _camid, _minScale );

    _cameraPos[0] = center.x();
    _cameraPos[1] = center.y();
    _cameraPos[2] = center.z();
    _scale = _minScale;

    update ();
    return _scale;
}

float QViewport::find_view ( const SGR::vec3f& minvec, const SGR::vec3f& maxvec, float percentOfView )
{
    // translate
    SGR::vec3f center = ( minvec + maxvec ) / 2.0f;

    // scale
    float s = calcScale ( minvec, maxvec );
    if ( s != 0 )
    {
	_scale = percentOfView / s;

	camera_reset ( _camid );
	camera_translate ( _camid, center.x(), center.y(), center.z() );
	camera_scale ( _camid, _scale );
	_cameraPos[0] = center.x();
	_cameraPos[1] = center.y();
	_cameraPos[2] = center.z();
    }
    return _scale;
}

void QViewport::left ()
{
    CoordQueryTool* ptool = SGVTools::getInst().getTool<CoordQueryTool> (SGVTools::COORDQUERY_TOOL);
    SGR::vec2f p1 = ptool->viewportToScene ( SGR::vec2f(0,0) );
    SGR::vec2f p2 = ptool->viewportToScene ( SGR::vec2f(size().width(), size().height()) );
    SGR::vec2f scenesize = p2 - p1;
    // calculate move step
    float delta = fabs(scenesize.x()) / 20.f;
    LOG_INFO ( "QViewport::left  _isInConstraintMode = %d, camerapos.x=%f, limit=%f\n", _isInConstraintMode, _cameraPos[0],  _minTranslate.x() );
    if ( _isInConstraintMode && ((p1.x()-delta) < _minTranslate.x()) )
	_cameraPos[0] -= (p1.x()-_minTranslate.x());
    else
	_cameraPos[0] -= delta;
    LOG_INFO ( "\tcamerapos.x=%f\n", _cameraPos[0] );
    // camera translate
    camera_translate ( _camid, _cameraPos[0], _cameraPos[1], _cameraPos[2] );
    update ();
}

void QViewport::right ()
{
    CoordQueryTool* ptool = SGVTools::getInst().getTool<CoordQueryTool> (SGVTools::COORDQUERY_TOOL);
    SGR::vec2f p1 = ptool->viewportToScene ( SGR::vec2f(0,0) );
    SGR::vec2f p2 = ptool->viewportToScene ( SGR::vec2f(size().width(), size().height()) );
    SGR::vec2f scenesize = p2 - p1;
    // calculate move step
    float delta = fabs(scenesize.x()) / 20.f;

    if ( _isInConstraintMode && ( (p2.x()+delta)>_maxTranslate.x() ) )
	_cameraPos[0] += (_maxTranslate.x()-p2.x());
    else
	_cameraPos[0] += delta;
    
    // camera translate
    camera_translate ( _camid, _cameraPos[0], _cameraPos[1], _cameraPos[2] );
    update ();
}

void QViewport::up ()
{
    CoordQueryTool* ptool = SGVTools::getInst().getTool<CoordQueryTool> (SGVTools::COORDQUERY_TOOL);
    SGR::vec2f p1 = ptool->viewportToScene ( SGR::vec2f(0,0) );
    SGR::vec2f p2 = ptool->viewportToScene ( SGR::vec2f(size().width(), size().height()) );
    SGR::vec2f scenesize = p2 - p1;
    // calculate move step
    float delta = fabs(scenesize.x()) / 20.f;
    if ( _isInConstraintMode && ((p1.y()+delta) > _maxTranslate.y()) )
	_cameraPos[1] += (_maxTranslate.y()-p1.y());
    else
	_cameraPos[1] += delta;
    // camera translate
    camera_translate ( _camid, _cameraPos[0], _cameraPos[1], _cameraPos[2] );
    update ();
}

void QViewport::down ()
{
    CoordQueryTool* ptool = SGVTools::getInst().getTool<CoordQueryTool> (SGVTools::COORDQUERY_TOOL);
    SGR::vec2f p1 = ptool->viewportToScene ( SGR::vec2f(0,0) );
    SGR::vec2f p2 = ptool->viewportToScene ( SGR::vec2f(size().width(), size().height()) );
    SGR::vec2f scenesize = p2 - p1;
    // calculate move step
    float delta = fabs(scenesize.x()) / 20.f;
    if ( _isInConstraintMode && ( (p2.y()-delta)<_minTranslate.y() ) )
	_cameraPos[1] -= (p2.y()-_minTranslate.y());
    else
	_cameraPos[1] -= delta;
    // camera translate
    camera_translate ( _camid, _cameraPos[0], _cameraPos[1], _cameraPos[2] );
    update ();
}

void QViewport::zoomin ()
{
    if ( _isInConstraintMode && (_scale >= _maxScale) )
	_scale = _maxScale;
    else
	_scale *= 1.2;

    camera_scale ( _camid, _scale );
    update ();
}

void QViewport::zoomout ()
{
    if ( _isInConstraintMode && ( _scale <= _minScale ))
	_scale = _minScale;
    else
	_scale /= 1.2;

    camera_scale ( _camid, _scale );
    update ();
}

void QViewport::setCameraConstraint ( int nodeid, float percentOfView )
{
    float minarr[3], maxarr[3];
    get_bbox ( nodeid, minarr, maxarr );
    SGR::vec3f minvec(minarr), maxvec(maxarr);
    SGR::vec3f sz = (maxvec - minvec) / percentOfView;
    SGR::vec3f center = ( minvec + maxvec ) / 2.0f;
//     LOG_INFO ( "before : min.x=%f, min.y=%f, max.x=%f, max.y=%f\n", 
// 	       center.x()-sz.x()/2.0f, center.y()-sz.y()/2.0f,
// 	       center.x()+sz.x()/2.0f, center.y()+sz.y()/2.0f );
    QSize wndsize = size();
    if ( wndsize.width() > wndsize.height() )
	sz.x ( (wndsize.width() / wndsize.height()) * sz.y () );
    else
	sz.y ( (wndsize.height() / wndsize.width()) * sz.x () );
//     LOG_INFO ( "       : min.x=%f, min.y=%f, max.x=%f, max.y=%f\n", 
// 	       center.x()-sz.x()/2.0f, center.y()-sz.y()/2.0f,
// 	       center.x()+sz.x()/2.0f, center.y()+sz.y()/2.0f );

    _minTranslate = center - (sz / 2.0f);
    _maxTranslate = center + (sz / 2.0f);
//     LOG_INFO ( "       : min.x=%f, min.y=%f, max.x=%f, max.y=%f\n", 
// 	       _minTranslate.x(), _minTranslate.y(),
// 	       _maxTranslate.x(), _maxTranslate.y() );

    _minScale = calcScale ( _minTranslate, _maxTranslate );
    _maxScale = 1;
    _isInConstraintMode = true;
}

void QViewport::setCameraConstraint ( SGR::vec3f minTranslate, SGR::vec3f maxTranslate, float minScale, float maxScale )
{
    _minTranslate = minTranslate;
    _maxTranslate = maxTranslate;
    _minScale     = minScale;
    _maxScale     = maxScale;
    _isInConstraintMode = true;
}

float QViewport::calcScale ( const SGR::vec3f& minvec, const SGR::vec3f& maxvec )
{
    float scale;
    QSize wndsize = size();
    if ( wndsize.width() > wndsize.height() )
	scale = 1.0f / ( maxvec - minvec ).x();
    else
	scale = 1.0f / ( maxvec - minvec ).y();
    return scale;
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

	setCameraConstraint(0, 0.8);
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
    try
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
    catch ( std::exception& ex )
    {
	LOG_INFO ( ex.what () );
    }
}

void QViewport::keyReleaseEvent ( QKeyEvent * event )
{
    try
    {
	QWidget::keyReleaseEvent ( event );
    }
    catch ( std::exception& ex )
    {
	LOG_INFO ( ex.what () );
    }
}
