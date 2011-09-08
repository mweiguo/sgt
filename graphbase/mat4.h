#ifndef _MATRIX_4_H_
#define _MATRIX_4_H_
#include <cstring>

// column-major
template < class T >
class Mat4
{
public:
    Mat4 ();
    Mat4 ( T t0, T t1, T t2, T t3, T t4, T t5, T t6, T t7, T t8, T t9, T t10, T t11, T t12, T t13, T t14, T t15 );
    Mat4 ( T* v );
    void loadIdentity ();
    Mat4<T>& operator *= ( const Mat4<T>& rhs );

    static Mat4<T> translate_matrix ( T x, T y, T z );
    static Mat4<T> scale_matrix ( T sx, T sy, T sz );
    
    T data[16];
};

//============================================================

template < class T >
Mat4<T>::Mat4 ()
{
    memset ( data, 0, sizeof(float)*16 );
    data[0] = data[5] = data[10] = data[15] = 0;
}

//============================================================

template < class T >
Mat4<T>::Mat4 ( T t0, T t1, T t2, T t3, T t4, T t5, T t6, T t7, T t8, T t9, T t10, T t11, T t12, T t13, T t14, T t15 )
{
    data[0]  = t0;
    data[1]  = t1;
    data[2]  = t2;
    data[3]  = t3;
    data[4]  = t4;
    data[5]  = t5;
    data[6]  = t6;
    data[7]  = t7;
    data[8]  = t8;
    data[9]  = t9;
    data[10] = t10;
    data[11] = t11;
    data[12] = t12;
    data[13] = t13;
    data[14] = t14;
    data[15] = t15;
}

//============================================================

template < class T >
Mat4<T>::Mat4 ( T* v )
{
    memcpy ( data, v, sizeof(float)*16 );
}

//============================================================

template < class T >
void Mat4<T>::loadIdentity ()
{
    memset ( data, 0, sizeof(float)*16 );
    data[0] = data[5] = data[10] = data[15] = 1;
}

//============================================================

template < class T >
Mat4<T>& Mat4<T>::operator *= ( const Mat4<T>& rhs )
{
    float tmp[16];
    tmp[0]  = data[0]*rhs.data[0]  + data[4]*rhs.data[1]  + data[8]*rhs.data[2]  + data[12]*rhs.data[3];
    tmp[4]  = data[0]*rhs.data[4]  + data[4]*rhs.data[5]  + data[8]*rhs.data[6]  + data[12]*rhs.data[7];
    tmp[8]  = data[0]*rhs.data[8]  + data[4]*rhs.data[9]  + data[8]*rhs.data[10] + data[12]*rhs.data[11];
    tmp[12] = data[0]*rhs.data[12] + data[4]*rhs.data[13] + data[8]*rhs.data[14] + data[12]*rhs.data[15];

    tmp[1]  = data[1]*rhs.data[0]  + data[5]*rhs.data[1]  + data[9]*rhs.data[2]  + data[13]*rhs.data[3];
    tmp[5]  = data[1]*rhs.data[4]  + data[5]*rhs.data[5]  + data[9]*rhs.data[6]  + data[13]*rhs.data[7];
    tmp[9]  = data[1]*rhs.data[8]  + data[5]*rhs.data[9]  + data[9]*rhs.data[10] + data[13]*rhs.data[11];
    tmp[13] = data[1]*rhs.data[12] + data[5]*rhs.data[13] + data[9]*rhs.data[14] + data[13]*rhs.data[15];

    tmp[2]  = data[2]*rhs.data[0]  + data[6]*rhs.data[1]  + data[10]*rhs.data[2]  + data[14]*rhs.data[3];
    tmp[6]  = data[2]*rhs.data[4]  + data[6]*rhs.data[5]  + data[10]*rhs.data[6]  + data[14]*rhs.data[7];
    tmp[10] = data[2]*rhs.data[8]  + data[6]*rhs.data[9]  + data[10]*rhs.data[10] + data[14]*rhs.data[11];
    tmp[14] = data[2]*rhs.data[12] + data[6]*rhs.data[13] + data[10]*rhs.data[14] + data[14]*rhs.data[15];

    tmp[3]  = data[3]*rhs.data[0]  + data[7]*rhs.data[1]  + data[11]*rhs.data[2]  + data[15]*rhs.data[3];
    tmp[7]  = data[3]*rhs.data[4]  + data[7]*rhs.data[5]  + data[11]*rhs.data[6]  + data[15]*rhs.data[7];
    tmp[11] = data[3]*rhs.data[8]  + data[7]*rhs.data[9]  + data[11]*rhs.data[10] + data[15]*rhs.data[11];
    tmp[15] = data[3]*rhs.data[12] + data[7]*rhs.data[13] + data[11]*rhs.data[14] + data[15]*rhs.data[15];

    memcpy ( data, tmp, sizeof(T)*16 );
    return *this;
}

//============================================================

template < class T >
Mat4<T> Mat4<T>::translate_matrix ( T dx, T dy, T dz )
{
    return Mat4<T> (   1,   0,   0,  0, 
                       0,   1,   0,  0, 
                       0,   0,   1,  0,
                      dx,  dy,  dz,  1);
    
}

//============================================================

template < class T >
Mat4<T> Mat4<T>::scale_matrix ( T sx, T sy, T sz )
{
    return Mat4<T> ( sx, 0,  0,  0, 
                     0,  sy, 0,  0, 
                     0,  0,  sz, 0,
                     0,  0,  0,  1);
}

//============================================================

typedef Mat4<float> mat4f;

#endif // _MATRIX_4_H_
