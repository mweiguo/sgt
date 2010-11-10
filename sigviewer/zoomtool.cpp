#include "zoomtool.h"
#include "view.h"

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

//    _pview->find_view ( mincoord, maxcoord, 0.8 );
    _pview->updateWindow ();
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
