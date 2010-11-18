#include "keyboardmanipulator.h"

KeyboardManipulater::KeyboardManipulater ( SGVTools* tools ) : SGVTool(tools)
{
}

void KeyboardManipulater::keydown ( int key )
{
    CameraTool* cameraTool = _tools->getTool ( SGVTools::LOCATE_TOOL );
    if ( NULL == cameraTool )
	return;

    switch ( key )
    {
    case Qt::Key_F:
	cameraTool->locateAll ();
	break;
    case Qt::Key_Left:
	cameraTool->moveLeft ();
	break;
    case Qt::Key_Right:
	cameraTool->moveRight ();
	break;
    case Qt::Key_Up:
	cameraTool->moveUp ();
	break;
    case Qt::Key_Down:
	cameraTool->moveDown ();
	break;
    case Qt::Key_PageDown:
	cameraTool->zoomIn ();
	break;
    case Qt::Key_PageUp:
	cameraTool->zoomOut ();
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




