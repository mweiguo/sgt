#ifndef _QGLVIEWPORT_H_
#define _QGLVIEWPORT_H_

#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <sgr_vec3.h>

class QGLViewport : public QGLWidget
{
public:
    static QGLViewport& getInst();

    int vpid () { return _viewportid; }
    int camid () { return _camid; }
    int projid () { return _projid; }
    
    float full_view ();
    float find_view ( const SGR::vec3f& minvec, const SGR::vec3f& maxvec, float percentOfView );
    
protected:
    virtual void paintGL ();
    virtual void resizeGL ( int width, int height );


    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event  );
    virtual void wheelEvent ( QWheelEvent * event );
    virtual void keyPressEvent ( QKeyEvent * event );
    virtual void keyReleaseEvent ( QKeyEvent * event );
private:
    QGLViewport( const char* title );

    int _viewportid, _camid, _projid;
};

#endif // _QGLVIEWPORT_H_
