#include "qviewport.h"
#include "sgr_interface.h"
#include "sgr_seedgen.h"
#include "sgv_tools.h"
#include <tinylog.h>
#include <QApplication>
#include <QMessageBox>
#include <sgr_mat4.h>


QViewport::QViewport( const char* title, QWidget* parent ) : View(), QWidget(parent)
{
    try 
    {
	// init window states
	setWindowTitle ( title );
	setFocusPolicy ( Qt::StrongFocus );
	if ( !hasFocus() )
	    setFocus(Qt::ActiveWindowFocusReason);

	// init tools
	_tools.initialize ( this );
	_tools.addTool ( SGVTools::CAMERA_TOOL );
	_tools.addTool ( SGVTools::KEYBOARDMANIPULATER_TOOL );

	LOG_INFO ("sigviewer initialize ok.\n");
    }
    catch ( std::exception& ex )
    {
	LOG_ERROR (ex.what());
    }
}

QViewport::~QViewport()
{
}

void QViewport::updateWindow ()
{
    update ();
}

int QViewport::getHeight()
{
    return height();
}

int QViewport::getWidth()
{
    return width();
}

void QViewport::resizeEvent ( QResizeEvent* event )
{
    try 
    {
	float w = event->size().width();
	float h = event->size().height();
	if ( w > h )
	    viewport_geometry ( vpid(), w/2, h/2, h, -h );
	else
	    viewport_geometry ( vpid(), w/2, h/2, w, -w );
    }
    catch ( std::exception& ex )
    {
	LOG_ERROR (ex.what());
    }
}

void QViewport::paintEvent ( QPaintEvent* event )
{
    try 
    {
	QPainter painter(this);    
	viewport_update ( vpid(), painter );
    }
    catch ( std::exception& ex )
    {
	LOG_ERROR (ex.what());
    }

}

void QViewport::mousePressEvent ( QMouseEvent * event )
{
    View::mousePressEvent ( event );
    QWidget::mousePressEvent ( event );
}

void QViewport::mouseReleaseEvent ( QMouseEvent * event )
{
    View::mouseReleaseEvent ( event );
    QWidget::mouseReleaseEvent ( event );
}

void QViewport::mouseMoveEvent ( QMouseEvent * event  )
{
    View::mouseMoveEvent ( event );
    QWidget::mouseMoveEvent ( event );
}

void QViewport::wheelEvent ( QWheelEvent * event )
{
    View::wheelEvent ( event );
    QWidget::wheelEvent ( event );
}

void QViewport::keyPressEvent ( QKeyEvent * event )
{
    View::keyPressEvent ( event );
    QWidget::keyPressEvent ( event );
}

void QViewport::keyReleaseEvent ( QKeyEvent * event )
{
    View::keyReleaseEvent ( event );
    QWidget::keyReleaseEvent ( event );
}
