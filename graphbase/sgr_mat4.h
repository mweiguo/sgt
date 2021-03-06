#ifndef _MAT4_H_
#define _MAT4_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstring>
namespace SGR
{

template < class ValueType >
class mat4
{
public:
    typedef mat4 self;
    typedef mat4& reference;
    typedef const mat4& const_reference;

    mat4 ( ValueType a00=1, ValueType a01=0, ValueType a02=0, ValueType a03=0, 
           ValueType a10=0, ValueType a11=1, ValueType a12=0, ValueType a13=0, 
           ValueType a20=0, ValueType a21=0, ValueType a22=1, ValueType a23=0,
           ValueType a30=0, ValueType a31=0, ValueType a32=0, ValueType a33=1 ) {
        m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
        m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
        m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
        m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;
    }
    mat4 ( ValueType* data )
    {
	memcpy ( m, data, sizeof(ValueType)*16 );
    }
    mat4 operator* ( const_reference rhs ) const;

    template < class Vec >
    Vec operator * ( const Vec& rhs ) const
    {
        typename Vec::value_type t  = m[0][0]*rhs[0] + m[0][1]*rhs[1] + m[0][2]*rhs[2] + m[0][3]*rhs[3];
        typename Vec::value_type t1 = m[1][0]*rhs[0] + m[1][1]*rhs[1] + m[1][2]*rhs[2] + m[1][3]*rhs[3];
        typename Vec::value_type t2 = m[2][0]*rhs[0] + m[2][1]*rhs[1] + m[2][2]*rhs[2] + m[2][3]*rhs[3];
        typename Vec::value_type t3 = m[3][0]*rhs[0] + m[3][1]*rhs[1] + m[3][2]*rhs[2] + m[3][3]*rhs[3];
        return Vec ( t, t1, t2, t3 );
    }

    const mat4& operator*= ( const_reference rhs ) const;
    const mat4& operator= ( const_reference rhs ) const;

    const bool operator== ( const_reference rhs ) const
    {
        if ( this == &rhs )
            return true;

        for ( int i=0; i<4; i++ )
        {
            for ( int j=0; j<4; j++ )
            {
                if ( m[i][j] != rhs.m[i][j] )
                    return false;
            }
        }

        return true;
    }

    void dx ( ValueType v ) { m[0][3] = v; }
    void dy ( ValueType v ) { m[1][3] = v; }
    void dz ( ValueType v ) { m[2][3] = v; }

    void sx ( ValueType v ) { m[0][0] = v; }
    void sy ( ValueType v ) { m[1][1] = v; }
    void sz ( ValueType v ) { m[2][2] = v; }

    ValueType dx () const { return m[0][3]; }
    ValueType dy () const { return m[1][3]; }
    ValueType dz () const { return m[2][3]; }

    ValueType sx () const { return m[0][0]; }
    ValueType sy () const { return m[1][1]; }
    ValueType sz () const { return m[2][2]; }

    ValueType m00 () const { return m[0][0]; }
    ValueType m01 () const { return m[0][1]; }
    ValueType m02 () const { return m[0][2]; }
    ValueType m03 () const { return m[0][3]; }
    ValueType m10 () const { return m[1][0]; }
    ValueType m11 () const { return m[1][1]; }
    ValueType m12 () const { return m[1][2]; }
    ValueType m13 () const { return m[1][3]; }
    ValueType m20 () const { return m[2][0]; }
    ValueType m21 () const { return m[2][1]; }
    ValueType m22 () const { return m[2][2]; }
    ValueType m23 () const { return m[2][3]; }
    ValueType m30 () const { return m[3][0]; }
    ValueType m31 () const { return m[3][1]; }
    ValueType m32 () const { return m[3][2]; }
    ValueType m33 () const { return m[3][3]; }

    ValueType& m00 () { return m[0][0]; }
    ValueType& m01 () { return m[0][1]; }
    ValueType& m02 () { return m[0][2]; }
    ValueType& m03 () { return m[0][3]; }
    ValueType& m10 () { return m[1][0]; }
    ValueType& m11 () { return m[1][1]; }
    ValueType& m12 () { return m[1][2]; }
    ValueType& m13 () { return m[1][3]; }
    ValueType& m20 () { return m[2][0]; }
    ValueType& m21 () { return m[2][1]; }
    ValueType& m22 () { return m[2][2]; }
    ValueType& m23 () { return m[2][3]; }
    ValueType& m30 () { return m[3][0]; }
    ValueType& m31 () { return m[3][1]; }
    ValueType& m32 () { return m[3][2]; }
    ValueType& m33 () { return m[3][3]; }

