#ifndef _HANDTOOL_H_
#define _HANDTOOL_H_
#include "sgv_tools.h"

class HandTool : public SGVTool
{
public:
    virtual void lbuttondown ( float x, float y );
    virtual void lbuttonup ( float x, float y );
    virtual void lbuttonmove ( float x, float y );
    virtual void mbuttondown ( float x, float y );
    virtual void mbuttonup ( float x, float y );
    virtual void mbuttonmove ( float x, float y );
};

#endif // _HANDTOOL_H_
