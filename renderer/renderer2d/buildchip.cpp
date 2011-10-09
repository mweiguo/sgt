#include <sgr_node2lc.h>
#include <sgr_nodes.h>
#include "sgr_lcreport.h"
#include "mat4f.h"
#include <fstream>
#include <ctime>
#include <math.h>

list<SLCNode*> nodes;
SLCMaterial *gmat, *gmat1;

SLCNode* logic0_lod0 ( SLCMaterial *mat, float& width, float& height )
{
    width = 40;
    height = 300;
    SLCTransformNode* t = new SLCTransformNode ();
    nodes.push_back ( t );

    float dy=0;
    float h = 20;
    for ( int i=0; i<10; i++ )
    {
	SLCTransformNode* t1 = new SLCTransformNode ();
	mat_translatematrix ( t1->mat, 0, dy, 0 );
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (40, 0 ); rc1->pnts[2] = vec2f (40, 20); rc1->pnts[3] = vec2f ( 0, 20);
	rc1->z = -1;
	t1->addChild ( rc1 );
	t->addChild ( t1 );
	nodes.push_back ( t1 );
	nodes.push_back ( rc1 );

	if ( i == 4 )
	    dy += 2*h;
	else
	    dy += 3 * h/2;
    }
    return t;
}

SLCNode* logic_lod0 ( SLCMaterial *mat, float& width, float& height )
{
    SLCTransformNode* t = new SLCTransformNode ();
    nodes.push_back ( t );

    float dx=0;
    float dy=0;
    float w, h;
    for ( int i=0; i<20; i++ )
    {
	dx = 0;
	for ( int j=0; j<32; j++ )
	{
	    SLCTransformNode* t1 = new SLCTransformNode ();
	    t->addChild ( t1 );
	    nodes.push_back ( t1 );
	    t1->addChild ( logic0_lod0 ( mat, w, h ) );
	    mat_translatematrix ( t1->mat, dx, dy, 0 );
	    dx += 5 * w;
	}
	dy += 2 * h;
    }
    return t;
}

// --------------------------------------------------------------------------------

