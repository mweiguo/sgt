#include "document.h"
#include <sgr_render2d.h>
#include <ctime>
#include <iostream>
using namespace std;

Document::Document()
{
    sceneid = -1;
    miscsceneid = r2d_load_scene ( "miscscene.slc" );
}

Document::~Document ()
{
    closeScene();
    r2d_unload_scene ( miscsceneid );
    miscsceneid = -1;
}

void Document::openScene ( const char* filename )
{
    clock_t t = clock();
    sceneid = r2d_load_scene ( filename );
    cout << "load finished, ellapse " << clock() - t << endl;
}

void Document::closeScene ()
{
    if ( sceneid != -1 )
    {
	r2d_unload_scene ( sceneid );
	sceneid = -1;
    }

}

