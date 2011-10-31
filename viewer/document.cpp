#include "document.h"
#include "glwidget.h"

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

void Document::init ( GLWidget* widget, QGLWidget* birdwidget )
{
	widget->makeCurrent();
	if ( (miscsceneid = r2d_load_scene ( "miscscene.slc" )) == -1 )
		cerr << "load miscscene.slc failed" << endl;
	else {
		//sharewidget->makeCurrent();
		//r2d_continue_load_resources ( miscsceneid );
		cout << "load miscscene.slc succeeded" << endl;
	}

	birdwidget->makeCurrent();
	if ( (birdviewmiscid = r2d_load_scene ( "birdviewmisc.slc" )) == -1 )
		cerr << "load birdviewmisc.slc failed" << endl;
	else {
		//sharewidget->makeCurrent();
		//r2d_continue_load_resources ( birdviewmiscid );
		cerr << "load birdviewmisc.slc succeeded" << endl;
	}
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

void Document::openScene ( GLWidget* widget, const char* filename )
{
    widget->makeCurrent ();

    clock_t t = clock();
    closeScene();
    if ( (sceneid = r2d_load_scene ( filename )) != -1 ) {
//        r2d_continue_load_resources ( sceneid );
        cout << "load " << filename << " finished, ellapse " << clock() - t << endl;
    } else {
        cerr << "load " << filename << " failed, ellapse " << clock() - t << endl;
    }
}

void Document::closeScene ()
{
    if ( sceneid != -1 )
    {
        r2d_unload_scene ( sceneid );
        sceneid = -1;
    }
}

