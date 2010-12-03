#include "keyboardmanipulator.h"
#include "locatetool.h"
#include <Qt>
#include <QApplication>
#include <tinylog.h>
#include <exception>
using namespace std;

KeyboardManipulater::KeyboardManipulater ( SGVTools* tools ) : SGVTool(tools)
{
}

void KeyboardManipulater::keydown ( int key )
{
    try
    {
	LOG_INFO ( "KeyboardManipulater::keydown\n" );
	CameraTool* cameraTool = _tools->getTool<CameraTool> ( SGVTools::CAMERA_TOOL );
	if ( NULL == cameraTool )
	    return;

	switch ( key )
	{
	case Qt::Key_F:
	    cameraTool->locateAll ();
	    break;
	case Qt::Key_Left:
	    LOG_INFO ( "KeyboardManipulater::left\n" );
	    cameraTool->left ();
	    break;
	case Qt::Key_Right:
	    cameraTool->right ();
	    break;
	case Qt::Key_Up:
	    cameraTool->up ();
	    break;
	case Qt::Key_Down:
	    cameraTool->down ();
	    break;
	case Qt::Key_Plus:
	case Qt::Key_Equal:
	    if ( isCtrlPressed() )
		cameraTool->zoomin ();
	    break;
	case Qt::Key_Minus:
	    if ( isCtrlPressed() )
		cameraTool->zoomout ();
	    break;
	case Qt::Key_Q:
	case Qt::Key_Escape:
	{
	    QApplication::instance()->quit ();
	    break;
	}
	default:
	    break;
	}
	SGVTool::keydown ( key );
    }
    catch ( exception& ex )
    {
	LOG_INFO ( ex.what() );
    }
}

void KeyboardManipulater::keyup ( int key )
{
    SGVTool::keyup ( key );
}




