#ifndef _QGLVIEWPORT_H_
#define _QGLVIEWPORT_H_

#include <QGLWidget>
#include <sgr_vec2.h>
#include "view.h"

class SGVTools;
class QGLViewport : public QGLWidget, virtual public View
{
public:
    QGLViewport( const char* title );
    virtual ~QGLViewport();

    virtual void updateWindow ();
    virtual int getHeight();
    virtual int getWidth();
protected:
    virtual void paintGL ();
    virtual void resizeGL ( int width, int height );
};

#endif // _QGLVIEWPORT_H_