SLCNode* localrouting0_lod0 ( SLCMaterial *mat, float& width, float& height )
{
    width = 40;
    height = 300;
    float z = 0;

    SLCTransformNode* t0 = new SLCTransformNode ();
    nodes.push_back ( t0 );

    {
	SLCTransformNode* t = new SLCTransformNode ();
	mat_translatematrix ( t->mat, 4.5, -0.5, 0 );
	nodes.push_back ( t );
	t0->addChild ( t );
	{
	    SLCPLineNode* line = new SLCPLineNode( mat );
	    nodes.push_back ( line );
	    t->addChild ( line );
	    line->pnts.push_back ( vec2f( 0, 0) );
	    line->pnts.push_back ( vec2f( 0, -10.5) );
	    line->z = z;
	}

	{
	    SLCPLineNode* line = new SLCPLineNode( mat );
	    nodes.push_back ( line );
	    t->addChild ( line );
	    line->pnts.push_back ( vec2f( 4, 0) );
	    line->pnts.push_back ( vec2f( 4, -10.5) );
	    line->z = z;
	}
    }

    {
	SLCTransformNode* t = new SLCTransformNode ();
	mat_translatematrix ( t->mat, 30.5, 20, 0 );
	nodes.push_back ( t );
	t0->addChild ( t );
	{
	    SLCPLineNode* line = new SLCPLineNode( mat );
	    nodes.push_back ( line );
	    t->addChild ( line );
	    line->pnts.push_back ( vec2f( 0, 0) );
	    line->pnts.push_back ( vec2f( 0, 5) );
	    line->pnts.push_back ( vec2f( 13, 5) );
	    line->pnts.push_back ( vec2f( 13, 24) );
	    line->z = z;
	}

	{
	    SLCTransformNode* t1 = new SLCTransformNode ();
	    mat_translatematrix ( t1->mat, 12.5, 23.5, 0 );
	    SLCRectNode* rc1 = new SLCRectNode ( mat );
	    rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (1, 0 ); rc1->pnts[2] = vec2f (1, 1); rc1->pnts[3] = vec2f ( 0, 1);
	    rc1->z = -1;
	    t1->addChild ( rc1 );
	    t->addChild ( t1 );
	    nodes.push_back ( t1 );
	    nodes.push_back ( rc1 );
	}
    }

    {
	SLCTransformNode* t = new SLCTransformNode ();
	mat_translatematrix ( t->mat, 1, -10, 0 );
	nodes.push_back ( t );
	t0->addChild ( t );
	{
	    SLCPLineNode* line = new SLCPLineNode( mat );
	    nodes.push_back ( line );
	    t->addChild ( line );
	    line->pnts.push_back ( vec2f( 0, 0) );
	    line->pnts.push_back ( vec2f( -6, 0) );
	    line->pnts.push_back ( vec2f( -6, 164) );
	    line->pnts.push_back ( vec2f( 5.5, 164) );
	    line->pnts.push_back ( vec2f( 5.5, 168) );
	    line->z = z;
	}

	{
	    SLCPLineNode* line = new SLCPLineNode( mat );
	    nodes.push_back ( line );
	    t->addChild ( line );
	    line->pnts.push_back ( vec2f( 2, 172) );
	    line->pnts.push_back ( vec2f( -6, 172) );
	    line->pnts.push_back ( vec2f( -6, 340) );
	    line->z = z;
	}

	{
	    SLCPLineNode* line = new SLCPLineNode( mat );
	    nodes.push_back ( line );
	    t->addChild ( line );
	    line->pnts.push_back ( vec2f( -26, 340) );
	    line->pnts.push_back ( vec2f( -26, -15) );
	    line->pnts.push_back ( vec2f(  46, -15) );
	    line->pnts.push_back ( vec2f(  46, 340) );
	    line->z = z;
	}

	{
	    SLCPLineNode* line = new SLCPLineNode( mat );
	    nodes.push_back ( line );
	    t->addChild ( line );
	    line->pnts.push_back ( vec2f( -80, 355) );
	    line->pnts.push_back ( vec2f( -80, -25) );
	    line->z = z;
	}

	{
	    SLCPLineNode* line = new SLCPLineNode( mat );
	    nodes.push_back ( line );
	    t->addChild ( line );
	    line->pnts.push_back ( vec2f( 64, 355) );
	    line->pnts.push_back ( vec2f( 64, -25) );
	    line->z = z;
	}
    }
    return t0;
}

SLCNode* localrouting_lod0 ( SLCMaterial *mat, float& width, float& height )
{
    SLCTransformNode* t = new SLCTransformNode ();
    nodes.push_back ( t );

    float dx=0;
    float dy=0;
    float w, h;
    for ( int i=0; i<20; i++ )
    {
	dx = 0;
	for ( int j=0; j<32; j++ )
	{
	    SLCTransformNode* t1 = new SLCTransformNode ();
	    t->addChild ( t1 );
	    nodes.push_back ( t1 );
	    t1->addChild ( localrouting0_lod0 ( mat, w, h ) );
	    mat_translatematrix ( t1->mat, dx, dy, 0 );
	    dx += 5 * w;
	}
	dy += 2 * h;
    }
    return t;
}

// --------------------------------------------------------------------------------

