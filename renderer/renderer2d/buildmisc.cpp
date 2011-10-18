#include <sgr_node2lc.h>
#include <sgr_nodes.h>
#include "sgr_lcreport.h"
#include "mat4f.h"
#include <fstream>
#include <ctime>

list<SLCNode*> nodes;
SLCMaterial *gmat;

// --------------------------------------------------------------------------------

void initMaterials ()
{
    gmat = new SLCMaterial ( "layer_material");
    gmat->foreground_color = vec4i(155, 0, 0, 155);
    gmat->background_color = vec4i(0, 0, 200, 115);
//     gmat->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
//     gmat->linewidth = 1;
//     gmat->fontfilename = "";
//     gmat->texturefilename = "";
    nodes.push_back ( gmat );
}

int main ( int argc, char* argv[] )
{
    if ( argc != 2 )
    {
	cout << "usage : " << argv[0] << " SLCFileName" << endl;
	cout << argv[0] << " will generate a slc file & idx files" << endl;
	return 0;
    }
    initLC();
    initMaterials();

//    list<SLCNode*> nodes;
    SLCSceneNode scene ( "misc_scene");
    scene.addChild ( gmat );

    {
	SLCLayerNode* layer = new SLCLayerNode ( "background", gmat );
	nodes.push_back ( layer );
	scene.addChild ( layer );
	SLCLODNode* lod = new SLCLODNode();
	nodes.push_back ( lod );
	layer->addChild ( lod );
	SLCLODPageNode* lodpage = new SLCLODPageNode();
	lodpage->delayloading = false;
//	lodpage->kdtree = "misc_background0.idx";
	lodpage->imposter = true;
	nodes.push_back ( lodpage );
	lod->addChild ( lodpage );

	SLCRectNode* rc1 = new SLCRectNode ( gmat );
	nodes.push_back ( rc1 );
	rc1->pnts[0] = vec2f ( 0, 0 );
	rc1->pnts[1] = vec2f ( 0, 0 );
	rc1->pnts[2] = vec2f ( 0, 0 );
	rc1->pnts[3] = vec2f ( 0, 0 );
	rc1->z = 100;
	lodpage->addChild ( rc1 );
    }

    ofstream o;
    o.open ("base.xml" );
    o << scene.toXML();
    o.close();

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

