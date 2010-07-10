#ifndef _VEC4_H_
#define _VEC4_H_

#include "vec2.h"
#include "vec3.h"

template < class T >
class vec4
{
public:
    typedef T value_type;
    typedef vec4<T> self;
    typedef vec4<T>& reference;
    typedef const vec4<T>& const_reference;
public:
    // constructor
    vec4 ( const vec2<T>& v0, const vec2<T>& v1=vec2<T>(0,1)) { _v[0]=v0[0]; _v[1]=v0[1]; _v[2]=v1[0]; _v[3]=v1[1]; }
    vec4 ( const vec3<T>& v3, T v=1 ) { _v[0]=v3[0]; _v[1]=v3[1]; _v[2]=v3[2]; _v[3] = v; }
    vec4 ( value_type* v ) { _v[0] = v[0]; _v[1] = v[1]; _v[2] = v[2]; _v[3] = v[3]; }
    vec4 ( value_type x1=0, value_type y1=0, value_type z1=0, value_type w1=1 ) { _v[0] = x1; _v[1] = y1; _v[2] = z1; _v[3] = w1; }
    // getter & setter
    vec4 xyzw() const      { return vec4(_v[0], _v[1], _v[2], _v[3]); }
    vec3<T> xxx() const    { return vec3<T>(_v[0], _v[0], _v[0]); }
    vec3<T> xxy() const    { return vec3<T>(_v[0], _v[0], _v[1]); }
    vec3<T> xxz() const    { return vec3<T>(_v[0], _v[0], _v[2]); }
    vec3<T> xyx() const    { return vec3<T>(_v[0], _v[1], _v[0]); }
    vec3<T> xyy() const    { return vec3<T>(_v[0], _v[1], _v[1]); }
    vec3<T> xyz() const    { return vec3<T>(_v[0], _v[1], _v[2]); }
    vec3<T> xzx() const    { return vec3<T>(_v[0], _v[2], _v[0]); }
    vec3<T> xzy() const    { return vec3<T>(_v[0], _v[2], _v[1]); }
    vec3<T> xzz() const    { return vec3<T>(_v[0], _v[2], _v[2]); }
    vec2<T> xx() const     { return vec2<T>(_v[0], _v[0]); }
    vec2<T> xy() const     { return vec2<T>(_v[0], _v[1]); }
    vec2<T> xz() const     { return vec2<T>(_v[0], _v[2]); }
    vec2<T> xw() const     { return vec2<T>(_v[0], _v[3]); }
    vec2<T> yx() const     { return vec2<T>(_v[1], _v[0]); }
    vec2<T> yy() const     { return vec2<T>(_v[1], _v[1]); }
    vec2<T> yz() const     { return vec2<T>(_v[1], _v[2]); }
    vec2<T> yw() const     { return vec2<T>(_v[1], _v[3]); }
    vec2<T> zx() const     { return vec2<T>(_v[2], _v[0]); }
    vec2<T> zy() const     { return vec2<T>(_v[2], _v[1]); }
    vec2<T> zz() const     { return vec2<T>(_v[2], _v[2]); }
    vec2<T> zw() const     { return vec2<T>(_v[2], _v[3]); }
    vec2<T> wx() const     { return vec2<T>(_v[3], _v[0]); }
    vec2<T> wy() const     { return vec2<T>(_v[3], _v[1]); }
    vec2<T> wz() const     { return vec2<T>(_v[3], _v[2]); }
    vec2<T> ww() const     { return vec2<T>(_v[3], _v[3]); }

    value_type x() const           { return _v[0]; }
    value_type y() const           { return _v[1]; }
    value_type z() const           { return _v[2]; }
    value_type w() const           { return _v[3]; }
    void xyzw( T v0, T v1, T v2, T v3 ) { _v[0] = v0; _v[1] = v1; _v[2] = v2; _v[3] = v3; }
    void xyz( T v0, T v1, T v2 ) { _v[0] = v0; _v[1] = v1; _v[2] = v2; }
    void xzy( T v0, T v1, T v2 ) { _v[0] = v0; _v[2] = v1; _v[1] = v2; }
    void yxz( T v0, T v1, T v2 ) { _v[1] = v0; _v[0] = v1; _v[2] = v2; }
    void yzx( T v0, T v1, T v2 ) { _v[1] = v0; _v[2] = v1; _v[0] = v2; }
    void zxy( T v0, T v1, T v2 ) { _v[2] = v0; _v[0] = v1; _v[1] = v2; }
    void zyx( T v0, T v1, T v2 ) { _v[2] = v0; _v[1] = v1; _v[0] = v2; }
    void xy( T v0, T v1 )   { _v[0] = v0; _v[1] = v1; }
    void xz( T v0, T v1 )   { _v[0] = v0; _v[2] = v1; }
    void xw( T v0, T v1 )   { _v[0] = v0; _v[3] = v1; }
    void yz( T v0, T v1 )   { _v[1] = v0; _v[2] = v1; }
    void zx( T v0, T v1 )   { _v[2] = v0; _v[0] = v1; }
    void zy( T v0, T v1 )   { _v[2] = v0; _v[1] = v1; }
    void zw( T v0, T v1 )   { _v[2] = v0; _v[3] = v1; }
    void x( value_type v )         { _v[0] = v; }
    void y( value_type v )         { _v[1] = v; }
    void z( value_type v )         { _v[2] = v; }
    void w( value_type v )         { _v[3] = v; }
public:
    value_type mod ();
    reference operator = ( const_reference rhs );
    vec4 operator + ( const_reference rhs );
    vec4 operator - ( const_reference rhs ) const;
    vec4 operator / ( double rhs );
    value_type& operator[] (int idx)       { return _v[idx]; }
    value_type operator[] (int idx) const  { return _v[idx]; }
public:
    value_type _v[4];
};

// vec3
template < class T >
inline T vec4<T>::mod ()
{ 
    double x1 = x() / w();
    double y1 = y() / w();
    double z1 = z() / w();
    return sqrt( x1*x1 + y1*y1 + z1*z1 ); 
}



template < class T >
inline typename vec4<T>::reference vec4<T>::operator = ( const_reference rhs )
{ 
    xyzw ( rhs.x(), rhs.y(), rhs.z(), rhs.w() );
    return *this;
}

template < class T >
inline typename vec4<T>::self vec4<T>::operator + ( const_reference rhs )
{ 
    return vec3<T> ( x() + rhs.x(),  y() + rhs.y(), z() + rhs.z(), w() + rhs.w() ); 
}

template < class T >
inline typename vec4<T>::self vec4<T>::operator - ( const_reference rhs ) const
{ 
    return self ( x() - rhs.x(),  y() - rhs.y(), z() - rhs.z(), w() - rhs.w() ); 
}

template < class T >
inline typename vec4<T>::self vec4<T>::operator / ( double rhs )
{ 
    return self ( x()/rhs,  y()/rhs, z()/rhs, w()/rhs ); 
}

typedef vec4<float>  vec4f;
typedef vec4<int>    vec4i;
typedef vec4<double> vec4d;

#endif