SLCNode* port0_lod0 ( SLCMaterial *mat, float& width, float& height )
{
    width = 40;
    height = 300;

    SLCTransformNode* t00 = new SLCTransformNode ();
    nodes.push_back ( t00 );

    float dy=0;
    float h = 20;
    for ( int i=0; i<10; i++ )
    {
	SLCTransformNode* t0 = new SLCTransformNode ();
	mat_translatematrix ( t0->mat, 0, dy, 0 );
	nodes.push_back ( t0 );
	t00->addChild ( t0 );

	{
	    SLCTransformNode* t = new SLCTransformNode ();
	    mat_translatematrix ( t->mat, 0, 2.5, 0 );
	    nodes.push_back ( t );
	    t0->addChild ( t );
	    float z = 0;
	    {
		SLCPLineNode* line = new SLCPLineNode( mat );
		nodes.push_back ( line );
		t->addChild ( line );
		line->pnts.push_back ( vec2f( -80, 0) );
		line->pnts.push_back ( vec2f( 0, 0) );
		line->z = z;
	    }

	    {
		SLCPLineNode* line = new SLCPLineNode( mat );
		nodes.push_back ( line );
		t->addChild ( line );
		line->pnts.push_back ( vec2f( -80, 2.5) );
		line->pnts.push_back ( vec2f( 0, 2.5) );
		line->z = z;
	    }

	    {
		SLCPLineNode* line = new SLCPLineNode( mat );
		nodes.push_back ( line );
		t->addChild ( line );
		line->pnts.push_back ( vec2f( -80, 5) );
		line->pnts.push_back ( vec2f( 0, 5) );
		line->z = z;
	    }

	    {
		SLCPLineNode* line = new SLCPLineNode( mat );
		nodes.push_back ( line );
		t->addChild ( line );
		line->pnts.push_back ( vec2f( -80, 7.5) );
		line->pnts.push_back ( vec2f( 0, 7.5) );
		line->z = z;
	    }
	}

	{
	    SLCTransformNode* t = new SLCTransformNode ();
	    mat_translatematrix ( t->mat, 0, 14.5, 0 );
	    nodes.push_back ( t );
	    t0->addChild ( t );
	    float z = 0;
	    {
		SLCPLineNode* line = new SLCPLineNode( mat );
		nodes.push_back ( line );
		t->addChild ( line );
		line->pnts.push_back ( vec2f( -25, 0) );
		line->pnts.push_back ( vec2f( 0, 0) );
		line->z = z;
	    }

	    {
		SLCPLineNode* line = new SLCPLineNode( mat );
		nodes.push_back ( line );
		t->addChild ( line );
		line->pnts.push_back ( vec2f( -5, 2.5) );
		line->pnts.push_back ( vec2f( 0, 2.5) );
		line->z = z;
	    }
	}

	{
	    SLCTransformNode* t = new SLCTransformNode ();
	    mat_translatematrix ( t->mat, 40, 8.5, 0 );
	    nodes.push_back ( t );
	    t0->addChild ( t );
	    float z = 0;
	    {
		SLCPLineNode* line = new SLCPLineNode( mat );
		nodes.push_back ( line );
		t->addChild ( line );
		line->pnts.push_back ( vec2f( 0, 0) );
		line->pnts.push_back ( vec2f( 25, 0) );
		line->z = z;
	    }

	    {
		SLCPLineNode* line = new SLCPLineNode( mat );
		nodes.push_back ( line );
		t->addChild ( line );
		line->pnts.push_back ( vec2f( 0, 4) );
		line->pnts.push_back ( vec2f( 25, 4) );
		line->z = z;
	    }
	}

	if ( i == 4 )
	    dy += 2*h;
	else
	    dy += 3 * h/2;

    }
    return t00;
}

SLCNode* port_lod0 ( SLCMaterial *mat, float& width, float& height )
{
    SLCTransformNode* t = new SLCTransformNode ();
    nodes.push_back ( t );

    float dx=0;
    float dy=0;
    float w, h;
    for ( int i=0; i<20; i++ )
    {
	dx = 0;
	for ( int j=0; j<32; j++ )
	{
	    SLCTransformNode* t1 = new SLCTransformNode ();
	    t->addChild ( t1 );
	    nodes.push_back ( t1 );
	    t1->addChild ( port0_lod0 ( mat, w, h ) );
	    mat_translatematrix ( t1->mat, dx, dy, 0 );
	    dx += 5 * w;
	}
	dy += 2 * h;
    }
    return t;
}

// --------------------------------------------------------------------------------

