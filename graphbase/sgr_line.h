#ifndef _LINE_H_
#define _LINE_H_

#include "sgr_bbox.h"
#include "sgr_vec2.h"
#include "sgr_vec3.h"
#include "sgr_vec4.h"
namespace SGR
{

template < class T >
class /*SGR_DLL*/ Line
{
public:
    Line ( T x1=0, T y1=0, T x2=0, T y2=0 ) { setPoints ( x1, y1, x2, y2 ); }
    Line ( const Line& rhs ) { setPoints (rhs.x1(), rhs.y1(), rhs.x2(), rhs.y2() ); }
    void setPoints ( T x1, T y1, T x2, T y2 ) { _data[0] = x1; _data[1] = y1; _data[2] = x2; _data[3] = y2; }
    inline T x1() const { return _data[0]; }
    inline T y1() const { return _data[1]; }
    inline T x2() const { return _data[2]; }
    inline T y2() const { return _data[3]; }
    inline void x1( T v ) { _data[0] = v; }
    inline void y1( T v ) { _data[1] = v; }
    inline void x2( T v ) { _data[2] = v; }
    inline void y2( T v ) { _data[3] = v; }
    inline vec2<T> point1() { return _data.xy(); }
    inline vec2<T> point2() { return _data.zw(); }
    inline void point1( T x, T y ) { _data.xy(x, y); }
    inline void point2( T x, T y ) { _data.zw(x, y); }
    ~Line () {}
    BBox boundingbox ()
    {
        BBox bb;
        //bb.init ( vec3<T>(_data.xy()) );
        //bb.expandby ( vec3<T>(_data.zw()) );
        bb.init ( _data.xy() );
        bb.expandby ( _data.zw() );
        return bb;
    }
private:
    vec4<T> _data;
};

typedef Line<float>  Linef;
typedef Line<double> Lined;
typedef Line<int>    Linei;
typedef Line<short>  Lines;

}
#endif // _LINE_H_
