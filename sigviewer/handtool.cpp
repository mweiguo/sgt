#include "handtool.h"
#include <sgr_interface.h>
#include "view.h"

HandTool::HandTool ( SGVTools* tools ) : SGVTool ( tools )
{
}

void HandTool::lbuttondown ( float x, float y )
{
    SGVTool::lbuttondown ( x, y );
}

void HandTool::lbuttonup ( float x, float y )
{
    SGVTool::lbuttonup ( x, y );
    float dx = _startX - x;
    float dy = _startY - y;
    camera_translate ( _pview->camid(), dx, dy, 0 );
    _pview->updateWindow ();
}

void HandTool::lbuttonmove ( float x, float y )
{
    SGVTool::lbuttonmove ( x, y );
    float dx = _startX - x;
    float dy = _startY - y;
    camera_translate ( _pview->camid(), dx, dy, 0 );
    _pview->updateWindow ();
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
    camera_translate ( _pview->camid(), dx, dy, 0 );
    _pview->updateWindow ();
}

void HandTool::mbuttonmove ( float x, float y )
{
    SGVTool::mbuttonmove ( x, y );
    float dx = _startX - x;
    float dy = _startY - y;
    camera_translate ( _pview->camid(), dx, dy, 0 );
    _pview->updateWindow ();
}
