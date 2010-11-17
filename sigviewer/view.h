#ifndef _VIEW_H_
#define _VIEW_H_

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include "sgv_tools.h"

class View
{
public:
    View ();
    virtual ~View ();
    virtual int vpid () { return _viewportid; }
    virtual int camid () { return _camid; }
    virtual int projid () { return _projid; }
    
    virtual void updateWindow ();
    virtual int getHeight();
    virtual int getWidth();

// keyborad & mouse events
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event  );
    virtual void wheelEvent ( QWheelEvent * event );
    virtual void keyPressEvent ( QKeyEvent * event );
    virtual void keyReleaseEvent ( QKeyEvent * event );
protected:
    SGVTools _tools;
private:
    int _viewportid, _camid, _projid;
};

#endif // _VIEW_H_
