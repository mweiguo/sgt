#include "sgr_bboxupdater.h"
#include "sgr_vfculler.h"
#include "sgr_lcreport.h"

#include <iostream>
#include <ctime>
using namespace std;
int main ( int argc, char* argv[] )
{
    if ( argc != 2) {
        cout << "usage : " << argv[0] << " slcFileName" << endl;
        return 0;
    }
    LC lc;
    clock_t t = clock();
    lc.load ( argv[1] );
    cout << "load LC ok, elapse " << clock() - t << "(ms)" << endl;

    t = clock();
    vfculler vfc;
    vec2f min = vec2f ( -1, -1 );
    vec2f max = vec2f ( 100, 1000 );
    lc.toElement ( ROOT );
    vfc.cull ( lc, BBox2d(min, max) );
    cout << "view frustum culling ok, elapse " << clock() - t << "(ms), render object count = " << vfc.renderObjects.size() << endl;
//     for ( list<int>::iterator pp=vfc.renderObjects.begin(); pp!=vfc.renderObjects.end(); ++pp )
//         cout << *pp << endl;
//     cout << endl;

    LCReport rpt ( lc );
    rpt.printCounter();

    t = clock();
    lc.free ();
    cout << "free LC ok, elapse " << clock() - t << "(ms)" << endl;
    return 0;
}
