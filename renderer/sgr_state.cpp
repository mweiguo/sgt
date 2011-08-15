#include "sgr_state.h"
#include <cstring>

SSTState::SSTState ( int t, float v )
{
    type = t;
    valuef = v;
}

SSTState::SSTState ( int t, int v )
{
    type = t;
    valuei = v;
}

SSTState::SSTState ( int t, float v1, float v2, float v3, float v4 )
{
    type = t;
    valuefv = vec4f ( v1, v2, v3, v4 );
}

SSTState::SSTState ( int t, float* v16 )
{
    type = t;
    memcpy ( valuefm, v16, sizeof(float) * 16 );
}

bool SSTState::operator== ( const SSTState& ss ) const
{
    if ( type == ss.type )
    {
	switch ( type )
	{
	case SST_TEXTURE:
	case SST_MESH:
	    return (valuei == ss.valuei);
	case SST_AMBIENT:
	case SST_DIFFUSE:
	case SST_SPECULAR:
	    return (valuefv == ss.valuefv);
	}
    }
    return false;
}
