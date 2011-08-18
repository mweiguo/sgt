#include "glwidget.h"


GLWidget::GLWidget ( const QGLFormat& fmt, QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags f ) :
    QGLWidget ( fmt, parent, shareWidget, f )
{
}

void GLWidget::initializeGL ()
{
    glClearColor ( 0.4, 0.0, 0.0, 0.0 );
    glEnable ( GL_DEPTH_TEST );

    dl = glGenLists ( 1 );
    glNewList ( dl, GL_COMPILE );
    glBegin( GL_TRIANGLES );
    glVertex3f ( 0.0, 0.0, 0.0 );
    glVertex3f ( 1.0, 0.0, 0.0 );
    glVertex3f ( 1.0, 1.0, 0.0 );
    glVertex3f ( 0.0, 0.0, 0.0 );
    glVertex3f ( -1.0, 0.0, 0.0 );
    glVertex3f ( -1.0, -1.0, 0.0 );
    glEnd ();
    glEndList ();
}


void GLWidget::paintGL ()
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity ();
    gluLookAt ( 0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );

    glCallList ( dl );
    swapBuffers ();
}

void GLWidget::resizeGL ( int width, int height )
{
    glViewport ( 0, 0, width, height );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity ();
    gluPerspective ( 30.0, 1.0 * width / height, 1.0, 100.0 );
    glMatrixMode ( GL_MODELVIEW );
}

