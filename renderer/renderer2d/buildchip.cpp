#include <sgr_node2lc.h>
#include <sgr_nodes.h>
#include "sgr_lcreport.h"
#include "mat4f.h"
#include <fstream>
#include <ctime>
#include <math.h>

list<SLCNode*> nodes;
SLCMaterial *gmat, *gmat1, *gmatLine0, *gmatLine, *gmatLine2, *gmatLine3;

SLCNode* logic0_lod2 ( SLCMaterial *mat, float& width, float& height )
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

SLCNode* logic_lod2 ( SLCMaterial *mat, float& width, float& height )
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
	    t1->addChild ( logic0_lod2 ( mat, w, h ) );
	    mat_translatematrix ( t1->mat, dx, dy, 0 );
	    dx += 7 * w;
	}
	dy += 1.8 * h;
    }
    return t;
}

SLCNode* logic0_lod0 ( SLCMaterial *mat, float& width, float& height )
{
    width = 40;
    height = 300;
    SLCTransformNode* t = new SLCTransformNode ();
    nodes.push_back ( t );

    float dy=0;
    float h = 150;
    for ( int i=0; i<2; i++ )
    {
	SLCTransformNode* t1 = new SLCTransformNode ();
	mat_translatematrix ( t1->mat, -40, dy, 0 );
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (80, 0 ); rc1->pnts[2] = vec2f (80, 150); rc1->pnts[3] = vec2f ( 0, 150);
	rc1->z = -1;
	t1->addChild ( rc1 );
	t->addChild ( t1 );
	nodes.push_back ( t1 );
	nodes.push_back ( rc1 );

	dy += 4 * h/3;
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
	    dx += 7 * w;
	}
	dy += 1.8 * h;
    }
    return t;
}

SLCNode* logic0_lod1 ( SLCMaterial *mat, float& width, float& height )
{
    width = 40;
    height = 300;
    SLCTransformNode* t = new SLCTransformNode ();
    nodes.push_back ( t );

    float dy=0;
    float h = 150;
    for ( int i=0; i<2; i++ )
    {
	SLCTransformNode* t1 = new SLCTransformNode ();
	mat_translatematrix ( t1->mat, 0, dy, 0 );
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (40, 0 ); rc1->pnts[2] = vec2f (40, 150); rc1->pnts[3] = vec2f ( 0, 150);
	rc1->z = -1;
	t1->addChild ( rc1 );
	t->addChild ( t1 );
	nodes.push_back ( t1 );
	nodes.push_back ( rc1 );

	dy += 5 * h/4;
    }
    return t;
}

SLCNode* logic_lod1 ( SLCMaterial *mat, float& width, float& height )
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
	    t1->addChild ( logic0_lod1 ( mat, w, h ) );
	    mat_translatematrix ( t1->mat, dx, dy, 0 );
	    dx += 7 * w;
	}
	dy += 1.8 * h;
    }
    return t;
}

// --------------------------------------------------------------------------------

SLCNode* globalrouting_lod0 ( SLCMaterial *mat, float& width, float& height )
{
    width = 40;
    height = 300;
    float z = 1;
    SLCTransformNode* t0 = new SLCTransformNode ();
    nodes.push_back ( t0 );

    // horizontal
    float y = 430;
    for ( int i=0; i<19; i++ )
    {
	SLCPLineNode* line = new SLCPLineNode( gmatLine0 );
	nodes.push_back ( line );
	t0->addChild ( line );
	line->pnts.push_back ( vec2f( -100, y) );
	line->pnts.push_back ( vec2f( 8800, y) );
	line->z = z;
	y += 1.8 * 300;
    }

    float x = 130;
    for ( int i=0; i<31; i++ )
    {
	SLCPLineNode* line = new SLCPLineNode( gmatLine0 );
	nodes.push_back ( line );
	t0->addChild ( line );
	line->pnts.push_back ( vec2f( x, -100) );
	line->pnts.push_back ( vec2f( x, 10700) );
	line->z = z;
	x += 7 * 40;
    }
    return t0;
}

// --------------------------------------------------------------------------------

SLCNode* localrouting0_lod0 ( SLCMaterial *mat, float& width, float& height )
{
    width = 40;
    height = 300;
    float z = 2;

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
	    SLCPLineNode* line = new SLCPLineNode( gmatLine3 );
	    nodes.push_back ( line );
	    t->addChild ( line );
	    line->pnts.push_back ( vec2f( -26, 340) );
	    line->pnts.push_back ( vec2f( -26, -15) );
	    line->pnts.push_back ( vec2f(  46, -15) );
	    line->pnts.push_back ( vec2f(  46, 340) );
	    line->z = z;
	}

	{
	    SLCPLineNode* line = new SLCPLineNode( gmatLine );
	    nodes.push_back ( line );
	    t->addChild ( line );
	    line->pnts.push_back ( vec2f( -80, 355) );
	    line->pnts.push_back ( vec2f( -80, -25) );
	    line->z = z;
	}

	{
	    SLCPLineNode* line = new SLCPLineNode( gmatLine2 );
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
	    dx += 7 * w;
	}
	dy += 1.8 * h;
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
	    dx += 7 * w;
	}
	dy += 1.8 * h;
    }
    return t;
}

// --------------------------------------------------------------------------------

