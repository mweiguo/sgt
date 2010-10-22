#ifndef _PROJECTION_H_
#define _PROJECTION_H_
namespace SGR
{

class Projection
{
public:
    const mat4f& projmatrix() { return _mat; }
    const mat4f& inversematrix() { return _inversemat; }
private:
    mat4f _mat, _inversemat;
};
}
#endif //_PROJECTION_H_
