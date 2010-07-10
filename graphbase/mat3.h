#ifndef _MAT3_H_
#define _MAT3_H_

#define _USE_MATH_DEFINES
#include <math.h>

template < class ValueType >
class mat3
{
public:
  typedef mat3 self;
  typedef const mat3& const_reference;

  mat3 ( ValueType a00=0, ValueType a01=0, ValueType a02=0, 
	 ValueType a10=0, ValueType a11=0, ValueType a12=0, 
	 ValueType a20=0, ValueType a21=0, ValueType a22=0 ) {
    m[0][0] = a00; m[0][1] = a01; m[0][2] = a02;
    m[1][0] = a10; m[1][1] = a11; m[1][2] = a12;
    m[2][0] = a20; m[2][1] = a21; m[2][2] = a22;
  }
  mat3 operator* ( const_reference rhs );

  template < class Vec >
  Vec operator * ( const Vec& rhs ) const
  {
    typename Vec::value_type t  = m[0][0]*rhs.x() + m[0][1]*rhs.y() + m[0][2]*rhs.z();
    typename Vec::value_type t1 = m[1][0]*rhs.x() + m[1][1]*rhs.y() + m[1][2]*rhs.z();
    typename Vec::value_type t2 = m[2][0]*rhs.x() + m[2][1]*rhs.y() + m[2][2]*rhs.z();
    return Vec ( t, t1, t2 );
  }

  void dx ( ValueType v ) { m[2][0] = v; }
  void dy ( ValueType v ) { m[2][1] = v; }
  void sx ( ValueType v ) { m[0][0] = v; }
  void sy ( ValueType v ) { m[1][1] = v; }

  ValueType dx () { return m[2][0]; }
  ValueType dy () { return m[2][1]; }
  ValueType sx () { return m[0][0]; }
  ValueType sy () { return m[1][1]; }
  ValueType m00 () { return m[0][0]; }
  ValueType m01 () { return m[0][1]; }
  ValueType m02 () { return m[0][2]; }
  ValueType m10 () { return m[1][0]; }
  ValueType m11 () { return m[1][1]; }
  ValueType m12 () { return m[1][2]; }
  ValueType m20 () { return m[2][0]; }
  ValueType m21 () { return m[2][1]; }
  ValueType m22 () { return m[2][2]; }

  void normal () {
    m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;
    m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;
    m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;
  }
  static mat3 rotate_matrix ( float angle );
  static mat3 scale_matrix ( ValueType sx, ValueType sy );
private:
  ValueType m[3][3];
};

template < class ValueType >
mat3<ValueType> mat3<ValueType>::rotate_matrix ( float angle )
{
  
  float alpha = angle * M_PI / 180;
  return mat3<ValueType> ( cos(alpha), -sin(alpha), 0,
			   sin(alpha), cos(alpha), 0, 
			   0, 0, 1 );
}

template < class ValueType >
mat3<ValueType> mat3<ValueType>::scale_matrix ( ValueType sx, ValueType sy )
{
  return mat3<ValueType> ( sx, 0, 0, 0, sy, 0, 0, 0, 1 );
}

template < class ValueType >
inline mat3<ValueType> mat3<ValueType>::operator* ( const_reference rhs )
{
  return mat3<ValueType> ( m[0][0]*rhs.m[0][0] + m[0][1]*rhs.m[1][0] + m[0][2]*rhs.m[2][0], 
			   m[0][0]*rhs.m[0][1] + m[0][1]*rhs.m[1][1] + m[0][2]*rhs.m[2][1],
			   m[0][0]*rhs.m[0][2] + m[0][1]*rhs.m[1][2] + m[0][2]*rhs.m[2][2],
			   m[1][0]*rhs.m[0][0] + m[1][1]*rhs.m[1][0] + m[1][2]*rhs.m[2][0], 
			   m[1][0]*rhs.m[0][1] + m[1][1]*rhs.m[1][1] + m[1][1]*rhs.m[2][1],
			   m[1][0]*rhs.m[0][2] + m[1][1]*rhs.m[1][2] + m[1][1]*rhs.m[2][2],
			   m[2][0]*rhs.m[0][0] + m[2][1]*rhs.m[1][0] + m[2][2]*rhs.m[2][0], 
			   m[2][0]*rhs.m[0][1] + m[2][1]*rhs.m[1][1] + m[2][1]*rhs.m[2][1],
			   m[2][0]*rhs.m[0][2] + m[2][1]*rhs.m[1][2] + m[2][1]*rhs.m[2][2]
			   );		    
}

typedef mat3<float> mat3f;
typedef mat3<double> mat3d;
typedef mat3<int> mat3i;
typedef mat3<short> mat3s;

#endif
