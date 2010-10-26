#include "sgr_projection.h"
namespace SGR
{

const mat4f& Projection::projmatrix()
{
    return _mat;
}

const mat4f& Projection::inversematrix()
{
    return _invmat;
}

void Projection::frustum ( float left, float right, float bottom, float top, float near, float far )
{
    _mat = mat4f::frustum_matrix ( left, right, bottom, top, near, far );
    _invmat = mat4f::frustum_inv_matrix ( left, right, bottom, top, near, far );
}

void Projection::ortho ( float left, float right, float bottom, float top, float near, float far )
{
    _mat = mat4f::ortho_matrix ( left, right, bottom, top, near, far );
    _invmat = mat4f::ortho_inv_matrix ( left, right, bottom, top, near, far );
}

void Projection::perspective ( float fovy, float aspect, float near, float far )
{
}

}
