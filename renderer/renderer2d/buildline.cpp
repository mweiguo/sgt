#include <sgr_node2lc.h>
#include <sgr_nodes.h>
#include "sgr_lcreport.h"
#include "mat4f.h"
#include <fstream>
#include <ctime>

list<SLCNode*> nodes;
SLCMaterial *gmat, *gmat1, *gmat2;

// --------------------------------------------------------------------------------

void initMaterials ()
{
    gmat = new SLCMaterial ( "material1");
    gmat->foreground_color = vec4i(155, 0, 0, 125);
    gmat->background_color = vec4i(55, 255, 255, 125);
    gmat->linetype = 0xaaaa;//SLCMaterial::LINETYPE_SOLID;
    gmat->linetypefactor = 16;
    gmat->linewidth = 0;
    gmat->fontfilename = "simsun.ttc";
    gmat->texturefilename = "hands.jpg";
    nodes.push_back ( gmat );

    gmat1 = new SLCMaterial ( "material2");
    gmat1->foreground_color = vec4i(155, 0, 0, 125);
    gmat1->background_color = vec4i(55, 255, 255, 125);
    gmat1->linetype = 0xaaaa;//SLCMaterial::LINETYPE_SOLID;
    gmat1->linetypefactor = 8;
//    gmat1->linetype = SLCMaterial::LINETYPE_SOLID;
    gmat1->linewidth = 4;
    gmat1->fontfilename = "simsun.ttc";
    gmat1->texturefilename = "hands.jpg";
    nodes.push_back ( gmat1 );

    gmat2 = new SLCMaterial ( "material3");
    gmat2->foreground_color = vec4i(155, 0, 0, 125);
    gmat2->background_color = vec4i(55, 255, 255, 125);
    gmat2->linetype = 0xaaaa;//SLCMaterial::LINETYPE_SOLID;
    gmat2->linetypefactor = 16;
//    gmat2->linetype = SLCMaterial::LINETYPE_SOLID;
    gmat2->linewidth = 10.2;
    gmat2->fontfilename = "simsun.ttc";
    gmat2->texturefilename = "hands.jpg";
    nodes.push_back ( gmat2 );
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
    scene.addChild ( gmat1 );
    scene.addChild ( gmat2 );

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

	lod->scales.push_back ( 0.2 );

	{
	    SLCPLineNode* line = new SLCPLineNode( gmat );
	    nodes.push_back ( line );
	    line->pnts.push_back ( vec2f( 0, 3) );
	    line->pnts.push_back ( vec2f( 10, 3) );
	    line->z = 1;
	    lodpage->addChild ( line );
	}

	{
	    SLCPLineNode* line = new SLCPLineNode( gmat1 );
	    nodes.push_back ( line );
	    line->pnts.push_back ( vec2f( 0, 5) );
	    line->pnts.push_back ( vec2f( 10, 5) );
	    line->z = 1;
	    lodpage->addChild ( line );
	}

	{
	    SLCPLineNode* line = new SLCPLineNode( gmat2 );
	    nodes.push_back ( line );
	    line->pnts.push_back ( vec2f( 0, 7) );
	    line->pnts.push_back ( vec2f( 10, 7) );
	    line->z = 1;
	    lodpage->addChild ( line );
	}
    }

//     ofstream o;
//     o.open ("test.xml" );
//     o << scene.toXML();
//     o.close();

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