    void m00 ( ValueType v ) { m[0][0] = v; }
    void m01 ( ValueType v ) { m[0][1] = v; }
    void m02 ( ValueType v ) { m[0][2] = v; }
    void m03 ( ValueType v ) { m[0][3] = v; }
    void m10 ( ValueType v ) { m[1][0] = v; }
    void m11 ( ValueType v ) { m[1][1] = v; }
    void m12 ( ValueType v ) { m[1][2] = v; }
    void m13 ( ValueType v ) { m[1][3] = v; }
    void m20 ( ValueType v ) { m[2][0] = v; }
    void m21 ( ValueType v ) { m[2][1] = v; }
    void m22 ( ValueType v ) { m[2][2] = v; }
    void m23 ( ValueType v ) { m[2][3] = v; }
    void m30 ( ValueType v ) { m[3][0] = v; }
    void m31 ( ValueType v ) { m[3][1] = v; }
    void m32 ( ValueType v ) { m[3][2] = v; }
    void m33 ( ValueType v ) { m[3][3] = v; }

    void normalize () {
        m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
        m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
        m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
        m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
    }
	// mat4 operator* ( const_reference rhs );
	// template <class Vec4>
	// Vec4 operator* ( Vec4::const_reference rhs );
    void toArray ( ValueType* ptr ) const
    {
	memcpy ( ptr, m, sizeof(ValueType)*16 );
    }
    // x=0, y=1, z=2
    static mat4 translate_matrix ( float x, float y, float z );
    static mat4 rotate_matrix ( float angle, int axis );
    static mat4 scale_matrix ( ValueType sx, ValueType sy, ValueType sz );
    static mat4 frustum_matrix ( ValueType l, ValueType r, ValueType b, ValueType t, ValueType n, ValueType f );
    static mat4 ortho_matrix ( ValueType l, ValueType r, ValueType b, ValueType t, ValueType n, ValueType f );

