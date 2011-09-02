#include <iostream>
#include <sstream>
#include <ctime>
using namespace std;
#include "sgr_nodes.h"
#include "sgr_lc.h"
#include "sgr_node2lc.h"
#include "sgr_lcreport.h"

/**
 * --builddata (-b)
 * --file geos.slc ( -f slcFileName )
 * --runflow (-r)
 * --help (-h)
 */
int main ( int argc, char* argv[] )
{
    if ( argc != 2 )
    {
	cout << "usage : " << argv[0] << " slcFileName" << endl;
	cout << argv[0] << " will generate a slc file & idx files" << endl;
	return 0;
    }
    list<SLCNode*> nodes;
    SLCSceneNode scene ("test_scene");

    SLCMaterial* mat_layer = new SLCMaterial ("layer_material");
    mat_layer->foreground_color = vec3i(155, 0, 0);
    mat_layer->background_color = vec3i(0, 0, 155);
    mat_layer->linetype = SLCMaterial::LINETYPE_SOLID;
    mat_layer->linewidth = 0;
    SLCMaterial* mat = new SLCMaterial ( "mat" );
    mat->foreground_color = vec3i(155, 0, 0);
    mat->background_color = vec3i(0, 44, 155);
    mat->linetype = SLCMaterial::LINETYPE_DASH;
    mat->linewidth = 1;
    SLCLayerNode* layer = new SLCLayerNode ( "layer1", mat_layer );
    SLCLODNode* lod = new SLCLODNode();
    SLCLODPageNode* lodpage = new SLCLODPageNode();
    lodpage->delayloading = false;
    lodpage->kdtree = "./lodpage1.idx";
    lodpage->imposter = true;
    int x = 0;
    int y = 0;
    for ( int i=0; i<20e4; i++ )
    {
	SLCMaterial* tmpMaterial;
	if ( i%2 == 0 )
	    tmpMaterial = mat_layer;
	else
	    tmpMaterial = mat;

	SLCRectNode* rc1 = new SLCRectNode ( tmpMaterial );
	rc1->pnts[0] = vec2f (x,   y  );
	rc1->pnts[1] = vec2f (x+1, y  );
	rc1->pnts[2] = vec2f (x+1, y+1);
	rc1->pnts[3] = vec2f (x,   y+1);
	nodes.push_back ( rc1 );
	lodpage->addChild ( rc1 );
	x += 1;
	if ( (i%9==0) && (i != 0) ) {
	    y += 1;
	    x = 0;
	}
    }

    scene.addChild ( mat_layer );
    scene.addChild ( mat );
    scene.addChild ( layer );

    layer->addChild ( lod );
    lod->addChild ( lodpage );

    nodes.push_back ( mat_layer );
    nodes.push_back ( mat );
    nodes.push_back ( layer );
    nodes.push_back ( lod );
    nodes.push_back ( lodpage );

//     string t = scene.toXML();
//     ofstream o1;
//     o1.open ( "scene.xml" );
//     o1 << t;
//     o1.close ();
    // ********************************************************************************
    SLCNode2LC node2lc ( &scene );
    node2lc.convert ( argv[1] );

    for ( list<SLCNode*>::iterator pp=nodes.begin(); pp!=nodes.end(); ++pp )
        delete *pp;
    
    LC nlc;
    clock_t t = clock();
    nlc.load ( "geos.slc" );
    cout << "parse finished, elapse " << clock() - t << "(ms), kdtreesize = " << nlc.kdtrees.size() << endl;

    t = clock();
    LCReport rpt ( nlc );
    rpt.printCounter ();
    cout << "traverse finished, elapse " << clock() - t << "(ms)" << endl;

    t = clock();
    nlc.free();
    cout << "free LC finished, elapse " << clock() - t << "(ms)" << endl;
    
    return 0;
}
