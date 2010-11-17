#include "sgv_tools.h"
#include "qviewport.h"
#include "sgr_interface.h"
#include "handtool.h"
#include "zoomtool.h"
#include "coordquery.h"
#include "locatetool.h"

// --------------------------------------------------
const int SGVTools::HAND_TOOL = 1;
const int SGVTools::ZOOM_TOOL = 2;
const int SGVTools::COORDQUERY_TOOL = 3;
const int SGVTools::LOCATE_TOOL = 4;


SGVTools& SGVTools::getInst()
{
    static SGVTools inst;
    return inst;
}

SGVTools::SGVTools ()
{
    _currentTool = NULL;
    _pview = NULL;
}

SGVTools::~SGVTools ()
{
    for ( std::map< int, SGVTool* >::iterator pp = _tools.begin(); pp!=_tools.end(); ++pp )
    {
	delete pp->second;
    }
}

void SGVTools::initialize ( View* pview )
{
    std::map< int, SGVTool* >::iterator pp, end=_tools.end();
    for ( pp=_tools.begin(); pp!=end; ++pp )
    {
	pp->second->initialize ( pview );
    }
    _pview = pview;
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

void SGVTools::addTool ( int idx )
{
    SGVTool* tool = NULL;
    switch ( idx )
    {
    case HAND_TOOL:
    {
	tool = new HandTool(this);
	break;
    }
    case ZOOM_TOOL:
	tool = new ZoomTool(this);
	break;
    case COORDQUERY_TOOL:
	tool = new CoordQueryTool(this);
	break;
    case LOCATE_TOOL:
	tool = new LocateTool(this);
	break;
    }

    if ( tool )
    {
	_tools[idx] = tool;
	_currentTool = tool;
	tool->initialize ( _pview );
    }
}

void SGVTools::removeTool ( int idx )
{
    std::map< int, SGVTool* >::iterator pp = _tools.find ( idx );
    if ( pp != _tools.end() )
    {
	delete pp->second;
	_tools.erase ( pp );
    }
}

// --------------------------------------------------

SGVTool::SGVTool ( SGVTools* tools )
{
    _pview = NULL;
    _tools = tools;
}

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

