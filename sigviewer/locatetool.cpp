#include "locatetool.h"
#include "sgv_tools.h"
#include <sgr_interface.h>
#include "view.h"
#include "coordquery.h"

CameraTool::CameraTool ( SGVTools* tools ) : SGVTool(tools)
{
    _currentScale = 1;
}

void CameraTool::locateAll ()
{

    float min[3], max[3];
    get_bbox ( 0/*_pview->scene()->sceneid()*/, min, max );
    SGR::vec3f dimension ( max[0]-min[0], max[1]-min[1], max[2]-min[2] );

    // calculate scale
    if ( dimension.x() > dimension.y() )
	_currentScale = _pview->getHeight() / dimension.y();
    else
	_currentScale = _pview->getWidth() / dimension.x();

    // calculate center
    _currentTranslate = SGR::vec3f ( (max[0]+min[0])/2.0, (max[1]+min[1])/2.0, (max[2]+min[2])/2.0 );
    
    // do change camera
    camera_translate ( _pview->camid(), _currentTranslate.x(), _currentTranslate.y(), _currentTranslate.z() );
    camera_scale ( _pview->camid(), _currentScale );

    _pview->updateWindow();
}

void CameraTool::locate_object ( int id )
{
}

float CameraTool::find_view ( const SGR::vec3f& minvec, const SGR::vec3f& maxvec, float percentOfView )
{
    // translate
    SGR::vec3f center = ( minvec + maxvec ) / 2.0f;

    // scale
    float s = calcScale ( minvec, maxvec );
    if ( s != 0 )
    {
	_currentScale = percentOfView / s;

	camera_reset ( _pview->camid() );
	camera_translate ( _pview->camid(), center.x(), center.y(), center.z() );
	camera_scale ( _pview->camid(), _currentScale );
	_currentTranslate.xyz ( center.x(), center.y(), center.z() );
    }
    return _currentScale;
}

void CameraTool::left ()
{
    CoordQueryTool* ptool = _tools->getTool<CoordQueryTool> (SGVTools::COORDQUERY_TOOL);
    SGR::vec2f p1 = ptool->viewportToScene ( SGR::vec2f(0,0) );
    SGR::vec2f p2 = ptool->viewportToScene ( SGR::vec2f( _pview->getWidth(), _pview->getHeight() ) );
    SGR::vec2f scenesize = p2 - p1;
    // calculate move step
    float delta = fabs(scenesize.x()) / 20.f;
    if ( _isInConstraintMode && ((p1.x()-delta) < _minTranslate.x()) )
	_currentTranslate.x ( _currentTranslate.x() - (p1.x()-_minTranslate.x()) );
    else
	_currentTranslate.x ( _currentTranslate.x() - delta );
    // camera translate
    camera_translate ( _pview->camid(), _currentTranslate.x(), _currentTranslate.y(), _currentTranslate.z() );
    _pview->updateWindow ();
}

void CameraTool::right ()
{
    CoordQueryTool* ptool = _tools->getTool<CoordQueryTool> (SGVTools::COORDQUERY_TOOL);
    SGR::vec2f p1 = ptool->viewportToScene ( SGR::vec2f(0,0) );
    SGR::vec2f p2 = ptool->viewportToScene ( SGR::vec2f( _pview->getWidth(), _pview->getHeight() ) );
    SGR::vec2f scenesize = p2 - p1;
    // calculate move step
    float delta = fabs(scenesize.x()) / 20.f;

    if ( _isInConstraintMode && ( (p2.x()+delta)>_maxTranslate.x() ) )
	_currentTranslate.x ( _currentTranslate.x() + (_maxTranslate.x()-p2.x()) );
    else
	_currentTranslate.x ( _currentTranslate.x() + delta );
    
    // camera translate
    camera_translate ( _pview->camid(), _currentTranslate.x(), _currentTranslate.y(), _currentTranslate.z() );
    _pview->updateWindow ();
}

