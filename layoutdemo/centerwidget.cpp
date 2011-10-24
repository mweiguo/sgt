#include "centerwidget.h"
#include <glwidget.h>
#include <tools.h>
#include "layoutdocument.h"
#include <sgr_render2d.h>
#include <QHBoxLayout>
#include <iostream>
#include "mainwindow.h"

GLItemsWidget::GLItemsWidget ( MainWindow* context, Tools* t, int* mainSceneId, const QGLFormat& fmt, QWidget* parent, const QGLWidget * shareWidget, Qt::WindowFlags f )
    : GLMainView ( context, t, mainSceneId, fmt, parent, shareWidget, f )
{
}

//================================================================================

void GLItemsWidget::paintGL ()
{
    setTransform();
    LayoutDocument* doc = dynamic_cast<LayoutDocument*>(document);
    if ( doc ) {
	int ids[] = { doc->layoutSceneId };
	r2d_update_scenes ( ids, 1 );
// 	swapBuffers ();
    }
}

//================================================================================

void GLItemsWidget::resizeGL ( int width, int height )
{
	GLMainView::resizeGL(width,height);
	homeposition1();
}

//================================================================================

CenterWidget::CenterWidget ( MainWindow* context, GLScrollWidget* t, GLScrollWidget* b, QWidget* p )
    : QWidget ( p )
{
    top    = t;
    bottom = b;
    top->setMinimumHeight ( 120 );
    top->setMaximumHeight ( 120 );

    QVBoxLayout* v = new QVBoxLayout();
    v->setContentsMargins ( 0, 0, 0, 0);
    v->setSpacing (0);
    v->addWidget ( top );
    v->addWidget ( bottom );
    setLayout ( v );
}

