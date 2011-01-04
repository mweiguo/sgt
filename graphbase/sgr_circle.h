#ifndef _CIRCLE_H_
#define _CIRCLE_H_

#include <sgr_bbox.h>
#include "sgr_global.h"
#include "sgr_line.h"

namespace SGR
{

template < class T >
class /*SGR_DLL*/ Circle
{
public:
    Circle ( T x=0, T y=0, T r=0 )
    {
        setCenter ( x, y, 0 );
        setRadius ( r );
    }
    Circle ( const Circle<T>& rhs )
    {
        setCenter ( rhs._center );
        setRadius ( rhs._radius );
    }
    void setCenter ( vec3<T> cen )
    {
        _center = cen;
    }
    void setCenter ( T x, T y, T z )
    {
        _center.xyz ( x, y, z );
    }
    vec3<T> getCenter () const
    {
        return _center;
    }
    void setRadius ( T r )
    {
        _radius = r;
    }
    T getRadius () const
    {
        return _radius;
    }

    ~Circle () {}
    BBox boundingbox () const
    {
        BBox bb;
        bb.init ( _center-_radius );
        bb.expandby ( _center+_radius );
        return bb;
    }
    inline void setUserData ( void* data ) { _userData = data; }
    inline void* getUserData () { return _userData; }

private:
    vec3<T> _center;
    T _radius;
    void* _userData;
};

typedef Circle<float>  Circlef;
typedef Circle<double> Circled;
typedef Circle<int>    Circlei;
typedef Circle<short>  Circles;

}

#endif // _CIRCLE_H_
