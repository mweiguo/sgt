#ifndef _SGR_RENDEROPTION_H_
#define _SGR_RENDEROPTION_H_

#include "sgr_bbox.h"
#include "sgr_mat4.h"

namespace SGR
{

class RenderOption
{
public:
    BBox bb;
    //mat4f* mvmatrix;
    // matrix = viewpmapping * model-view
    mat4f matrix, reverse_mvpw;
    float scale;
    float vpXYWH[4];
    SGR::mat4f mvmat, projmat;

    RenderOption() {}
    virtual ~RenderOption() {}
};

}

#endif //_SGR_RENDEROPTION_H_
