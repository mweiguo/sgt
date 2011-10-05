#include "glwidget.h"
#include <sgr_render2d.h>
#include "document.h"
#include "tools.h"
#include <QMouseEvent>
#include <QKeyEvent>

#include <iostream>
using namespace std;

GLWidget::GLWidget ( MainWindow* context, const QGLFormat& fmt, QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags f ) :
    QGLWidget ( fmt, parent, shareWidget, f )
{
    tools = new Tools ( context );
    tools->setTools ( (Tools::ToolType)(Tools::NONE_TOOL | Tools::HAND_TOOL) );
}

void GLWidget::initializeGL ()
{
    r2d_init ();
}

void GLWidget::paintGL ()
{
    r2d_update_scenes ( &document->sceneid, 1 );
    swapBuffers ();
}

void GLWidget::resizeGL ( int width, int height )
{
//    cout << "width = " << width << ", height = " << height << endl;
    r2d_resize ( width, height );
}

int fromQtModifiers ( Qt::KeyboardModifiers qm )
{
    int modifiers = 0;
    if ( qm & Qt::NoModifier )
	modifiers &= Tool::KB_NoModifier;
    if ( qm & Qt::ShiftModifier )
	modifiers &= Tool::KB_ShiftModifier;
    if ( qm & Qt::ControlModifier )
	modifiers &= Tool::KB_ControlModifier;
    if ( qm & Qt::AltModifier )
	modifiers &= Tool::KB_AltModifier;
    if ( qm & Qt::MetaModifier )
	modifiers &= Tool::KB_MetaModifier;
    if ( qm & Qt::KeypadModifier )
	modifiers &= Tool::KB_KeypadModifier;
    if ( qm & Qt::GroupSwitchModifier )
	modifiers &= Tool::KB_GroupSwitchModifier;
    return modifiers;
}

void GLWidget::keyPressEvent ( QKeyEvent * event )
{
    try
    {
	if ( tools->currentTool )
	{
	    int modifiers = fromQtModifiers ( event->modifiers() );
	    tools->currentTool->OnKeyPress ( event->key(), modifiers );
	}
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

//================================================================================

void GLWidget::keyReleaseEvent ( QKeyEvent * event )
{
    try
    {
	if ( tools->currentTool )
	{
	    int modifiers = fromQtModifiers ( event->modifiers() );
	    tools->currentTool->OnKeyRelease ( event->key(), modifiers );
	}
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

//================================================================================

void GLWidget::mouseMoveEvent ( QMouseEvent * event )
{
    try
    {
	if ( tools->currentTool )
	{
	    if ( (Qt::LeftButton & event->buttons()) != 0 )
		tools->currentTool->OnLMouseMove ( event->x(), event->y() );
	    else if ( (Qt::MidButton & event->buttons()) != 0 )
		tools->currentTool->OnMMouseMove ( event->x(), event->y() );
	} else {
	    if ( Qt::MidButton & event->buttons() )
		tools->currentTool->OnMMouseMove ( event->x(), event->y() );
	}
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

//================================================================================

void GLWidget::mousePressEvent ( QMouseEvent * event )
{
    try
    {
	if ( tools->currentTool )
	{
	    if ( Qt::LeftButton & event->buttons() ) {
		tools->currentTool->OnLButtonDown ( event->x(), event->y() );
	    } else if ( Qt::MidButton & event->buttons() ) {
		_oldTool = tools->selectTool ( Tools::HAND_TOOL );
		tools->currentTool->OnMButtonDown ( event->x(), event->y() );
	    }
	} else {
	    if ( Qt::MidButton & event->buttons() ) {
		_oldTool = tools->selectTool ( Tools::HAND_TOOL );
		tools->currentTool->OnMButtonDown ( event->x(), event->y() );
	    }
	}
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

//================================================================================

void GLWidget::mouseReleaseEvent ( QMouseEvent * event )
{
    try
    {
	if ( tools->currentTool )
	{
	    if ( Qt::LeftButton == event->button() ) {
		tools->currentTool->OnLButtonUp ( event->x(), event->y() );
	    } else if ( Qt::MidButton == event->button() ) {
		tools->currentTool->OnMButtonUp ( event->x(), event->y() );
		tools->selectTool ( _oldTool );
	    }
	} else {
	    if ( Qt::MidButton == event->button() ) {
		tools->currentTool->OnMButtonUp ( event->x(), event->y() );
		tools->selectTool ( _oldTool );
	    }
	}
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

