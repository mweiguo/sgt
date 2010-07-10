#ifndef __VEC_UNITTEST_H_
#define __VEC_UNITTEST_H_

#include "vec3.h"
#include <stdexcept>

using namespace std;
class vec3Test : public vec3<float>
{
public:
  typedef vec3<float> vec3f;
  typedef vec2<float> vec2f;

  vec3Test (float x1, float y1, float z1) : vec3f(x1,y1,z1)
  {
    modTest ();
    addTest ();
    subTest ();
    divTest ();
    crossTest();
    getterTest();
  }
  void modTest ()
  {
    if ( sqrt(x() * x() + y() * y() + z() * z()) != mod() )
      throw logic_error ( "modTest failed" );
  }
  void addTest ()
  {
    vec3f t ( 1, 2, 3 );
    vec3f t2( 4, 5, 6 );
    vec3f t1 = t + t2;
    if ( t1.x() != 5 || t1.y() != 7 || t1.z() != 9 )
      throw logic_error ( "addTest failed" );
  }
  void subTest ()
  {
    vec3f t ( x()+1, y()+1, z()+1 );
    vec3f t1 = t - *this;
    if ( t1.x() != 1 || t1.y() != 1 || t1.z() != 1 )
      throw logic_error ( "subTest failed" );
  }
  void divTest ()
  {
    vec3f t ( 2, 3, 4 );
    vec3f t1 = t / 2;
    if ( t1.x() != 1 || t1.y() != 1.5 || t1.z() != 2 )
      throw logic_error ( "divTest failed" );
  }
  void crossTest()
  {
    vec3f t ( x()+1, y()+2, z()+3 );
    vec3f t1 = this->cross ( t );

    if ( t1.x() != y() * t.z() - z() * t.y() ||
	 t1.y() != z() * t.x() - x() * t.z() ||
	 t1.z() != x() * t.y() - y() * t.x() )
      throw logic_error ( "crossTest failed" );
  }
  void getterTest()
  {
    vec3f v ( 1, 2, 3 );
    {
      vec3f t = v.xxx();
      if ( t.x() != 1 || t.y() != 1 || t.z() != 1 )
	throw logic_error ( "xxx() failed" );

      t = v.xxy();
      if ( t.x() != 1 || t.y() != 1 || t.z() != 2 )
	throw logic_error ( "xxy() failed" );

      t = v.xxz();
      if ( t.x() != 1 || t.y() != 1 || t.z() != 3 )
	throw logic_error ( "xxz() failed" );

      t = v.xyx();
      if ( t.x() != 1 || t.y() != 2 || t.z() != 1 )
	throw logic_error ( "xyx() failed" );

      t = v.xyy();
      if ( t.x() != 1 || t.y() != 2 || t.z() != 2 )
	throw logic_error ( "xyy() failed" );

      t = v.xyz();
      if ( t.x() != 1 || t.y() != 2 || t.z() != 3 )
	throw logic_error ( "xyz() failed" );

      t = v.xzx();
      if ( t.x() != 1 || t.y() != 3 || t.z() != 1 )
	throw logic_error ( "xzx() failed" );

      t = v.xzy();
      if ( t.x() != 1 || t.y() != 3 || t.z() != 2 )
	throw logic_error ( "xzy() failed" );

      t = v.xzz();
      if ( t.x() != 1 || t.y() != 3 || t.z() != 3 )
	throw logic_error ( "xzz() failed" );
    }
    {
      vec2f t = v.xx();
      if ( t.x() != 1 || t.y() != 1 )
	throw logic_error ( "xx() failed" );

      t = v.xy();
      if ( t.x() != 1 || t.y() != 2 )
	throw logic_error ( "xy() failed" );

      t = v.xz();
      if ( t.x() != 1 || t.y() != 3 )
	throw logic_error ( "xz() failed" );

      t = v.yx();
      if ( t.x() != 2 || t.y() != 1 )
	throw logic_error ( "yx() failed" );

      t = v.yy();
      if ( t.x() != 2 || t.y() != 2 )
	throw logic_error ( "yy() failed" );

      t = v.yz();
      if ( t.x() != 2 || t.y() != 3 )
	throw logic_error ( "yz() failed" );

      t = v.zx();
      if ( t.x() != 3 || t.y() != 1 )
	throw logic_error ( "zx() failed" );

      t = v.zy();
      if ( t.x() != 3 || t.y() != 2 )
	throw logic_error ( "zy() failed" );

      t = v.zz();
      if ( t.x() != 3 || t.y() != 3 )
	throw logic_error ( "zzb() failed" );
    }
  }
};

#endif
