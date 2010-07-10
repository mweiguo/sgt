#ifndef _QVIEWPORT_H_
#define _QVIEWPORT_H_

#include <QWidget>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <list>
#include "renderfunctor.h"
#include "renderflow.h"
#include "nodemgr.h"
using namespace std;
#include "agef_global.h"

class AGEF_EXPORT QViewport : public QWidget
{
public:
    static QViewport& getInst()
    {
        static QViewport inst("viewport");
        return inst;
    }
    void add_viewport ( int id );
    void remove_viewport ( int id );
    void dirty_all ();

protected:
    virtual void resizeEvent ( QResizeEvent* event );
    virtual void paintEvent ( QPaintEvent* event );
    virtual void mouseMoveEvent ( QMouseEvent * event  );
private:
    QViewport( const char* title );

    list<Viewport*> _viewports;
    int _x, _y;
    //    int _viewport, _camid;
};

#endif
