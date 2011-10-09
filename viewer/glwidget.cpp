#include <QMouseEvent>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QStatusBar>

#include <iostream>
#include <sstream>

#include "glwidget.h"
#include "document.h"
#include "tools.h"
#include "mainwindow.h"

#include <sgr_render2d.h>

using namespace std;

GLWidget::GLWidget ( MainWindow* cont, const QGLFormat& fmt, QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags f ) :
    QGLWidget ( fmt, parent, shareWidget, f )
{
    context = cont;
    setMouseTracking ( true );
    document = context->doc;
    tools = NULL;
    scale = 1;
    translate[0] = translate[1] = 0;
}

void GLWidget::zoomin()
{
    scale *= 1.2;
}

void GLWidget::zoomout()
{
    scale *= 1 / 1.2;
}

void GLWidget::lefttranslate()
{
    float delta[2];
    r2d_get_viewport_rect ( delta );
    translate[0] += delta[2]/20.0f;
}

void GLWidget::righttranslate()
{
    float delta[4];
    r2d_get_viewport_rect ( delta );
    translate[0] += -delta[2]/20.0f;
}

void GLWidget::uptranslate()
{
    float delta[4];
    r2d_get_viewport_rect ( delta );
    translate[1] += -delta[3]/20.0f;
}

void GLWidget::downtranslate()
{
    float delta[4];
    r2d_get_viewport_rect ( delta );
    translate[1] += delta[3]/20.0f;
}

void GLWidget::homeposition()
{
    float ratio = 1.0 * size().height() / size().width();
    // get minmax
    float minmax[4];
    r2d_get_scene_minmax ( context->doc->sceneid, minmax, minmax+2 );
    float center[2] = { (minmax[2] + minmax[0]) / 2, (minmax[3] + minmax[1]) / 2 };
    float size[2]   = { (minmax[2] - minmax[0]) / 2, (minmax[3] - minmax[1]) / (2*ratio) };
    scale = size[0] > size[1] ? 1.0 / size[0] : 1.0 / size[1];
    translate[0] = -center[0];
    translate[1] = -center[1];
}

void GLWidget::initializeGL ()
{
    r2d_init ();
}

void GLWidget::setTransform()
{
    r2d_loadidentity ();
    r2d_scale ( scale );
    r2d_translate ( translate[0], translate[1] );
}

