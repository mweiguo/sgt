#ifndef _QGLVIEWPORT_H_
#define _QGLVIEWPORT_H_

#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <sgr_vec2.h>
#include "view.h"

class SGVTools;
class QGLViewport : public QGLWidget, virtual public View
{
public:
    QGLViewport( const char* title );
    virtual ~QGLViewport();

    virtual void updateWindow ();
    SGVTools* sgvtools () { return _tools; }
    const SGVTools* sgvtools () const { return _tools; }

    SGR::vec2f viewportToScene ( SGR::vec2f vpxy );
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
    int _viewportid, _camid, _projid;
    SGVTools* _tools;
};

#endif // _QGLVIEWPORT_H_
