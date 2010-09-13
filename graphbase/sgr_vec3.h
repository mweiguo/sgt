#ifndef _VEC3_H_
#define _VEC3_H_

#include "sgr_vec2.h"
namespace SGR
{

template < class T >
class vec3
{
 public:
    typedef T value_type;
    typedef const vec3& const_reference;
 public:
    // constructor
    vec3 ( const vec2<T>& v2 ) { _v[0]=v2[0]; _v[1]=v2[1]; _v[2]=0; }
    vec3 ( const vec3<T>& v3 ) { 
        _v[0]=v3[0]; 
        _v[1]=v3[1]; 
        _v[2]=v3[2];
    }
    vec3 ( T* v ) { _v[0] = v[0]; _v[1] = v[1]; _v[2] = v[2]; }
    vec3 ( T x1=0, T y1=0, T z1=0 ) { _v[0] = x1; _v[1] = y1; _v[2] = z1; }
    // min max element
    T min_element () 
    {
        return _v[0] < _v[1] ? ( _v[0] < _v[2] ? _v[0] : _v[2] ) : ( _v[1] < _v[2] ? _v[1] : _v[2] );
    }
    T max_element () 
    {
        return _v[0] > _v[1] ? ( _v[0] > _v[2] ? _v[0] : _v[2] ) : ( _v[1] > _v[2] ? _v[1] : _v[2] );
    }
    
