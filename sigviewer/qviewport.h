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
#include "view.h"

class QViewport : public QWidget, virtual public View
{
public:
    QViewport( const char* title="", QWidget* parent=NULL );
    ~QViewport();
    virtual void updateWindow ();
    virtual int getHeight();
    virtual int getWidth();
protected:
    virtual void resizeEvent ( QResizeEvent* event );
    virtual void paintEvent ( QPaintEvent* event );

    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event  );
    virtual void wheelEvent ( QWheelEvent * event );
    virtual void keyPressEvent ( QKeyEvent * event );
    virtual void keyReleaseEvent ( QKeyEvent * event );
};

#endif
