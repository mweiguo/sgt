#include <iostream>
#include <fstream>
using namespace std;

#include "sgr_nodes.h"

int main ( int argc, char* argv[] )
{
    if ( argc != 2 )
    {
	cout << "usage : " << argv[0] << " outputFileName" << endl;
	cout << argv[0] << " will generate xml format for a simple scene" << endl;
	return 0;
    }

    list<SLCNode*> nodes;
    SLCSceneNode scene ("test_scene");
    {
	SLCMaterial* mat_layer = new SLCMaterial ("layer_material");
	mat_layer->foreground_color = vec3i(155, 0, 0);
	mat_layer->background_color = vec3i(0, 0, 155);
	mat_layer->linetype = SLCMaterial::LINETYPE_SOLID;
	mat_layer->linewidth = 0;
	mat_layer->fontfilename = "simsun.ttc";
	SLCMaterial* mat = new SLCMaterial ( "mat" );
	mat->foreground_color = vec3i(55, 0, 0);
	mat->background_color = vec3i(0, 44, 155);
	mat->linetype = SLCMaterial::LINETYPE_DASH;
	mat->linewidth = 1;
	mat->fontfilename = "simhei.ttf";
	SLCLayerNode* layer = new SLCLayerNode ( "layer1", mat_layer );
	SLCLODNode* lod = new SLCLODNode();
	SLCLODPageNode* lodpage = new SLCLODPageNode();
	lodpage->delayloading = false;
	lodpage->kdtree = "lodpage1.xml";
	lodpage->imposter = true;
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f (0, 0); rc1->pnts[1] = vec2f (1, 1);
	rc1->pnts[2] = vec2f (2, 2); rc1->pnts[3] = vec2f (3, 3);
	SLCRectNode* rc2 = new SLCRectNode ( mat );
	rc2->pnts[0] = vec2f (4, 4); rc2->pnts[1] = vec2f (5, 5);
	rc2->pnts[2] = vec2f (6, 6); rc2->pnts[3] = vec2f (7, 7);
	SLCRectNode* rc3 = new SLCRectNode ( mat_layer );
	rc3->pnts[0] = vec2f (8, 8);   rc3->pnts[1] = vec2f (9, 9);
	rc3->pnts[2] = vec2f (10, 10); rc3->pnts[3] = vec2f (11, 11);
	SLCTextNode* txt = new SLCTextNode ( mat_layer );
	txt->pos.xy ( 10, 20 );
	txt->scale = 1.5;
	txt->rotz = 45;
	txt->text = "china中国";
	SLCPLineNode* pline = new SLCPLineNode ( mat_layer );
	pline->pnts.push_back ( vec2f(0,0) );
	pline->pnts.push_back ( vec2f(1,0) );
	pline->pnts.push_back ( vec2f(1,1) );
	pline->pnts.push_back ( vec2f(0,1) );
	SLCLODPageNode* lodpage2 = new SLCLODPageNode();
	lodpage2->delayloading = true;
	lodpage2->kdtree = "lodpage2.xml";
	lodpage2->imposter = false;

	SLCLayerNode* layer2 = new SLCLayerNode ( "layer2", mat_layer );
	SLCLODNode* lod2 = new SLCLODNode ();
	SLCLODPageNode* lodpage3 = new SLCLODPageNode();
	lodpage3->delayloading = false;
	lodpage3->kdtree = "lodpage3.xml";
	lodpage3->imposter = true;
	SLCRectNode* rc4 = new SLCRectNode ( mat );
	rc4->pnts[0] = vec2f (0, 0); rc4->pnts[1] = vec2f (1, 1);
	rc4->pnts[2] = vec2f (2, 2); rc4->pnts[3] = vec2f (3, 3);

	scene.addChild ( mat_layer );
	scene.addChild ( mat );
	scene.addChild ( layer );
	scene.addChild ( layer2 );

	layer->addChild ( lod );
	lod->addChild ( lodpage );
	lod->addChild ( lodpage2 );
	lodpage->addChild ( rc1 );
	lodpage->addChild ( rc2 );
	lodpage->addChild ( rc3 );
	lodpage->addChild ( txt );
	lodpage->addChild ( pline );

	layer2->addChild ( lod2 );
	lod2->addChild ( lodpage3 );
	lodpage3->addChild ( rc4 );

	nodes.push_back ( mat_layer );
	nodes.push_back ( mat );
	nodes.push_back ( layer );
	nodes.push_back ( lod );
	nodes.push_back ( lodpage );
	nodes.push_back ( rc1 );
	nodes.push_back ( rc2 );
	nodes.push_back ( rc3 );
	nodes.push_back ( txt );
	nodes.push_back ( pline );
	nodes.push_back ( lodpage2 );
	nodes.push_back ( layer2 );
	nodes.push_back ( lod2 );
	nodes.push_back ( lodpage3 );
	nodes.push_back ( rc4 );
    }
    
    ofstream o;
//    o.imbue ( locale("chs") );
    o.open ( argv[1] );
    o << scene.toXML();
    o.close ();

    for ( list<SLCNode*>::iterator pp=nodes.begin(); pp!=nodes.end(); ++pp )
	delete *pp;
    return 0;
}