void initMaterials ()
{
    gmat = new SLCMaterial ( "layer_material");
    gmat->foreground_color = vec3i(155, 0, 0);
    gmat->background_color = vec3i(255, 255, 255);
    gmat->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
    gmat->linewidth = 0;
    gmat->fontfilename = "simsun.ttc";
    gmat->texturefilename = "hands.jpg";
    nodes.push_back ( gmat );

    gmat1 = new SLCMaterial ( "layer_material1");
    gmat1->foreground_color = vec3i(0, 0, 155);
    gmat1->background_color = vec3i(190, 213, 235);
    gmat1->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
    gmat1->linewidth = 0;
    gmat1->fontfilename = "simsun.ttc";
    gmat1->texturefilename = "hands.jpg";
    nodes.push_back ( gmat1 );
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
    SLCSceneNode scene ( "test_scene");
    scene.addChild ( gmat );
    scene.addChild ( gmat1 );

    {
	SLCLayerNode* layer = new SLCLayerNode ( "background", gmat );
	nodes.push_back ( layer );
	SLCLODNode* lod = new SLCLODNode();
	nodes.push_back ( lod );
	SLCLODPageNode* lodpage = new SLCLODPageNode();
	nodes.push_back ( lodpage );
	lodpage->delayloading = false;
	lodpage->kdtree = "background0.idx";
	lodpage->imposter = true;
	scene.addChild ( layer );
	layer->addChild ( lod );
	lod->addChild ( lodpage );

	SLCTransformNode* t = new SLCTransformNode ();
//	mat_rotatematrix ( t->mat, M_PI / 2, 2 );
	mat_translatematrix ( t->mat, -100, -100, 0 );
	nodes.push_back ( t );
	lodpage->addChild ( t );

	float w = 6400;
	float h = 11900;
	SLCRectNode* rc1 = new SLCRectNode ( gmat1 );
	nodes.push_back ( rc1 );
	rc1->pnts[0] = vec2f ( 0, 0 );
	rc1->pnts[1] = vec2f ( w, 0 );
	rc1->pnts[2] = vec2f ( w, h );
	rc1->pnts[3] = vec2f ( 0, h );
	rc1->z = -2;
//	lodpage->addChild ( rc1 );
	t->addChild ( rc1 );
    }
    {
	SLCLayerNode* layer = new SLCLayerNode ( "local routing", gmat );
	nodes.push_back ( layer );
	SLCLODNode* lod = new SLCLODNode();
	nodes.push_back ( lod );
	SLCLODPageNode* lodpage = new SLCLODPageNode();
	nodes.push_back ( lodpage );
	lodpage->delayloading = false;
	lodpage->kdtree = "localrouting0.idx";
	lodpage->imposter = true;
	scene.addChild ( layer );
	layer->addChild ( lod );
	lod->addChild ( lodpage );
	float w, h;
	lodpage->addChild ( localrouting_lod0(gmat, w, h) );
    }
    {
	SLCLayerNode* layer = new SLCLayerNode ( "logic", gmat );
	nodes.push_back ( layer );
	SLCLODNode* lod = new SLCLODNode();
	nodes.push_back ( lod );
	SLCLODPageNode* lodpage = new SLCLODPageNode();
	nodes.push_back ( lodpage );
	lodpage->delayloading = false;
	lodpage->kdtree = "logic0.idx";
	lodpage->imposter = true;
	scene.addChild ( layer );
	layer->addChild ( lod );
	lod->addChild ( lodpage );
	float w, h;
	lodpage->addChild ( logic_lod0( gmat, w, h) );
    }

    {
	SLCLayerNode* layer = new SLCLayerNode ( "port", gmat );
	nodes.push_back ( layer );
	SLCLODNode* lod = new SLCLODNode();
	nodes.push_back ( lod );
	SLCLODPageNode* lodpage = new SLCLODPageNode();
	nodes.push_back ( lodpage );
	lodpage->delayloading = false;
	lodpage->kdtree = "port0.idx";
	lodpage->imposter = true;
	scene.addChild ( layer );
	layer->addChild ( lod );
	lod->addChild ( lodpage );
	float w, h;
	lodpage->addChild ( port_lod0( gmat, w, h) );
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
    LCReport rpt ( nlc );
    rpt.printCounter ();
    cout << "traverse finished, elapse " << clock() - t << "(ms)" << endl;

    t = clock();
    nlc.free();
    cout << "free LC finished, elapse " << clock() - t << "(ms)" << endl;
    
    return 0;
}

