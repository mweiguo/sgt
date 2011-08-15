#ifndef _SST_STATE_H_
#define _SST_STATE_H_

#include <vec4.h>

#define SST_TEXTURE     0
#define SST_AMBIENT     1
#define SST_DIFFUSE     2
#define SST_SPECULAR    3
#define SST_MESH        4
#define SST_TRANSFORM   5

class SSTState
{
public:
    SSTState ( int t, float v );
    SSTState ( int t, int v );
    SSTState ( int t, float v1, float v2, float v3, float v4 );
    SSTState ( int t, float* v16 );
    int   type;
    float valuef;
    int   valuei;
    vec4f valuefv;
    float valuefm[16];

    bool operator== ( const SSTState& ss ) const;
};

#endif //  _SST_STATE_H_
