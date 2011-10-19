#include "centerwidget.h"
#include <glwidget.h>
#include <tools.h>
#include "layoutdocument.h"
#include <sgr_render2d.h>
#include <QHBoxLayout>
#include <iostream>
#include "mainwindow.h"

GLItemsWidget::GLItemsWidget ( MainWindow* context, int* mainSceneId, const QGLFormat& fmt, QWidget* parent, const QGLWidget * shareWidget, Qt::WindowFlags f )
    : GLMainView ( context, mainSceneId, fmt, parent, shareWidget, f )
{
    tools = new Tools ( context );
    tools->setTools ( Tools::NONE_TOOL | Tools::HAND_TOOL | Tools::ZOOM_TOOL );
}

//================================================================================

void GLItemsWidget::paintGL ()
{
    setTransform();
    LayoutDocument* doc = dynamic_cast<LayoutDocument*>(document);
    if ( doc ) {
	//std::cout << "_________________________________" << doc->layoutSceneId << std::endl;
	int ids[] = { doc->layoutSceneId };
	r2d_update_scenes ( ids, 1 );
	swapBuffers ();
    }
}

//================================================================================

void GLItemsWidget::resizeGL ( int width, int height )
{
	GLMainView::resizeGL(width,height);
	homeposition1();
}

// GLScrollItemsWidget::GLScrollItemsWidget ( MainWindow* context, const QGLFormat& fmt, QWidget* parent, const QGLWidget * shareWidget, Qt::WindowFlags f )
// {
//     context = cont;
//     widget = new GLItemsWidget(context, fmt, parent, shareWidget, f );

//     hbar = new QScrollBar();
//     hbar->setRange ( 0, 1 );
//     hbar->setOrientation ( Qt::Horizontal );
//     hbar->setSingleStep ( 1 );
//     hbar->show ();
//     hbar->resize ( 200, 20 );

//     vbar = new QScrollBar();
//     vbar->setRange ( 0, 1 );
//     vbar->setOrientation ( Qt::Vertical );
//     vbar->setSingleStep ( 1 );
//     vbar->show ();
//     vbar->resize ( 20, 200 );
//     connect ( hbar, SIGNAL(valueChanged(int)), this, SLOT(onHBarValueChanged(int)) );
//     connect ( vbar, SIGNAL(valueChanged(int)), this, SLOT(onVBarValueChanged(int)) );

//     QHBoxLayout* h = new QHBoxLayout();
//     h->setSpacing (0);
//     h->addWidget ( widget );
//     h->addWidget ( vbar );

//     QHBoxLayout* h2 = new QHBoxLayout();
//     h2->setSpacing (0);
//     h2->addWidget ( hbar );
//     h2->addSpacing ( 20 );

//     QVBoxLayout* v = new QVBoxLayout();
//     v->setContentsMargins ( 0, 0, 0, 0);
//     v->setSpacing (0);
//     v->addLayout ( h );
//     v->addLayout ( h2 );
//     setLayout ( v );
// }

//================================================================================

CenterWidget::CenterWidget ( MainWindow* context, GLMainView* t, GLMainView* b, QWidget* p )
    : QWidget ( p )
{
    top    = new GLScrollWidget(context, t );
    bottom = new GLScrollWidget(context, b );
    top->setMinimumHeight ( 120 );
    top->setMaximumHeight ( 120 );

    QVBoxLayout* v = new QVBoxLayout();
    v->setContentsMargins ( 0, 0, 0, 0);
    v->setSpacing (0);
    v->addWidget ( top );
    v->addWidget ( bottom );
    setLayout ( v );
}

