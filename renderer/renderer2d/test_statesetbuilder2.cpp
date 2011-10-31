#include <list>
#include <string>
#include <fstream>
#include <ctime>
#include "sgr_vfculler.h"
#include "sgr_lcreport.h"
#include "sgr_bboxupdater.h"
#include "sgr_statesetbuilder.h"
#include <IL/il.h>
#include <IL/ilut.h>

using namespace std;
float currentScale = 1;
int main ( int argc, char* argv[] )
{
    if ( argc != 4 )
    {
	cout << "usage : " << argv[0] << " slcFileName opaqueStateSetDump transparentStateSetDump";
	return 0;
    }

    ilInit();
    ilutRenderer(ILUT_OPENGL);
    LC lc;
    clock_t t = clock();
    lc.load ( argv[1] );
    cout << "load LC ok, elapse " << clock() - t << "(ms)" << endl;
    // update bbox
    lc.toElement ( ROOT );
    BBox2dUpdater::forward_update ( lc );
    
    t = clock();
    vfculler vfc;
    vec2f min = vec2f ( -1, -1 );
    vec2f max = vec2f (  10, 700 );
    lc.toElement ( ROOT );
    vfc.cull ( lc, BBox2d(min, max) );
    cout << "view frustum culling ok, elapse " << clock() - t << "(ms), render object count = " << vfc.renderObjects.size() << endl;

    t = clock();
    list<StateSet*> op, tr;
    StateSetBuilder2::build ( &lc, vfc.renderObjects, op, tr );
    cout << "stateset builder ok, elapse " << clock() - t << "(ms) " << endl;
    cout << "op.size() = " << op.size() << ", tr.size() = " << tr.size() << endl;
    string opxml, trxml;
    for ( list<StateSet*>::iterator pp=op.begin(); pp!=op.end(); ++pp )
	opxml += (*pp)->toXML();
    for ( list<StateSet*>::iterator pp=tr.begin(); pp!=tr.end(); ++pp )
	trxml += (*pp)->toXML();
    
    ofstream o;
    o.open ( argv[2] );
    o << opxml;
    o.close ();

    o.open ( argv[3] );
    o << trxml;
    o.close ();

    LCReport rpt ( lc );
    rpt.printCounter();

    t = clock();
    lc.free ();
    cout << "free LC ok, elapse " << clock() - t << "(ms)" << endl;
    return 0;
}
