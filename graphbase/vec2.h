#ifndef _VEC2_H_
#define _VEC2_H_

#include <cmath>

template < class T >
class vec2
{
public:
    typedef vec2<T> self;
    typedef vec2<T>& reference;
    typedef T value_type;
    typedef const vec2 const_reference;
public:
    // constructor
    template< class Vec >
    vec2 ( const Vec& v ) { _v[0] = v.x(); _v[1] = v.y(); }
    vec2 ( T* v ) { _v[0] = v[0]; _v[1] = v[1]; }
    vec2 ( T x1=0, T y1=0 ) { _v[0] = x1; _v[1] = y1; }
    // getter & setter
    vec2 xx() const { return vec2(_v[0],_v[0]); }
    vec2 xy() const { return *this; }
    vec2 yx() const { return vec2(_v[1],_v[0]); }
    vec2 yy() const { return vec2(_v[1],_v[1]); }
    T x() const { return _v[0]; }
    T y() const { return _v[1]; }
    T w() const { return _v[0]; }
    T h() const { return _v[1]; }

    void x( T v ) { _v[0] = v; }
    void y( T v ) { _v[1] = v; }
    void w( T v ) { _v[0] = v; }
    void h( T v ) { _v[1] = v; }
    void xy( T v0, T v1 ) { _v[0] = v0; _v[1] = v1; }
    void xy( vec2 v ) { _v[0] = v.x(); _v[1] = v.y(); }
    void yx( vec2 v ) { _v[0] = v.y(); _v[1] = v.x(); }
public:
    reference normal ();
    self mod ( T length );
    T mod ();
    self operator - ( const_reference rhs );
    reference operator *= ( T rhs ); 
    vec2 operator = ( const vec2& rhs );
    vec2 operator + ( const vec2& rhs );
    T& operator[] (int idx)       { return _v[idx]; }
    T operator[] (int idx) const  { return _v[idx]; }

protected:
    T _v[2];
};


// vec2
template < class T >
inline typename vec2<T>::reference vec2<T>::normal () 
{ 
    typename vec2<T>::value_type m = mod (); 
    if ( m != 0 ) {
        _v[0] /= m; 
        _v[1] /= m; 
    }
    return *this;
}

template < class T >
inline vec2<T> vec2<T>::mod ( T length ) 
{ 
    normal();
    *this *= length;
    return *this;
}

template < class T >
inline T vec2<T>::mod ()
{
    return sqrt((double)(_v[0]*_v[0] + _v[1]*_v[1])); 
}

template < class T >
inline vec2<T> vec2<T>::operator - ( typename vec2<T>::const_reference rhs )
{
    return vec2 ( _v[0] - rhs._v[0],  _v[1] - rhs._v[1] ); 
}

template < class T >
inline typename vec2<T>::reference vec2<T>::operator *= ( T rhs )
{
    _v[0] *= rhs;
    _v[1] *= rhs;
    return *this;
}

template < class T >
inline vec2<T> vec2<T>::operator = ( const vec2<T>& rhs )
{
    x ( rhs.x() );
    y ( rhs.y() );
    return *this;
}

template < class T >
inline vec2<T> vec2<T>::operator + ( const vec2<T>& rhs )
{
    x ( x() + rhs.x() );
    y ( y() + rhs.y() );
    return *this;
}

typedef vec2<float> vec2f;
typedef vec2<double> vec2d;
typedef vec2<int> vec2i;
typedef vec2<short> vec2s;

#endif
