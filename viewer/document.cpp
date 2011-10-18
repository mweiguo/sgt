#include "document.h"
#include <sgr_render2d.h>
#include <ctime>
#include <iostream>
using namespace std;

Document::Document()
{
    sceneid = -1;
    miscsceneid    = -1;
    birdviewmiscid = -1;
}

void Document::init ()
{
    if ( (miscsceneid = r2d_load_scene ( "miscscene.slc" )) == -1 )
	cerr << "load miscscene.slc failed" << endl;
    else
	cout << "load miscscene.slc succeeded" << endl;
	
    if ( (birdviewmiscid = r2d_load_scene ( "birdviewmisc.slc" )) == -1 )
	cerr << "load birdviewmisc.slc failed" << endl;
    else
	cerr << "load birdviewmisc.slc succeeded" << endl;


}

Document::~Document ()
{
    closeScene();
    if ( -1 != miscsceneid )
	r2d_unload_scene ( miscsceneid );
    if ( -1 != birdviewmiscid )
	r2d_unload_scene ( birdviewmiscid );
    miscsceneid = -1;
    birdviewmiscid = -1;
}

void Document::openScene ( const char* filename )
{
    clock_t t = clock();
    sceneid = r2d_load_scene ( filename );
    cout << "load " << filename << " finished, ellapse " << clock() - t << endl;
}

void Document::closeScene ()
{
    if ( sceneid != -1 )
    {
	r2d_unload_scene ( sceneid );
	sceneid = -1;
    }

}

