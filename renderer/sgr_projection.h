#ifndef _PROJECTION_H_
#define _PROJECTION_H_
#include "sgr_sgnode.h"
#include "sgr_mat4.h"

namespace SGR
{

class Projection : public SGNode
{
public:
    const mat4f& projmatrix();
    const mat4f& inversematrix();                 
    void frustum ( float left, float right, float bottom, float top, float near, float far );
    void ortho ( float left, float right, float bottom, float top, float near, float far );
    void perspective ( float fovy, float aspect, float near, float far );
private:
    mat4f _mat;
    mat4f _invmat;
};
}
#endif //_PROJECTION_H_
