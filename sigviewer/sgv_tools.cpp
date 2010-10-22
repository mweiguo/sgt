#include "sgv_tools.h"
#include "qviewport.h"
#include "sgr_interface.h"

// --------------------------------------------------
const int SGVTools::HAND_TOOL = 1;
const int SGVTools::ZOOM_TOOL = 2;


SGVTools& SGVTools::getInst()
{
    static SGVTools inst;
    return inst;
}

SGVTools::SGVTools ()
{
    _tools[HAND_TOOL] = new HandTool ();
    _tools[ZOOM_TOOL] = new ZoomTool ();
    _currentTool = NULL;
}

void SGVTools::setEnviroments ( QViewport* view )
{
    std::map< int, SGVTool* >::iterator pp, end=_tools.end();
    for ( pp=_tools.begin(); pp!=end; ++pp )
    {
	pp->second->setView ( view );
    }
}

SGVTool* SGVTools::selectTool ( int tool )
{
    std::map< int, SGVTool* >::iterator pp = _tools.find ( tool );
    if ( pp == _tools.end() )
	_currentTool = NULL;
    else
	_currentTool = pp->second;
    return _currentTool;
}

SGVTool* SGVTools::currentTool ()
{
    return _currentTool;
}

// --------------------------------------------------

void SGVTool::lbuttondown ( float x, float y )
{
    _startX = x;
    _startY = y;
}

void SGVTool::lbuttonup ( float x, float y )
{
    _lastX = x;
    _lastY = y;
}

void SGVTool::lbuttonmove ( float x, float y )
{
    _lastX = x;
    _lastY = y;
}

void SGVTool::rbuttondown ( float x, float y )
{
    _startX = x;
    _startY = y;
}

void SGVTool::rbuttonup ( float x, float y )
{
    _lastX = x;
    _lastY = y;
}

void SGVTool::rbuttonmove ( float x, float y )
{
    _lastX = x;
    _lastY = y;
}

void SGVTool::mbuttondown ( float x, float y )
{
    _startX = x;
    _startY = y;
}

void SGVTool::mbuttonup ( float x, float y )
{
    _lastX = x;
    _lastY = y;
}

void SGVTool::mbuttonmove ( float x, float y )
{
    _lastX = x;
    _lastY = y;
}

void SGVTool::mousemove ( float x, float y )
{
}

void SGVTool::keydown ( int key )
{
}

void SGVTool::keyup ( int key )
{
}

void SGVTool::mousewheel ( float x, float y, float delta )
{
}

void SGVTool::setCtrlPressed ( bool isPressed )
{
    _isCtrlPressed = isPressed;
}

bool SGVTool::isCtrlPressed ()
{
    return _isCtrlPressed;
}

void SGVTool::setAltPressed ( bool isPressed )
{
    _isAltPressed = isPressed;
}

bool SGVTool::isAltPressed ()
{
    return _isAltPressed;
}

void SGVTool::setShiftPressed ( bool isPressed )
{
    _isShiftPressed = isPressed;
}

bool SGVTool::isShiftPressed ()
{
    return _isShiftPressed;
}

void SGVTool::setView ( QViewport* view )
{
    _view = view;
}

// --------------------------------------------------

void HandTool::lbuttondown ( float x, float y )
{
    SGVTool::lbuttondown ( x, y );
}

void HandTool::lbuttonup ( float x, float y )
{
    SGVTool::lbuttonup ( x, y );
    float dx = _startX - x;
    float dy = _startY - y;
    camera_translate ( _view->camid(), dx, dy, 0 );
    _view->update ();
}

void HandTool::lbuttonmove ( float x, float y )
{
    SGVTool::lbuttonmove ( x, y );
    float dx = _startX - x;
    float dy = _startY - y;
    camera_translate ( _view->camid(), dx, dy, 0 );
    _view->update ();
}

void HandTool::mbuttondown ( float x, float y )
{
    SGVTool::mbuttondown ( x, y );
}

void HandTool::mbuttonup ( float x, float y )
{
    SGVTool::mbuttonup ( x, y );
    float dx = _startX - x;
    float dy = _startY - y;
    camera_translate ( _view->camid(), dx, dy, 0 );
    _view->update ();
}

void HandTool::mbuttonmove ( float x, float y )
{
    SGVTool::mbuttonmove ( x, y );
    float dx = _startX - x;
    float dy = _startY - y;
    camera_translate ( _view->camid(), dx, dy, 0 );
    _view->update ();
}

// --------------------------------------------------
ZoomTool::ZoomTool ()
{
    _scale = 1;
}

void ZoomTool::lbuttondown ( float x, float y )
{
    SGVTool::lbuttondown ( x, y );
}

void ZoomTool::lbuttonup ( float x, float y )
{
    SGVTool::lbuttonup ( x, y );

    float mincoord[3], maxcoord[3];
    mincoord[0] = x < _startX ? x : _startX;
    mincoord[1] = y < _startY ? y : _startY;
    mincoord[2] = 0;
    maxcoord[0] = x >= _startX ? x : _startX;
    maxcoord[1] = y >= _startY ? y : _startY;
    maxcoord[2] = 0;

    _view->find_view ( mincoord, maxcoord, 0.8 );
    _view->update ();
}

void ZoomTool::lbuttonmove ( float x, float y )
{
    SGVTool::lbuttonmove ( x, y );
}

void ZoomTool::mousewheel ( float x, float y, float delta )
{
//     SGVTool::mousewheel ( x, y, delta );
//     if ( !isCtrlPressed() )
// 	return;


//     // get screen center in scene coordinates
//     // get cneter of sceneCenter and current pos
//     QSize wndsize = _view->size();
//     float vpcoord[3] = { wndsize.width()/2.f, wndsize.height()/2.f, 0 };
//     float scenecoord[3];
//     get_scenepos ( _view->vpid(), vpcoord, scenecoord );
//     scenecoord[0] = (scenecoord[0] + x) / 2.f;
//     scenecoord[1] = (scenecoord[1] + y) / 2.f;
//     scenecoord[2] = 0;

//     if ( delta > 0 )
//     {
// 	_scale *= 1.5;
//     }
//     else
//     {
// 	_scale /= 1.5;
//     }

//     camera_scale ( _view->camid(), _scale );
//     camera_translate ( _view->camid(), scenecoord[0], scenecoord[1], scenecoord[2] );
//     _view->update ();
}

