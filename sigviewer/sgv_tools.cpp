#include "sgv_tools.h"
#include "qviewport.h"
#include "sgr_interface.h"

// --------------------------------------------------
const int SGVTools::HAND_TOOL = 1;
const int SGVTools::ZOOM_TOOL = 2;
const int SGVTools::COORDQUERY_TOOL = 3;


SGVTools& SGVTools::getInst()
{
    static SGVTools inst;
    return inst;
}

SGVTools::SGVTools ()
{
//     _tools[HAND_TOOL] = new HandTool ();
//     _tools[ZOOM_TOOL] = new ZoomTool ();
//     _tools[COORDQUERY_TOOL] = new CoordQueryTool ();
    _currentTool = NULL;
}

void SGVTools::initialize ( View* pview )
{
    std::map< int, SGVTool* >::iterator pp, end=_tools.end();
    for ( pp=_tools.begin(); pp!=end; ++pp )
    {
	pp->second->initialize ( pview );
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

void SGVTool::initialize ( View* pview )
{
    _pview = pview;
}

