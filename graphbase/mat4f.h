#ifndef _MATRIX_4D_H_
#define _MATRIX_4D_H_

//extern "C" {
typedef float SGRFLOAT;
void mat_loadidentity ( SGRFLOAT* m );
void mat_multmatrix   ( /*out*/SGRFLOAT* m, SGRFLOAT* rhs16 );
void mat_multvector   ( SGRFLOAT* m, SGRFLOAT* rhs4  /*out*/ );
void mat_translatematrix ( SGRFLOAT* m, SGRFLOAT x, SGRFLOAT y, SGRFLOAT z );
void mat_scalematrix     ( SGRFLOAT* m, SGRFLOAT sx, SGRFLOAT sy, SGRFLOAT sz );
void mat_rotatematrix    ( SGRFLOAT* m, SGRFLOAT angle, int axis );
//}

#endif // _MATRIX_4D_H_