void initMaterials ()
{
    gmat = new SLCMaterial ( "material");
    gmat->foreground_color = vec4i(155, 0, 0, 255);
    gmat->background_color = vec4i(255, 255, 255, 255);
    gmat->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
    gmat->linewidth = 1;
    gmat->fontfilename = "simsun.ttc";
    gmat->texturefilename = "hands.jpg";
    nodes.push_back ( gmat );

    gmat1 = new SLCMaterial ( "material1");
    gmat1->foreground_color = vec4i(0, 0, 155, 255);
    gmat1->background_color = vec4i(190, 213, 235, 255);
    gmat1->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
    gmat1->linewidth = 1;
    gmat1->fontfilename = "simsun.ttc";
    gmat1->texturefilename = "hands.jpg";
    nodes.push_back ( gmat1 );

    gmatLine0 = new SLCMaterial ( "materialLine0");
    gmatLine0->foreground_color = vec4i(0, 0, 155, 255);
    gmatLine0->background_color = vec4i(250, 250, 250, 255);
    gmatLine0->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
    gmatLine0->linewidth = 10000;
    gmatLine0->fontfilename = "simsun.ttc";
    gmatLine0->texturefilename = "hands.jpg";
    nodes.push_back ( gmatLine0 );

    gmatLine = new SLCMaterial ( "materialLine");
    gmatLine->foreground_color = vec4i(0, 0, 155, 255);
    gmatLine->background_color = vec4i(255, 255, 252, 255);
    gmatLine->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
    gmatLine->linewidth = 4000;
    gmatLine->fontfilename = "simsun.ttc";
    gmatLine->texturefilename = "hands.jpg";
    nodes.push_back ( gmatLine );

    gmatLine2 = new SLCMaterial ( "materialLine2");
    gmatLine2->foreground_color = vec4i(0, 0, 155, 255);
    gmatLine2->background_color = vec4i(255, 255, 254, 255);
    gmatLine2->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
    gmatLine2->linewidth = 2000;
    gmatLine2->fontfilename = "simsun.ttc";
    gmatLine2->texturefilename = "hands.jpg";
    nodes.push_back ( gmatLine2 );

    gmatLine3 = new SLCMaterial ( "materialLine3");
    gmatLine3->foreground_color = vec4i(0, 0, 155, 255);
    gmatLine3->background_color = vec4i(255, 255, 250, 255);
    gmatLine3->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
    gmatLine3->linewidth = 1000;
    gmatLine3->fontfilename = "simsun.ttc";
    gmatLine3->texturefilename = "hands.jpg";
    nodes.push_back ( gmatLine3 );
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
    scene.addChild ( gmatLine0 );
    scene.addChild ( gmatLine );
    scene.addChild ( gmatLine2 );
    scene.addChild ( gmatLine3 );

    {
	SLCLayerNode* layer = new SLCLayerNode ( "background", gmat );
        layer->pickable = false;
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

	float w = 8900;
	float h = 10800;
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
	SLCLayerNode* layer = new SLCLayerNode ( "global routing", gmat );
	nodes.push_back ( layer );
	SLCLODNode* lod = new SLCLODNode();
	nodes.push_back ( lod );
	SLCLODPageNode* lodpage = new SLCLODPageNode();
	nodes.push_back ( lodpage );
	lodpage->delayloading = false;
	lodpage->kdtree = "globalrouting0.idx";
	lodpage->imposter = true;
	scene.addChild ( layer );
	layer->addChild ( lod );
	lod->addChild ( lodpage );
	float w, h;
	lodpage->addChild ( globalrouting_lod0(gmat, w, h) );
    }

    {
	float w, h;
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
	lodpage->addChild ( localrouting_lod0(gmat, w, h) );

	SLCLODPageNode* lodpage1 = new SLCLODPageNode();
	nodes.push_back ( lodpage1 );
	lod->addChild ( lodpage1 );
	lod->addChild ( lodpage );
	lod->scales.push_back (0.00019);
    }
    {
	float w, h;
	SLCLayerNode* layer = new SLCLayerNode ( "logic", gmat );
	nodes.push_back ( layer );
	scene.addChild ( layer );

	SLCLODNode* lod = new SLCLODNode();
	nodes.push_back ( lod );
	layer->addChild ( lod );

	SLCLODPageNode* lodpage0 = new SLCLODPageNode();
	nodes.push_back ( lodpage0 );
	lodpage0->delayloading = false;
	lodpage0->kdtree = "logic0.idx";
	lodpage0->imposter = true;
	lodpage0->addChild ( logic_lod0( gmat, w, h) );

	SLCLODPageNode* lodpage1 = new SLCLODPageNode();
	nodes.push_back ( lodpage1 );
	lodpage1->delayloading = false;
	lodpage1->kdtree = "logic1.idx";
	lodpage1->imposter = true;
	lodpage1->addChild ( logic_lod1( gmat, w, h) );

	SLCLODPageNode* lodpage2 = new SLCLODPageNode();
	nodes.push_back ( lodpage2 );
	lodpage2->delayloading = false;
	lodpage2->kdtree = "logic2.idx";
	lodpage2->imposter = true;
	lodpage2->addChild ( logic_lod2( gmat, w, h) );

	lod->addChild ( lodpage0 );
	lod->addChild ( lodpage1 );
	lod->addChild ( lodpage2 );
	lod->scales.push_back (0.00019);
	lod->scales.push_back (0.00022);
    }

    {
	float w, h;
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
	lodpage->addChild ( port_lod0( gmat, w, h) );
	SLCLODPageNode* lodpage1 = new SLCLODPageNode();
	nodes.push_back ( lodpage1 );
	lod->addChild ( lodpage1 );
	lod->addChild ( lodpage );

	lod->scales.push_back (0.00026);
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

