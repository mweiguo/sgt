#ifndef _QVIEWPORT_H_
#define _QVIEWPORT_H_

#include <QWidget>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <list>
#include "sgr_renderfunctor.h"
#include "sgr_renderflow.h"
using namespace std;
#include "agef_global.h"

class QViewport : public QWidget
{
public:
    QViewport( const char* title="", QWidget* parent=NULL );
    int vpid () { return _viewport; }
    int camid () { return _camid; }
    int projid () { return _projid; }
    
    float full_view ();
    float find_view ( const SGR::vec3f& minvec, const SGR::vec3f& maxvec, float percentOfView );
    void left ();
    void right ();
    void up ();
    void down ();
    void zoomin ();
    void zoomout ();

protected:
    virtual void resizeEvent ( QResizeEvent* event );
    virtual void paintEvent ( QPaintEvent* event );

    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event  );
    virtual void wheelEvent ( QWheelEvent * event );
    virtual void keyPressEvent ( QKeyEvent * event );
    virtual void keyReleaseEvent ( QKeyEvent * event );
private:

/*     list<SGR::Viewport*> _viewports; */
//     int _x, _y;
    int _viewport, _camid, _projid;
    // transform parameters
    float _scale;
    float _cameraPos[3];
};

#endif
