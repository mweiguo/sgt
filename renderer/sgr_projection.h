#ifndef _PROJECTION_H_
#define _PROJECTION_H_
namespace SGR
{

class Projection
{
public:
    const mat4f& projmatrix() { return _mat; }
private:
    mat4f _mat;
};
}
#endif //_PROJECTION_H_