    // getter
    void xyz ( T* v0, T* v1, T* v2 ) const { *v0=_v[0]; *v1=_v[1]; *v2=_v[2]; }
    void xyz ( T* v ) const { v[0]=_v[0]; v[1]=_v[1]; v[2]=_v[2]; }
    // compound
    vec3 xxx() const       	     { return vec3(_v[0], _v[0], _v[0]); }
    vec3 xxy() const       	     { return vec3(_v[0], _v[0], _v[1]); }
    vec3 xxz() const       	     { return vec3(_v[0], _v[0], _v[2]); }
    vec3 xyx() const       	     { return vec3(_v[0], _v[1], _v[0]); }
    vec3 xyy() const       	     { return vec3(_v[0], _v[1], _v[1]); }
    vec3 xyz() const       	     { return vec3(_v[0], _v[1], _v[2]); }
    vec3 xzx() const       	     { return vec3(_v[0], _v[2], _v[0]); }
    vec3 xzy() const       	     { return vec3(_v[0], _v[2], _v[1]); }
    vec3 xzz() const       	     { return vec3(_v[0], _v[2], _v[2]); }
    vec2<T> xx() const     	     { return vec2<T>(_v[0], _v[0]); }
    vec2<T> xy() const     	     { return vec2<T>(_v[0], _v[1]); }
    vec2<T> xz() const     	     { return vec2<T>(_v[0], _v[2]); }
    vec2<T> yx() const     	     { return vec2<T>(_v[1], _v[0]); }
    vec2<T> yy() const     	     { return vec2<T>(_v[1], _v[1]); }
    vec2<T> yz() const     	     { return vec2<T>(_v[1], _v[2]); }
    vec2<T> zx() const     	     { return vec2<T>(_v[2], _v[0]); }
    vec2<T> zy() const     	     { return vec2<T>(_v[2], _v[1]); }
    vec2<T> zz() const     	     { return vec2<T>(_v[2], _v[2]); }
    T x() const           	     { return _v[0]; }
    T y() const           	     { return _v[1]; }
    T z() const           	     { return _v[2]; }
    T w() const           	     { return _v[0]; }
    T h() const           	     { return _v[1]; }
    T l() const           	     { return _v[2]; }
    void xyz( T v0, T v1, T v2 )     { _v[0] = v0; _v[1] = v1; _v[2] = v2; }
    void xzy( T v0, T v1, T v2 )     { _v[0] = v0; _v[2] = v1; _v[1] = v2; }
    void yxz( T v0, T v1, T v2 )     { _v[1] = v0; _v[0] = v1; _v[2] = v2; }
    void yzx( T v0, T v1, T v2 )     { _v[1] = v0; _v[2] = v1; _v[0] = v2; }
    void zxy( T v0, T v1, T v2 )     { _v[2] = v0; _v[0] = v1; _v[1] = v2; }
    void zyx( T v0, T v1, T v2 )     { _v[2] = v0; _v[1] = v1; _v[0] = v2; }
    void xy( T v0, T v1 )   	     { _v[0] = v0; _v[1] = v1; }
    void xz( T v0, T v1 )   	     { _v[0] = v0; _v[2] = v1; }
    void yz( T v0, T v1 )   	     { _v[1] = v0; _v[2] = v1; }
    void zx( T v0, T v1 )   	     { _v[2] = v0; _v[0] = v1; }
    void zy( T v0, T v1 )   	     { _v[2] = v0; _v[1] = v1; }
    void x( T v )           	     { _v[0] = v; }
    void y( T v )           	     { _v[1] = v; }
    void z( T v )           	     { _v[2] = v; }
 public:
    T mod ();
    const vec3<T>& normal ();
    //bool operator < ( const_reference rhs ) const;
    //bool operator > ( const_reference rhs ) const;
    bool operator != ( const_reference rhs ) const;
    bool operator == ( const_reference rhs ) const;
    vec3 operator = ( const_reference rhs );
    vec3 operator += ( const_reference rhs );
    vec3 operator -= ( const_reference rhs );
    vec3 operator + ( const_reference rhs ) const;
    vec3 operator - ( const_reference rhs ) const;
    bool operator > ( const_reference rhs ) const;
    bool operator < ( const_reference rhs ) const;
    bool operator >= ( const_reference rhs ) const;
    bool operator <= ( const_reference rhs ) const;
    vec3 operator / ( double rhs ) const;
    T& operator[] (int idx)       { return _v[idx]; }
    T operator[] (int idx) const  { return _v[idx]; }
    vec3 cross ( const_reference rhs ) const;
    vec3 min_element ( const vec3<T>& rhs ) const;
    vec3 max_element ( const vec3<T>& rhs ) const;
 public:
    T _v[3];
};

// vec3
template < class T >
inline T vec3<T>::mod ()
{ 
    return sqrt((double)(x()*x() + y()*y() + z()*z()) ); 
}

template < class T >
inline const vec3<T>& vec3<T>::normal ()
{
    typename vec2<T>::value_type m = mod (); 
    if ( m != 0 ) {
        _v[0] /= m; 
        _v[1] /= m; 
        _v[2] /= m; 
    }
    return *this;

}
//template < class T >
//inline bool vec3<T>::operator < ( const vec3<T>& rhs ) const
//{ 
//  return ( (x() < rhs.x()) && (y()<rhs.y()) && (z()<rhs.z()) );
//}
//
//template < class T >
//inline bool vec3<T>::operator > ( const vec3<T>& rhs ) const
//{ 
//  return ( (x()>rhs.x()) && (y()>rhs.y()) && (z()>rhs.z()) );
//}
//
template < class T >
inline bool vec3<T>::operator != ( const vec3<T>& rhs ) const
{ 
    return x()!=rhs.x() || y()!=rhs.y() || z()!=rhs.z();
}

template < class T >
inline bool vec3<T>::operator == ( const vec3<T>& rhs ) const
{ 
    return (x()==rhs.x() && y()==rhs.y() && z()==rhs.z());
}

template < class T >
inline vec3<T> vec3<T>::operator = ( const vec3<T>& rhs )
{ 
    xyz ( rhs.x(), rhs.y(), rhs.z() );
    return *this;
}

template < class T >
inline vec3<T> vec3<T>::operator += ( const vec3<T>& rhs )
{ 
    xyz ( x()+rhs.x(), y()+rhs.y(), z()+rhs.z() );
    return *this;
}

template < class T >
inline vec3<T> vec3<T>::operator -= ( const vec3<T>& rhs )
{ 
    xyz ( x()-rhs.x(), y()-rhs.y(), z()-rhs.z() );
    return *this;
}

template < class T >
inline vec3<T> vec3<T>::operator + ( const vec3<T>& rhs ) const
{ 
    return vec3<T> ( x() + rhs.x(),  y() + rhs.y(), z() + rhs.z() ); 
}

template < class T >
inline vec3<T> vec3<T>::operator - ( const vec3<T>& rhs ) const
{ 
    return vec3<T> ( x() - rhs.x(),  y() - rhs.y(), z() - rhs.z() ); 
}

template < class T >
inline bool vec3<T>::operator > ( const vec3<T>& rhs ) const
{ 
    return (x() > rhs.x()) &&  (y() > rhs.y()) && (z() > rhs.z()); 
}

template < class T >
inline bool vec3<T>::operator < ( const vec3<T>& rhs ) const
{ 
    return (x() < rhs.x()) &&  (y() < rhs.y()) && (z() < rhs.z()); 
}

template < class T >
inline bool vec3<T>::operator >= ( const vec3<T>& rhs ) const
{ 
    return (x() >= rhs.x()) &&  (y() >= rhs.y()) && (z() >= rhs.z()); 
}

template < class T >
inline bool vec3<T>::operator <= ( const vec3<T>& rhs ) const
{ 
    return (x() <= rhs.x()) &&  (y() <= rhs.y()) && (z() <= rhs.z()); 
}

template < class T >
inline vec3<T> vec3<T>::operator / ( double rhs ) const
{ 
    return vec3<T> ( (T)(x() / rhs),  (T)(y() / rhs), (T)(z() / rhs) ); 
}

template < class T >
inline vec3<T> vec3<T>::cross ( const vec3<T>& rhs ) const
{
    return vec3<T> ( y() * rhs.z() - rhs.y() * z(), 
                             z() * rhs.x() - x() * rhs.z(),
                             x() * rhs.y() - rhs.x() * y() );
} 

template < class T >
inline vec3<T> vec3<T>::min_element ( const vec3<T>& rhs ) const
{
    return vec3<T> ( x() < rhs.x() ? x() : rhs.x(),
                             y() < rhs.y() ? y() : rhs.y(),
                             z() < rhs.z() ? z() : rhs.z() );
} 

template < class T >
inline vec3<T> vec3<T>::max_element ( const vec3<T>& rhs ) const
{
    return vec3<T> ( x() > rhs.x() ? x() : rhs.x(),
                             y() > rhs.y() ? y() : rhs.y(),
                             z() > rhs.z() ? z() : rhs.z() );
}

typedef vec3<float>  vec3f;
typedef vec3<int>    vec3i;
typedef vec3<double> vec3d;
typedef vec3<short>  vec3s;
}
#endif
