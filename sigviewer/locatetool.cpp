#include "locatetool.h"
#include "sgv_tools.h"
#include <sgr_interface.h>
#include "view.h"

LocateTool::LocateTool ( SGVTools* tools ) : SGVTool(tools)
{
    _currentScale = 1;
}

void LocateTool::locateAll ()
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

void LocateTool::locate_object ( int id )
{
}