void GLWidget::resizeGL ( int width, int height )
{
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

GLMainView::GLMainView ( MainWindow* context, const QGLFormat& fmt, QWidget* parent, const QGLWidget * shareWidget, Qt::WindowFlags f )
    : GLWidget ( context, fmt, parent, shareWidget, f )
{
    tools = new Tools ( context );
    tools->setTools ( Tools::NONE_TOOL | Tools::HAND_TOOL | Tools::ZOOM_TOOL );
}

void GLMainView::paintGL ()
{
    setTransform();
    int ids[] = {document->sceneid, document->miscsceneid };
    r2d_update_scenes ( ids, 2 );
    swapBuffers ();
}

//================================================================================

GLBirdView::GLBirdView ( MainWindow* context, const QGLFormat& fmt, QWidget* parent, const QGLWidget * shareWidget, Qt::WindowFlags f )
    : GLWidget ( context, fmt, parent, shareWidget, f )
{
    tools = new Tools ( context );
    tools->setTools ( Tools::NONE_TOOL );

    int sid = document->birdviewmiscid;
    r2d_to_element ( sid, R2D_ROOT );
    r2d_to_element ( sid, R2D_FIRST_CHILD ); // layer
    r2d_to_element ( sid, R2D_FIRST_CHILD ); // lod
    r2d_to_element ( sid, R2D_FIRST_CHILD ); // lodpage
    rectid = r2d_to_element ( sid, R2D_FIRST_CHILD ); // rect
}

void GLBirdView::paintGL ()
{
    setTransform();
    int ids[] = {document->sceneid, document->birdviewmiscid};
    r2d_update_scenes ( ids, 2 );
    swapBuffers ();
}

void GLBirdView::resizeGL ( int width, int height )
{
    GLWidget::resizeGL ( width, height );
    homeposition();
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

//================================================================================

GLScrollWidget::GLScrollWidget ( MainWindow* cont, const QGLFormat& fmt, QWidget* parent, const QGLWidget * shareWidget, Qt::WindowFlags f )
{
    context = cont;
    widget = new GLMainView(context, fmt, parent, shareWidget, f );

    hbar = new QScrollBar();
    hbar->setRange ( 0, 1 );
    hbar->setOrientation ( Qt::Horizontal );
    hbar->setSingleStep ( 1 );
    hbar->show ();
    hbar->resize ( 200, 20 );

    vbar = new QScrollBar();
    vbar->setRange ( 0, 1 );
    vbar->setOrientation ( Qt::Vertical );
    vbar->setSingleStep ( 1 );
    vbar->show ();
    vbar->resize ( 20, 200 );
    connect ( hbar, SIGNAL(valueChanged(int)), this, SLOT(onHBarValueChanged(int)) );
    connect ( vbar, SIGNAL(valueChanged(int)), this, SLOT(onVBarValueChanged(int)) );

    QHBoxLayout* h = new QHBoxLayout();
    h->setSpacing (0);
    h->addWidget ( widget );
    h->addWidget ( vbar );

    QHBoxLayout* h2 = new QHBoxLayout();
    h2->setSpacing (0);
    h2->addWidget ( hbar );
    h2->addSpacing ( 20 );

    QVBoxLayout* v = new QVBoxLayout();
    v->setContentsMargins ( 0, 0, 0, 0);
    v->setSpacing (0);
    v->addLayout ( h );
    v->addLayout ( h2 );
    setLayout ( v );
}

void GLScrollWidget::onHBarValueChanged(int value )
{
//    cout << "GLScrollWidget::onHBarValueChanged, " << value << endl;
    // calculate new translate
    float vxywh[4];
    r2d_get_viewport_rect ( vxywh ); 
    float delta = vxywh[2] / 40.0;
    widget->translate[0] = initTranslate[0] + value * delta;

    widget->updateGL ();
}

void GLScrollWidget::onVBarValueChanged(int value )
{
//    cout << "GLScrollWidget::onVBarValueChanged, " << value << endl;
    float vxywh[4];
    r2d_get_viewport_rect ( vxywh ); 
    float delta = vxywh[3] / 40.0;
    widget->translate[1] = initTranslate[1] + value * delta;

    widget->updateGL ();
}

void GLScrollWidget::setViewportTransform ( float scale, float transx, float transy )
{
    widget->scale = scale;
    widget->translate[0] = transx;
    widget->translate[1] = transy;
    widget->updateGL ();

    stringstream ss;
    ss << "scale : " << scale;
    context->statusBar()->showMessage(ss.str().c_str());

//     // calculate scrollbar
     float sminmax[4], vxywh[4], swh[2];
     r2d_get_scene_minmax ( widget->document->sceneid, sminmax, sminmax+2 );
//     cout << "++++++++++++++++++++++++min,max = " << sminmax[0] << ", " << sminmax[1] << "  " << sminmax[2] << ", " << sminmax[3] << endl;
     swh[0] = sminmax[2] - sminmax[0];
     swh[1] = sminmax[3] - sminmax[1];
     r2d_get_viewport_rect ( vxywh ); 
//     cout << "++++++++++++++++++++++++vxywh = " << vxywh[0] << ", " << vxywh[1] << "  " << vxywh[2] << ", " << vxywh[3] << endl;
     int h = 40 * (swh[0] - vxywh[2]) / vxywh[2];
     int v = 40 * (swh[1] - vxywh[3]) / vxywh[3];
//     cout << "++++++++++++++++++++++++h = " << h << endl;
     h = h > 0 ? h : 0;
     v = v > 0 ? v : 0;
     int hp = 40 * (vxywh[0] - sminmax[0]) / vxywh[2];
     int vp = v - 40 * (vxywh[1] - sminmax[1]) / vxywh[3];

    initTranslate[0] = transx - vxywh[0] + sminmax[0];
    initTranslate[1] = transy - vxywh[1] + sminmax[1];

     disconnect ( hbar, SIGNAL(valueChanged(int)), 0, 0 );
     disconnect ( vbar, SIGNAL(valueChanged(int)), 0, 0 );
     hbar->setRange ( 0, h );
     vbar->setRange ( 0, v );
     hbar->setSliderPosition  ( hp );
     vbar->setSliderPosition  ( vp );
     connect ( hbar, SIGNAL(valueChanged(int)), this, SLOT(onHBarValueChanged(int)) );
     connect ( vbar, SIGNAL(valueChanged(int)), this, SLOT(onVBarValueChanged(int)) );

     emit transformChanged (vxywh[0], vxywh[1], vxywh[0]+vxywh[2], vxywh[1]+vxywh[3] );
}

void GLScrollWidget::zoomin()
{
    try
    {
	widget->zoomin();
	setViewportTransform ( widget->scale, widget->translate[0], widget->translate[1] );
    }
    catch ( exception& ex )
    {
	cerr << ex.what () << endl;
    }
}

void GLScrollWidget::zoomout()
{
    try
    {
	widget->zoomout();
	setViewportTransform ( widget->scale, widget->translate[0], widget->translate[1] );
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

void GLScrollWidget::lefttranslate()
{
    try
    {
	widget->lefttranslate();
	setViewportTransform ( widget->scale, widget->translate[0], widget->translate[1] );
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

void GLScrollWidget::righttranslate()
{
    try
    {
	widget->righttranslate();
	setViewportTransform ( widget->scale, widget->translate[0], widget->translate[1] );
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

void GLScrollWidget::uptranslate()
{
    try
    {
	widget->uptranslate();
	setViewportTransform ( widget->scale, widget->translate[0], widget->translate[1] );
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

void GLScrollWidget::downtranslate()
{
    try
    {
	widget->downtranslate();
	setViewportTransform ( widget->scale, widget->translate[0], widget->translate[1] );
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

void GLScrollWidget::homeposition()
{
    try
    {
 	widget->homeposition();
	setViewportTransform ( widget->scale, widget->translate[0], widget->translate[1] );
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}
