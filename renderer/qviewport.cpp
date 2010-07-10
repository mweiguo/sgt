#include "qviewport.h"
#include "viewport.h"
#include "interface.h"

QViewport::QViewport( const char* title )
{
    _x = _y = 0;
    setWindowTitle ( title );
}


void QViewport::add_viewport ( int id )
{
    Viewport* p = NodeMgr::getInst().getNodePtr<Viewport>(id);
    if ( p )
    {
        p->setID ( id );
        _viewports.push_back ( p );
    }
}
void QViewport::remove_viewport ( int id )
{
    Viewport* p = NodeMgr::getInst().getNodePtr<Viewport>(id);
    if ( p )
        _viewports.remove ( p );
}
void QViewport::resizeEvent ( QResizeEvent* event )
{
    list<Viewport*>::iterator pp, end=_viewports.end();
    for ( pp=_viewports.begin(); pp!=end; ++pp )
    {
        (*pp)->position ( _x, _y );
        (*pp)->size ( event->size().width(), event->size().height() );
    }
}
void QViewport::dirty_all ()
{
    list<Viewport*>::iterator pp, end=_viewports.end();
    for ( pp=_viewports.begin(); pp!=end; ++pp )
        (*pp)->dirty ( true );
}

void QViewport::paintEvent ( QPaintEvent* event )
{
    QPainter painter(this);    
    list<Viewport*>::iterator pp, end=_viewports.end();
    for ( pp=_viewports.begin(); pp!=end; ++pp )
    {
        RenderOption opt;
        opt.painter = &painter;
        RenderFlow renderflow ( *(*pp), opt );
        (*pp)->update();
    }
}

void QViewport::mouseMoveEvent ( QMouseEvent * event  )
{
    float viewportCoord[3], sceneCoord[3];
    viewportCoord[0] = event->x();
    viewportCoord[1] = event->y();
    viewportCoord[2] = 0;

    get_scenepos ( 2, viewportCoord, sceneCoord );
    qDebug ( "x=%f, y=%f, z=%f", sceneCoord[0], sceneCoord[1], sceneCoord[2] );

    int data[256];
    Viewport* vp = _viewports.front();
    int size = pick ( sceneCoord[0], sceneCoord[1], sceneCoord[2], vp->cameraid(), data );
    for ( int i=0; i<size; i++ )
    {
        qDebug ("seleced id = %d", data[i]);
    }
}