void CameraTool::up ()
{
    CoordQueryTool* ptool = _tools->getTool<CoordQueryTool> (SGVTools::COORDQUERY_TOOL);
    SGR::vec2f p1 = ptool->viewportToScene ( SGR::vec2f(0,0) );
    SGR::vec2f p2 = ptool->viewportToScene ( SGR::vec2f( _pview->getWidth(), _pview->getHeight() ) );
    SGR::vec2f scenesize = p2 - p1;
    // calculate move step
    float delta = fabs(scenesize.x()) / 20.f;
    if ( _isInConstraintMode && ((p1.y()+delta) > _maxTranslate.y()) )
	_currentTranslate[1] += (_maxTranslate.y()-p1.y());
    else
	_currentTranslate.y ( _currentTranslate.y() + delta );
    // camera translate
    camera_translate ( _pview->camid(), _currentTranslate.x(), _currentTranslate.y(), _currentTranslate.z() );
    _pview->updateWindow ();
}

void CameraTool::down ()
{
    CoordQueryTool* ptool = _tools->getTool<CoordQueryTool> (SGVTools::COORDQUERY_TOOL);
    SGR::vec2f p1 = ptool->viewportToScene ( SGR::vec2f(0,0) );
    SGR::vec2f p2 = ptool->viewportToScene ( SGR::vec2f( _pview->getWidth(), _pview->getHeight() ) );
    SGR::vec2f scenesize = p2 - p1;
    // calculate move step
    float delta = fabs(scenesize.x()) / 20.f;
    if ( _isInConstraintMode && ( (p2.y()-delta)<_minTranslate.y() ) )
	_currentTranslate[1] -= (p2.y()-_minTranslate.y());
    else
	_currentTranslate[1] -= delta;
    // camera translate
    _pview->updateWindow ();
}

void CameraTool::zoomin ()
{
    if ( _isInConstraintMode && (_currentScale >= _maxScale) )
	_currentScale = _maxScale;
    else
	_currentScale *= 1.2;

    camera_scale ( _pview->camid(), _currentScale );
    _pview->updateWindow ();
}

void CameraTool::zoomout ()
{
    if ( _isInConstraintMode && ( _currentScale <= _minScale ))
	_currentScale = _minScale;
    else
	_currentScale /= 1.2;

    camera_scale ( _pview->camid(), _currentScale );
    _pview->updateWindow ();
}

void CameraTool::setCameraConstraint ( int nodeid, float percentOfView )
{
    float minarr[3], maxarr[3];
    get_bbox ( nodeid, minarr, maxarr );
    SGR::vec3f minvec(minarr), maxvec(maxarr);
    SGR::vec3f sz = (maxvec - minvec) / percentOfView;
    SGR::vec3f center = ( minvec + maxvec ) / 2.0f;

    QSize wndsize ( _pview->getWidth(), _pview->getHeight() );
    if ( wndsize.width() > wndsize.height() )
	sz.x ( (wndsize.width() / wndsize.height()) * sz.y () );
    else
	sz.y ( (wndsize.height() / wndsize.width()) * sz.x () );

    _minTranslate = center - (sz / 2.0f);
    _maxTranslate = center + (sz / 2.0f);

    _minScale = calcScale ( _minTranslate, _maxTranslate );
    _maxScale = 1;
    _isInConstraintMode = true;
}

void CameraTool::setCameraConstraint ( SGR::vec3f minTranslate, SGR::vec3f maxTranslate, float minScale, float maxScale )
{
    _minTranslate = minTranslate;
    _maxTranslate = maxTranslate;
    _minScale     = minScale;
    _maxScale     = maxScale;
    _isInConstraintMode = true;
}

float CameraTool::calcScale ( const SGR::vec3f& minvec, const SGR::vec3f& maxvec )
{
    float scale;
    QSize wndsize ( _pview->getWidth(), _pview->getHeight() );
    if ( wndsize.width() > wndsize.height() )
	scale = 1.0f / ( maxvec - minvec ).x();
    else
	scale = 1.0f / ( maxvec - minvec ).y();
    return scale;
}