    static mat4 translate_inv_matrix ( float x, float y, float z );
    static mat4 rotate_inv_matrix ( float angle, int axis );
    static mat4 scale_inv_matrix ( ValueType sx, ValueType sy, ValueType sz );
    static mat4 frustum_inv_matrix ( ValueType l, ValueType r, ValueType b, ValueType t, ValueType n, ValueType f );
    static mat4 ortho_inv_matrix ( ValueType l, ValueType r, ValueType b, ValueType t, ValueType n, ValueType f );
private:
    ValueType m[4][4];
};

template < class ValueType >
mat4<ValueType> mat4<ValueType>::translate_matrix ( float dx, float dy, float dz )
{
    return mat4<ValueType> ( 1,  0,  0,  dx, 
                             0,  1,  0,  dy, 
                             0,  0,  1,  dz,
                             0,  0,  0,  1);
}

template < class ValueType >
mat4<ValueType> mat4<ValueType>::rotate_matrix ( float angle, int axis )
{
    float alpha = angle * M_PI / 180;
    switch ( axis ) {
    case 0:
        return mat4<ValueType> ( 1, 0,          0,           0,
                                 0, cos(alpha), -sin(alpha), 0,
                                 0, sin(alpha), cos(alpha),  0, 
                                 0, 0,          0,           1 );
    case 1:
        return mat4<ValueType> ( cos(alpha),  0, sin(alpha),  0,
                                 0,           1, 0,           0,
                                 -sin(alpha), 0, cos(alpha),  0, 
                                 0,           0, 0,           1 );
    case 2:
        return mat4<ValueType> ( cos(alpha),  -sin(alpha), 0, 0,
                                 sin(alpha),  cos(alpha),  0, 0,
                                 0,           0,           1, 0, 
                                 0,           0,           0, 1 );
    }
}

template < class ValueType >
mat4<ValueType> mat4<ValueType>::scale_matrix ( ValueType sx, ValueType sy, ValueType sz )
{
    return mat4<ValueType> ( sx, 0,  0,  0, 
                             0,  sy, 0,  0, 
                             0,  0,  sz, 0,
                             0,  0,  0,  1);
}

template < class ValueType >
mat4<ValueType> mat4<ValueType>::frustum_matrix ( ValueType l, ValueType r, ValueType b, ValueType t, ValueType n, ValueType f )
{
    return mat4<ValueType> ( (n*2)/(r-l),           0,  (r+l)/(r-l),  0, 
                             	       0, (n*2)/(t-b),  (t+b)/(t-b),  0, 
                             	       0,           0,  (f+n)/(n-f),  0,
                             	       0,           0,           -1,  0);
}

template < class ValueType >
mat4<ValueType> mat4<ValueType>::ortho_matrix ( ValueType l, ValueType r, ValueType b, ValueType t, ValueType n, ValueType f )
{
    return mat4<ValueType> ( 2/(r-l),           0,         0, (r+l)/(r-l),
                                   0,      2/(t-b),        0, (t+b)/(t-b),
                                   0,           0,   2/(n-f), (f+n)/(f-n),
			           0,           0,         0,           1);
}

template < class ValueType >
mat4<ValueType> mat4<ValueType>::translate_inv_matrix ( float x, float y, float z )
{
    return mat4<ValueType> ( 1,  0,  0,  -dx, 
                             0,  1,  0,  -dy, 
                             0,  0,  1,  -dz,
                             0,  0,  0,  1);
}

template < class ValueType >
mat4<ValueType> mat4<ValueType>::rotate_inv_matrix ( float angle, int axis )
{
    float alpha = -angle * M_PI / 180;
    switch ( axis ) {
    case 0:
        return mat4<ValueType> ( 1, 0,          0,           0,
                                 0, cos(alpha), -sin(alpha), 0,
                                 0, sin(alpha), cos(alpha),  0, 
                                 0, 0,          0,           1 );
    case 1:
        return mat4<ValueType> ( cos(alpha),  0, sin(alpha),  0,
                                 0,           1, 0,           0,
                                 -sin(alpha), 0, cos(alpha),  0, 
                                 0,           0, 0,           1 );
    case 2:
        return mat4<ValueType> ( cos(alpha),  -sin(alpha), 0, 0,
                                 sin(alpha),  cos(alpha),  0, 0,
                                 0,           0,           1, 0, 
                                 0,           0,           0, 1 );
    }
}

template < class ValueType >
mat4<ValueType> mat4<ValueType>::scale_inv_matrix ( ValueType sx, ValueType sy, ValueType sz )
{
    return mat4<ValueType> ( 1/sx,    0,     0,    0, 
                             0,    1/sy,     0,    0, 
                             0,       0,  1/sz,    0,
                             0,       0,     0,    1);
}

template < class ValueType >
mat4<ValueType> mat4<ValueType>::frustum_inv_matrix ( ValueType l, ValueType r, ValueType b, ValueType t, ValueType n, ValueType f )
{
    return mat4<ValueType> ( (r-l)/(n*2),           0,    	     0,        (r+l)/(2*n), 
                                       0, (r-b)/(n*2),    	     0,        (t+b)/(2*n),
                                       0,           0,    	     0,                 -1,
                                       0,           0,   (n-f)/(2*f*n),      (n+f)/(2*f*n));
}

template < class ValueType >
mat4<ValueType> mat4<ValueType>::ortho_inv_matrix ( ValueType l, ValueType r, ValueType b, ValueType t, ValueType n, ValueType f )
{
    return mat4<ValueType> ( (r-l)/2,           0,         0,     (r+l)/2,
                                   0,     (t-b)/2,         0,     (t+b)/2,
                                   0,           0,   (n-f)/2,     (f+n)/2,
			           0,           0,         0,           1);
}


template < class ValueType >
inline mat4<ValueType> mat4<ValueType>::operator* ( const_reference rhs ) const
{
    return mat4<ValueType> ( m[0][0]*rhs.m[0][0] + m[0][1]*rhs.m[1][0] + m[0][2]*rhs.m[2][0] + m[0][3]*rhs.m[3][0], 
                             m[0][0]*rhs.m[0][1] + m[0][1]*rhs.m[1][1] + m[0][2]*rhs.m[2][1] + m[0][3]*rhs.m[3][1],
                             m[0][0]*rhs.m[0][2] + m[0][1]*rhs.m[1][2] + m[0][2]*rhs.m[2][2] + m[0][3]*rhs.m[3][2],
                             m[0][0]*rhs.m[0][3] + m[0][1]*rhs.m[1][3] + m[0][2]*rhs.m[2][3] + m[0][3]*rhs.m[3][3],

                             m[1][0]*rhs.m[0][0] + m[1][1]*rhs.m[1][0] + m[1][2]*rhs.m[2][0] + m[1][3]*rhs.m[3][0], 
                             m[1][0]*rhs.m[0][1] + m[1][1]*rhs.m[1][1] + m[1][2]*rhs.m[2][1] + m[1][3]*rhs.m[3][1],
                             m[1][0]*rhs.m[0][2] + m[1][1]*rhs.m[1][2] + m[1][2]*rhs.m[2][2] + m[1][3]*rhs.m[3][2],
                             m[1][0]*rhs.m[0][3] + m[1][1]*rhs.m[1][3] + m[1][2]*rhs.m[2][3] + m[1][3]*rhs.m[3][3],

                             m[2][0]*rhs.m[0][0] + m[2][1]*rhs.m[1][0] + m[2][2]*rhs.m[2][0] + m[2][3]*rhs.m[3][0], 
                             m[2][0]*rhs.m[0][1] + m[2][1]*rhs.m[1][1] + m[2][2]*rhs.m[2][1] + m[2][3]*rhs.m[3][1],
                             m[2][0]*rhs.m[0][2] + m[2][1]*rhs.m[1][2] + m[2][2]*rhs.m[2][2] + m[2][3]*rhs.m[3][2],
                             m[2][0]*rhs.m[0][3] + m[2][1]*rhs.m[1][3] + m[2][2]*rhs.m[2][3] + m[2][3]*rhs.m[3][3],

                             m[3][0]*rhs.m[0][0] + m[3][1]*rhs.m[1][0] + m[3][2]*rhs.m[2][0] + m[3][3]*rhs.m[3][0], 
                             m[3][0]*rhs.m[0][1] + m[3][1]*rhs.m[1][1] + m[3][2]*rhs.m[2][1] + m[3][3]*rhs.m[3][1],
                             m[3][0]*rhs.m[0][2] + m[3][1]*rhs.m[1][2] + m[3][2]*rhs.m[2][2] + m[3][3]*rhs.m[3][2],
                             m[3][0]*rhs.m[0][3] + m[3][1]*rhs.m[1][3] + m[3][2]*rhs.m[2][3] + m[3][3]*rhs.m[3][3]
                             );		    
}

template < class ValueType >
inline const mat4<ValueType>& mat4<ValueType>::operator*= ( const_reference rhs ) const
{
    *this = *this * rhs;
    return *this;
}

template < class ValueType >
inline const mat4<ValueType>& mat4<ValueType>::operator= ( const_reference rhs ) const
{
    memcpy ( (void*)m, (void*)(rhs.m), sizeof(ValueType) * 16 );
    return *this;
}

// template < class ValueType >
// inline mat4<ValueType> mat4<ValueType>::operator* ( const_reference rhs )
// {
//     //    typedef mat4<ValueType> self;
//     return self ( m00()*rhs.m00()+m01()*rhs.m10()+m02()*rhs.m20()+m03()*rhs.m30(), 
//                   m00()*rhs.m01()+m01()*rhs.m11()+m02()*rhs.m21()+m03()*rhs.m31(),
//                   m00()*rhs.m02()+m01()*rhs.m12()+m02()*rhs.m22()+m03()*rhs.m32(),
//                   m00()*rhs.m03()+m01()*rhs.m13()+m02()*rhs.m23()+m03()*rhs.m33(),

//                   m10()*rhs.m00()+m11()*rhs.m10()+m12()*rhs.m20()+m13()*rhs.m30(), 
//                   m10()*rhs.m01()+m11()*rhs.m11()+m12()*rhs.m21()+m13()*rhs.m31(),
//                   m10()*rhs.m02()+m11()*rhs.m12()+m12()*rhs.m22()+m13()*rhs.m32(),
//                   m10()*rhs.m03()+m11()*rhs.m13()+m12()*rhs.m23()+m13()*rhs.m33(),

//                   m20()*rhs.m00()+m21()*rhs.m10()+m22()*rhs.m20()+m23()*rhs.m30(), 
//                   m20()*rhs.m01()+m21()*rhs.m11()+m22()*rhs.m21()+m23()*rhs.m31(),
//                   m20()*rhs.m02()+m21()*rhs.m12()+m22()*rhs.m22()+m23()*rhs.m32(),
//                   m20()*rhs.m03()+m21()*rhs.m13()+m22()*rhs.m23()+m23()*rhs.m33(),

//                   m30()*rhs.m00()+m31()*rhs.m10()+m32()*rhs.m20()+m33()*rhs.m30(), 
//                   m30()*rhs.m01()+m31()*rhs.m11()+m32()*rhs.m21()+m33()*rhs.m31(),
//                   m30()*rhs.m02()+m31()*rhs.m12()+m32()*rhs.m22()+m33()*rhs.m32(),
//                   m30()*rhs.m03()+m31()*rhs.m13()+m32()*rhs.m23()+m33()*rhs.m33() );
// }

// template < class ValueType >
// template <class Vec4>
// inline Vec4 mat4<ValueType>::operator* ( Vec4::const_reference rhs )
// {
//     return Vec4 ( m00()*rhs.x()+m01()*rhs.y()+m02()*rhs.z()+m03()*rhs.w(), 
//                   m10()*rhs.x()+m11()*rhs.y()+m12()*rhs.z()+m13()*rhs.w(), 
//                   m20()*rhs.x()+m21()*rhs.y()+m22()*rhs.z()+m23()*rhs.w(), 
//                   m30()*rhs.x()+m31()*rhs.y()+m32()*rhs.z()+m33()*rhs.w());

// }

typedef mat4<float> mat4f;
typedef mat4<double> mat4d;
typedef mat4<int> mat4i;
typedef mat4<short> mat4s;
}
#endif
