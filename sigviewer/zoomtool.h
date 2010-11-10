#ifndef _ZOOMTOOL_H_
#define _ZOOMTOOL_H_
#include "sgv_tools.h"

class ZoomTool : public SGVTool
{
public:
    ZoomTool ();
    virtual void lbuttondown ( float x, float y );
    virtual void lbuttonup ( float x, float y );
    virtual void lbuttonmove ( float x, float y );
    virtual void mousewheel ( float x, float y, float delta );
protected:
    float _scale;
};

#endif // _ZOOMTOOL_H_
