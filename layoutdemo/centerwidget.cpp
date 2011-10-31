#include "centerwidget.h"
#include <glwidget.h>
#include <tools.h>
#include "layoutdocument.h"
#include <sgr_render2d.h>
#include <QHBoxLayout>
#include <iostream>
#include "layoutmainwindow.h"

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
GLItemsWidget::GLItemsWidget ( MainWindow* context, int* mainSceneId, const QGLFormat& fmt, QWidget* parent, const QGLWidget * shareWidget, Qt::WindowFlags f )
    : GLMainView ( context, mainSceneId, fmt, parent, shareWidget, f )
=======
=======
>>>>>>> 47b45ba... fix some bugs
GLItemsWidget::GLItemsWidget ( ViewerContext* context, Tools* t, int* mainSceneId, const QGLFormat& fmt, QWidget* parent, const QGLWidget * shareWidget, Qt::WindowFlags f )
    : GLMainView ( context, t, mainSceneId, fmt, parent, shareWidget, f )
>>>>>>> 47b45ba... fix some bugs
=======
GLItemsWidget::GLItemsWidget ( ViewerContext* context, Tools* t, int* mainSceneId, const QGLFormat& fmt, QWidget* parent, const QGLWidget * shareWidget, Qt::WindowFlags f )
    : GLMainView ( context, t, mainSceneId, fmt, parent, shareWidget, f )
>>>>>>> layoutdemo
{
}

//================================================================================

void GLItemsWidget::paintGL ()
{
    setTransform();
    LayoutDocument* doc = dynamic_cast<LayoutDocument*>(document);
    if ( doc ) {
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	//std::cout << "_________________________________" << doc->layoutSceneId << std::endl;
	int ids[] = { doc->layoutSceneId };
	r2d_update_scenes ( ids, 1 );
	swapBuffers ();
=======
        int ids[] = { doc->layoutSceneId };
        r2d_update_scenes ( ids, 1 );
//      swapBuffers ();
>>>>>>> 47b45ba... fix some bugs
=======
        int ids[] = { doc->layoutSceneId };
        r2d_update_scenes ( ids, 1 );
//      swapBuffers ();
>>>>>>> 47b45ba... fix some bugs
=======
        int ids[] = { doc->layoutSceneId };
        r2d_update_scenes ( ids, 1 );
//      swapBuffers ();
>>>>>>> layoutdemo
    }
}

//================================================================================

void GLItemsWidget::resizeGL ( int width, int height )
{
    GLMainView::resizeGL(width,height);
    homeposition1();
}

//================================================================================

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
CenterWidget::CenterWidget ( MainWindow* context, GLMainView* t, GLMainView* b, QWidget* p )
=======
CenterWidget::CenterWidget ( ViewerContext* context, GLScrollWidget* t, GLScrollWidget* b, QWidget* p )
>>>>>>> 47b45ba... fix some bugs
=======
CenterWidget::CenterWidget ( ViewerContext* context, GLScrollWidget* t, GLScrollWidget* b, QWidget* p )
>>>>>>> 47b45ba... fix some bugs
=======
CenterWidget::CenterWidget ( ViewerContext* context, GLScrollWidget* t, GLScrollWidget* b, QWidget* p )
>>>>>>> layoutdemo
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

