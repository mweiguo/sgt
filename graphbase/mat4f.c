#include "mat4f.h"
#include <string.h>
#include <math.h>
void mat_loadidentity ( SGRFLOAT* m )
{
    memset ( m, 0, sizeof(SGRFLOAT)*16 );
    m[0] = m[5] = m[10] = m[15] = 1;
}

void mat_multmatrix   ( /*out*/SGRFLOAT* m, SGRFLOAT* rhs16 )
{
    SGRFLOAT tmp[16];
    tmp[0]  = m[0]*rhs16[0]  + m[4]*rhs16[1]  + m[8] *rhs16[2]  + m[12]*rhs16[3];
    tmp[4]  = m[0]*rhs16[4]  + m[4]*rhs16[5]  + m[8] *rhs16[6]  + m[12]*rhs16[7];
    tmp[8]  = m[0]*rhs16[8]  + m[4]*rhs16[9]  + m[8] *rhs16[10] + m[12]*rhs16[11];
    tmp[12] = m[0]*rhs16[12] + m[4]*rhs16[13] + m[8] *rhs16[14] + m[12]*rhs16[15];
    tmp[1]  = m[1]*rhs16[0]  + m[5]*rhs16[1]  + m[9] *rhs16[2]  + m[13]*rhs16[3];
    tmp[5]  = m[1]*rhs16[4]  + m[5]*rhs16[5]  + m[9] *rhs16[6]  + m[13]*rhs16[7];
    tmp[9]  = m[1]*rhs16[8]  + m[5]*rhs16[9]  + m[9] *rhs16[10] + m[13]*rhs16[11];
    tmp[13] = m[1]*rhs16[12] + m[5]*rhs16[13] + m[9] *rhs16[14] + m[13]*rhs16[15];
    tmp[2]  = m[2]*rhs16[0]  + m[6]*rhs16[1]  + m[10]*rhs16[2]  + m[14]*rhs16[3];
    tmp[6]  = m[2]*rhs16[4]  + m[6]*rhs16[5]  + m[10]*rhs16[6]  + m[14]*rhs16[7];
    tmp[10] = m[2]*rhs16[8]  + m[6]*rhs16[9]  + m[10]*rhs16[10] + m[14]*rhs16[11];
    tmp[14] = m[2]*rhs16[12] + m[6]*rhs16[13] + m[10]*rhs16[14] + m[14]*rhs16[15];
    tmp[3]  = m[3]*rhs16[0]  + m[7]*rhs16[1]  + m[11]*rhs16[2]  + m[15]*rhs16[3];
    tmp[7]  = m[3]*rhs16[4]  + m[7]*rhs16[5]  + m[11]*rhs16[6]  + m[15]*rhs16[7];
    tmp[11] = m[3]*rhs16[8]  + m[7]*rhs16[9]  + m[11]*rhs16[10] + m[15]*rhs16[11];
    tmp[15] = m[3]*rhs16[12] + m[7]*rhs16[13] + m[11]*rhs16[14] + m[15]*rhs16[15];

    memcpy ( m, tmp, sizeof(SGRFLOAT)*16 );
}

void mat_multvector   ( SGRFLOAT* m, SGRFLOAT* rhs4  /*out*/ )
{
    SGRFLOAT t  = m[0]*rhs4[0] + m[4]*rhs4[1] + m[8] *rhs4[2] + m[12]*rhs4[3];
    SGRFLOAT t1 = m[1]*rhs4[0] + m[5]*rhs4[1] + m[9] *rhs4[2] + m[13]*rhs4[3];
    SGRFLOAT t2 = m[2]*rhs4[0] + m[6]*rhs4[1] + m[10]*rhs4[2] + m[14]*rhs4[3];
    SGRFLOAT t3 = m[3]*rhs4[0] + m[7]*rhs4[1] + m[11]*rhs4[2] + m[15]*rhs4[3];
    rhs4[0] = t;
    rhs4[1] = t1;
    rhs4[2] = t2;
    rhs4[3] = t3;
}

void mat_translatematrix ( SGRFLOAT* m, SGRFLOAT x, SGRFLOAT y, SGRFLOAT z )
{
    m[12] = x;
    m[13] = y;
    m[14] = z;
}

void mat_scalematrix ( SGRFLOAT* m, SGRFLOAT sx, SGRFLOAT sy, SGRFLOAT sz )
{
    m[0] = sx;
    m[5] = sy;
    m[10] = sz;
}

/**
 * angle expressed in radians
 */
void mat_rotatematrix ( SGRFLOAT* m, SGRFLOAT angle, int axis )
{
//    float alpha = angle * M_PI / 180;
    switch ( axis ) {
    case 0:
	m[5]  = cos(angle);
	m[6]  = sin(angle);
	m[9]  = -sin(angle);
	m[10] = cos(angle);
	break;
    case 1:
	m[0]  = cos(angle);
	m[2]  = -sin(angle);
	m[8]  = sin(angle);
	m[10] = cos(angle);
	break;
    case 2:
	m[0] = cos(angle);
	m[1] = sin(angle);
	m[4] = -sin(angle);
	m[5] = cos(angle);
	break;
    }

}
