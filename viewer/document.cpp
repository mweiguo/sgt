#include "document.h"
#include <sgr_render2d.h>

Document::Document()
{
    sceneid = -1;
}

void Document::openScene ( const char* filename )
{
    sceneid = r2d_load_scene ( filename );
}

void Document::closeScene ()
{
    if ( sceneid != -1 )
    {
	r2d_unload_scene ( sceneid );
	sceneid = -1;
    }
}
