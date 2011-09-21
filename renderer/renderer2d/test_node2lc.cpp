#include <iostream>
#include <sstream>
#include <ctime>
using namespace std;
#include "sgr_nodes.h"
#include "sgr_lc.h"
#include "sgr_node2lc.h"
#include "sgr_lcreport.h"
#include <IL/il.h>
#include <IL/ilut.h>

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
    ilInit();
    ilutRenderer(ILUT_OPENGL);
    list<SLCNode*> nodes;
    SLCSceneNode scene ( "test_scene");

    SLCMaterial* mat_layer = new SLCMaterial ( "layer_material");
    mat_layer->foreground_color = vec3i(155, 0, 0);
    mat_layer->background_color = vec3i(155, 0, 155);
    mat_layer->linetype = SLCMaterial::LINETYPE_SOLID;
    mat_layer->linewidth = 0;
    mat_layer->fontfilename = "simsun.ttc";
    mat_layer->texturefilename = "hands.jpg";
    SLCMaterial* mat = new SLCMaterial ( "mat" );
    mat->foreground_color = vec3i(155, 144, 0);
    mat->background_color = vec3i(44, 155, 155);
    mat->linetype = SLCMaterial::LINETYPE_DASH;
    mat->linewidth = 1;
    mat->fontfilename = "simhei.ttf";
    mat->texturefilename = "hands.jpg";
    SLCLayerNode* layer = new SLCLayerNode ( "layer1", mat_layer );
    SLCLODNode* lod = new SLCLODNode();
    SLCLODPageNode* lodpage = new SLCLODPageNode();
    lodpage->delayloading = false;
    lodpage->kdtree = "lodpage1.idx";
    lodpage->imposter = true;
    int x = 0;
    int y = 0;
    for ( int i=0; i<1; i++ )
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
	x += 2;
	if ( (i%179==0) && (i != 0) ) {
	    y += 2;
	    x = 0;
	}
    }
    SLCTextNode* txt = new SLCTextNode ( mat_layer );
    txt->pos.xy ( 0, -1 );
    txt->scale = 1;
    txt->rotz = 0;
    txt->text = "`1234567890-=~!@#$%^&*()_+qwertyuiop[]\\asdfghjkl;'zxcvbnm,.QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?";
//    txt->text = "1234567890";
    nodes.push_back ( txt );
    lodpage->addChild ( txt );

    SLCPLineNode* pline = new SLCPLineNode ( mat );
    pline->pnts.push_back ( vec2f(0,-2) );
    pline->pnts.push_back ( vec2f(1,-2) );
    pline->pnts.push_back ( vec2f(1,-1) );
    pline->pnts.push_back ( vec2f(2,-1) );
    pline->pnts.push_back ( vec2f(2,-3) );
    nodes.push_back ( pline );
    lodpage->addChild ( pline );

    SLCPLineNode* pline2 = new SLCPLineNode ( mat );
    pline2->pnts.push_back ( vec2f(3,0) );
    pline2->pnts.push_back ( vec2f(4,0) );
    pline2->pnts.push_back ( vec2f(4,1) );
    pline2->pnts.push_back ( vec2f(5,1) );
    pline2->pnts.push_back ( vec2f(5,-1) );
    nodes.push_back ( pline2 );
    lodpage->addChild ( pline2 );

    SLCPolyNode* poly = new SLCPolyNode ( mat );
    poly->textureScale = 2;
    poly->textureAngle = 45;
    poly->filltexture  = true;
    poly->pnts.push_back ( vec2f(0,2) );
    poly->pnts.push_back ( vec2f(1,2) );
    poly->pnts.push_back ( vec2f(1,3) );
    poly->pnts.push_back ( vec2f(2,3) );
    poly->pnts.push_back ( vec2f(2,1) );
    nodes.push_back ( poly );
    lodpage->addChild ( poly );

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

    // ********************************************************************************
    SLCNode2LC node2lc ( &scene );
    node2lc.convert ( argv[1] );

    for ( list<SLCNode*>::iterator pp=nodes.begin(); pp!=nodes.end(); ++pp )
        delete *pp;
    
    LC nlc;
    clock_t t = clock();
    nlc.load ( argv[1] );
    cout << "parse finished, elapse " << clock() - t << "(ms), kdtreesize = " << nlc.kdtrees.size() << endl;

    t = clock();
    LCReport rpt ( nlc, 1 );
    rpt.printCounter ();
    cout << "traverse finished, elapse " << clock() - t << "(ms)" << endl;

    t = clock();
    nlc.free();
    cout << "free LC finished, elapse " << clock() - t << "(ms)" << endl;
    
    return 0;
}
