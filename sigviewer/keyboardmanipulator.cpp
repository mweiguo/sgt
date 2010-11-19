#include "keyboardmanipulator.h"
#include "locatetool.h"
#include <Qt>
#include <QApplication>

KeyboardManipulater::KeyboardManipulater ( SGVTools* tools ) : SGVTool(tools)
{
}

void KeyboardManipulater::keydown ( int key )
{
    CameraTool* cameraTool = _tools->getTool<CameraTool> ( SGVTools::CAMERA_TOOL );
    if ( NULL == cameraTool )
	return;

    switch ( key )
    {
    case Qt::Key_F:
	cameraTool->locateAll ();
	break;
    case Qt::Key_Left:
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
    case Qt::Key_PageDown:
	cameraTool->zoomin ();
	break;
    case Qt::Key_PageUp:
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
}

void KeyboardManipulater::keyup ( int key )
{
}




