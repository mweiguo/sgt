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

    void Projection::frustum ( float /*left*/, float /*right*/, float /*bottom*/, float /*top*/, float /*near*/, float /*far*/ )
    {
    }

    void Projection::ortho ( float /*left*/, float /*right*/, float /*bottom*/, float /*top*/, float /*near*/, float /*far*/ )
    {
    }

    void Projection::perspective ( float /*fovy*/, float /*aspect*/, float /*near*/, float /*far*/ )
    {
    }

    SGNode* Projection::clone ()
    {
        return new Projection(*this);
    }

}
