#include <sgr_node2lc.h>
#include <sgr_nodes.h>
#include "sgr_lcreport.h"
#include "mat4f.h"
#include <fstream>
#include <ctime>

list<SLCNode*> nodes;
SLCMaterial* mat;

SLCNode* logic0_lod0 ()
{
    SLCTransformNode* t = new SLCTransformNode ();
    nodes.push_back ( t );

    {
	SLCTransformNode* t1 = new SLCTransformNode ();
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (40, 0 ); rc1->pnts[2] = vec2f (40, 20); rc1->pnts[3] = vec2f ( 0, 20);
	rc1->z = -1;
	t1->addChild ( rc1 );
	t->addChild ( t1 );
	nodes.push_back ( t1 );
	nodes.push_back ( rc1 );
    }

    {
	SLCTransformNode* t1 = new SLCTransformNode ();
	mat_translatematrix ( t1->mat, -0.5, 16.5, 0 );
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (1, 0 ); rc1->pnts[2] = vec2f (1, 1); rc1->pnts[3] = vec2f ( 0, 1);
	rc1->z = -1;
	t1->addChild ( rc1 );
	t->addChild ( t1 );
	nodes.push_back ( t1 );
	nodes.push_back ( rc1 );
    }

    {
	SLCTransformNode* t1 = new SLCTransformNode ();
	mat_translatematrix ( t1->mat, -0.5, 14, 0 );
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (1, 0 ); rc1->pnts[2] = vec2f (1, 1); rc1->pnts[3] = vec2f ( 0, 1);
	rc1->z = -1;
	t1->addChild ( rc1 );
	t->addChild ( t1 );
	nodes.push_back ( t1 );
	nodes.push_back ( rc1 );
    }

    {
	SLCTransformNode* t1 = new SLCTransformNode ();
	mat_translatematrix ( t1->mat, -0.5, 9.5, 0 );
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (1, 0 ); rc1->pnts[2] = vec2f (1, 1); rc1->pnts[3] = vec2f ( 0, 1);
	rc1->z = -1;
	t1->addChild ( rc1 );
	t->addChild ( t1 );
	nodes.push_back ( t1 );
	nodes.push_back ( rc1 );
    }

    {
	SLCTransformNode* t1 = new SLCTransformNode ();
	mat_translatematrix ( t1->mat, -0.5, 7, 0 );
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (1, 0 ); rc1->pnts[2] = vec2f (1, 1); rc1->pnts[3] = vec2f ( 0, 1);
	rc1->z = -1;
	t1->addChild ( rc1 );
	t->addChild ( t1 );
	nodes.push_back ( t1 );
	nodes.push_back ( rc1 );
    }

    {
	SLCTransformNode* t1 = new SLCTransformNode ();
	mat_translatematrix ( t1->mat, -0.5, 4.5, 0 );
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (1, 0 ); rc1->pnts[2] = vec2f (1, 1); rc1->pnts[3] = vec2f ( 0, 1);
	rc1->z = -1;
	t1->addChild ( rc1 );
	t->addChild ( t1 );
	nodes.push_back ( t1 );
	nodes.push_back ( rc1 );
    }

    {
	SLCTransformNode* t1 = new SLCTransformNode ();
	mat_translatematrix ( t1->mat, -0.5, 2, 0 );
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (1, 0 ); rc1->pnts[2] = vec2f (1, 1); rc1->pnts[3] = vec2f ( 0, 1);
	rc1->z = -1;
	t1->addChild ( rc1 );
	t->addChild ( t1 );
	nodes.push_back ( t1 );
	nodes.push_back ( rc1 );
    }

    {
	SLCTransformNode* t1 = new SLCTransformNode ();
	mat_translatematrix ( t1->mat, 4, -0.5, 0 );
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (1, 0 ); rc1->pnts[2] = vec2f (1, 1); rc1->pnts[3] = vec2f ( 0, 1);
	rc1->z = -1;
	t1->addChild ( rc1 );
	t->addChild ( t1 );
	nodes.push_back ( t1 );
	nodes.push_back ( rc1 );
    }

    {
	SLCTransformNode* t1 = new SLCTransformNode ();
	mat_translatematrix ( t1->mat, 8, -0.5, 0 );
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (1, 0 ); rc1->pnts[2] = vec2f (1, 1); rc1->pnts[3] = vec2f ( 0, 1);
	rc1->z = -1;
	t1->addChild ( rc1 );
	t->addChild ( t1 );
	nodes.push_back ( t1 );
	nodes.push_back ( rc1 );
    }

    {
	SLCTransformNode* t1 = new SLCTransformNode ();
	mat_translatematrix ( t1->mat, 39.5, 8, 0 );
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (1, 0 ); rc1->pnts[2] = vec2f (1, 1); rc1->pnts[3] = vec2f ( 0, 1);
	rc1->z = -1;
	t1->addChild ( rc1 );
	t->addChild ( t1 );
	nodes.push_back ( t1 );
	nodes.push_back ( rc1 );
    }

    {
	SLCTransformNode* t1 = new SLCTransformNode ();
	mat_translatematrix ( t1->mat, 39.5, 12, 0 );
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (1, 0 ); rc1->pnts[2] = vec2f (1, 1); rc1->pnts[3] = vec2f ( 0, 1);
	rc1->z = -1;
	t1->addChild ( rc1 );
	t->addChild ( t1 );
	nodes.push_back ( t1 );
	nodes.push_back ( rc1 );
    }

    {
	SLCTransformNode* t1 = new SLCTransformNode ();
	mat_translatematrix ( t1->mat, 30, 19.5, 0 );
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (1, 0 ); rc1->pnts[2] = vec2f (1, 1); rc1->pnts[3] = vec2f ( 0, 1);
	rc1->z = -1;
	t1->addChild ( rc1 );
	t->addChild ( t1 );
	nodes.push_back ( t1 );
	nodes.push_back ( rc1 );
    }

    {
	SLCTransformNode* t1 = new SLCTransformNode ();
	mat_translatematrix ( t1->mat, 8, 19.5, 0 );
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (1, 0 ); rc1->pnts[2] = vec2f (1, 1); rc1->pnts[3] = vec2f ( 0, 1);
	rc1->z = -1;
	t1->addChild ( rc1 );
	t->addChild ( t1 );
	nodes.push_back ( t1 );
	nodes.push_back ( rc1 );
    }

    {
	SLCTransformNode* t1 = new SLCTransformNode ();
	mat_translatematrix ( t1->mat, 4, 19.5, 0 );
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f ( 0, 0 ); rc1->pnts[1] = vec2f (1, 0 ); rc1->pnts[2] = vec2f (1, 1); rc1->pnts[3] = vec2f ( 0, 1);
	rc1->z = -1;
	t1->addChild ( rc1 );
	t->addChild ( t1 );
	nodes.push_back ( t1 );
	nodes.push_back ( rc1 );
    }
    return t;
}

void initMaterials ()
{
    mat = new SLCMaterial ( "layer_material");
    mat->foreground_color = vec3i(155, 0, 0);
    mat->background_color = vec3i(155, 0, 155);
    mat->linetype = SLCMaterial::LINETYPE_SOLID;
    mat->linewidth = 0;
    mat->fontfilename = "simsun.ttc";
    mat->texturefilename = "hands.jpg";
    nodes.push_back ( mat );

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
    SLCLayerNode* layer = new SLCLayerNode ( "default layer", mat );
    nodes.push_back ( layer );
    SLCLODNode* lod = new SLCLODNode();
    nodes.push_back ( lod );
    SLCLODPageNode* lodpage = new SLCLODPageNode();
    nodes.push_back ( lodpage );
    lodpage->delayloading = false;
    lodpage->kdtree = "lodpage1.idx";
    lodpage->imposter = true;

    // build relations
    scene.addChild ( mat );
    scene.addChild ( layer );
    layer->addChild ( lod );
    lod->addChild ( lodpage );
    lodpage->addChild ( logic0_lod0() );

    ofstream o;
    o.open ("test.xml" );
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

