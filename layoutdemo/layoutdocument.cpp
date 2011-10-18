#include "layoutdocument.h"

#include <sgr_render2d.h>
#include <sgr_nodes.h>
#include <mat4f.h>
#include <sgr_node2lc.h>

#include <ctime>
#include <utility>
#include <sstream>
#include <iostream>
#include <fstream>
using namespace std;

LayoutDocument::LayoutDocument () :  Document()
{
    layoutSceneId = -1;
    plate = 0;
    objmat = 0;
    platemat = 0;
}

LayoutDocument::~LayoutDocument()
{
}

void LayoutDocument::openLayoutScene ( const char* filename )
{
    clock_t t = clock();
    layoutSceneId = r2d_load_scene ( filename );
    cout << "load " << filename << " finished, ellapse " << clock() - t << endl;
}

//--------------------------------------------------------------------------------

void LayoutDocument::buildDemoShapes()
{
    objmat = new SLCMaterial( "objectMaterial");
    objmat->foreground_color = vec4i(155, 0, 0, 155);
    objmat->background_color = vec4i(0, 200, 200, 2555);
    objmat->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
    objmat->linewidth = 1;
    objmat->fontfilename = "";
    objmat->texturefilename = "";

    platemat = new SLCMaterial( "plateMaterial" );
    platemat->foreground_color = vec4i(155, 0, 0, 155);
    platemat->background_color = vec4i(200, 0, 200, 255);
    platemat->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
    platemat->linewidth = 1;
    platemat->fontfilename = "";
    platemat->texturefilename = "";

    int shapeid = 0;
    {
	SLCRectNode* rc1 = new SLCRectNode ( objmat );
	rc1->pnts[0] = vec2f ( 0, 0 );
	rc1->pnts[1] = vec2f ( 5, 0 );
	rc1->pnts[2] = vec2f ( 5, 5 );
	rc1->pnts[3] = vec2f ( 0, 5 );
	rc1->z = 2;
	shapesInfo.insert ( pair<int,ShapeInfo>(shapeid,ShapeInfo()) );
	ShapeInfo& sinfo = shapesInfo[shapeid];
	sinfo.primitiveCount = 1;
	sinfo.node = rc1;
	shapeid ++;

	SLCTextNode* txt = new SLCTextNode ( objmat );
	stringstream ss;
	ss << sinfo.primitiveCount;
	txt->text = ss.str();
	txt->pos = vec3f ( 0, -5, 2 );
	txt->scale = 4;
	sinfo.cntnode = txt;
    }

    {
	SLCRectNode* rc1 = new SLCRectNode ( objmat );
	rc1->pnts[0] = vec2f ( 10,  0 );
	rc1->pnts[1] = vec2f ( 16, 0 );
	rc1->pnts[2] = vec2f ( 16, 3 );
	rc1->pnts[3] = vec2f ( 10,  3 );
	rc1->z = 2;
	shapesInfo.insert ( pair<int,ShapeInfo>(shapeid,ShapeInfo()) );
	ShapeInfo& sinfo = shapesInfo[shapeid];
	sinfo.primitiveCount = 1;
	sinfo.node = rc1;
	shapeid ++;

	SLCTextNode* txt = new SLCTextNode ( objmat );
	stringstream ss;
	ss << sinfo.primitiveCount;
	txt->text = ss.str();
	txt->pos = vec3f ( 10, -5, 2 );
	txt->scale = 4;
	sinfo.cntnode = txt;
    }

    plate = new SLCRectNode ( platemat );
    plate->pnts[0] = vec2f ( 0, 0 );
    plate->pnts[1] = vec2f ( 100, 0 );
    plate->pnts[2] = vec2f ( 100, 100 );
    plate->pnts[3] = vec2f ( 0, 100 );
    plate->z = 2;

}

//--------------------------------------------------------------------------------

void LayoutDocument::clearShapes ()
{
    for ( map<int, ShapeInfo>::iterator pp=shapesInfo.begin();
	  pp!=shapesInfo.end(); ++pp )
    {
	ShapeInfo& sinfo = pp->second;
	delete sinfo.node;
	delete sinfo.cntnode;
    }
    shapesInfo.clear();

    delete plate;
    delete objmat;
    delete platemat;
}

//--------------------------------------------------------------------------------

// int LayoutDocument::addShape ( const char* filename )
// {
//     ShapeInfo sinfo;
//     sinfo.sceneId = r2d_load_scene ( filename );
//     r2d_to_element ( sid, R2D_ROOT );
//     r2d_to_element ( sid, R2D_FIRST_CHILD ); // layer
//     r2d_to_element ( sid, R2D_FIRST_CHILD ); // lod
//     r2d_to_element ( sid, R2D_FIRST_CHILD ); // lodpage

//     sinfo.primitiveId = r2d_to_element ( sid, R2D_FIRST_CHILD ); // rect
//     sinfo.primitiveCount = 1;
// }

void LayoutDocument::setShapeCount ( int shapeid, int count )
{
    map<int, ShapeInfo>::iterator pp = shapesInfo.find ( shapeid );
    if ( pp != shapesInfo.end() )
    {
	ShapeInfo& sinfo = pp->second;
	sinfo.primitiveCount = count;
    }
}

void LayoutDocument::setPlateSize ( float width, float height )
{
    plate->setSize ( width, height );
}

void LayoutDocument::saveShapeFile ( const char* filename, const char* slcFileName )
{
    list<SLCNode*> nodes;
    // create scene node
    SLCSceneNode scene ( "shapes_scene");
    scene.addChild ( objmat );
    SLCLayerNode* layer = new SLCLayerNode ( "background", objmat );
    nodes.push_back ( layer );
    scene.addChild ( layer );
    SLCLODNode* lod = new SLCLODNode();
    nodes.push_back ( lod );
    layer->addChild ( lod );
    SLCLODPageNode* lodpage = new SLCLODPageNode();
    lodpage->delayloading = false;
    lodpage->imposter = true;
    nodes.push_back ( lodpage );
    lod->addChild ( lodpage );
    
    float x = 0;
    float y = 0;
    // join them together
    for ( map<int, ShapeInfo>::iterator pp=shapesInfo.begin();
	  pp!=shapesInfo.end(); ++pp )
    {
	ShapeInfo& sinfo = pp->second;

	SLCTransformNode* t1 = new SLCTransformNode ();
	mat_translatematrix ( t1->mat, x, y, 0 );
	nodes.push_back ( t1 );
	lodpage->addChild ( t1 );

	SLCPrimitiveNode* node = sinfo.node->copy();
	nodes.push_back ( node );
	t1->addChild ( node );

	{
	    SLCPrimitiveNode* node = sinfo.cntnode->copy();
	    nodes.push_back ( node );
	    t1->addChild ( node );
	}

	float minmax[6];
	node->getMinMax ( minmax );
	x += minmax[3] - minmax[0];
//	y += minmax[4] - minmax[1];
    }

    // output to xml
    ofstream o;
    o.open ( filename );
    o << scene.toXML();
    o.close();

    SLCNode2LC node2lc ( &scene );
    node2lc.convert ( slcFileName );

    // delete nodes
    for ( list<SLCNode*>::iterator pp=nodes.begin(); pp!=nodes.end(); ++pp )
        delete *pp;
}

void LayoutDocument::savePlateFile ( const char* filename )
{
    list<SLCNode*> nodes;
    // create scene node
    SLCSceneNode scene ( "shapes_scene");
    scene.addChild ( platemat );
    SLCLayerNode* layer = new SLCLayerNode ( "background", platemat );
    nodes.push_back ( layer );
    scene.addChild ( layer );
    SLCLODNode* lod = new SLCLODNode();
    nodes.push_back ( lod );
    layer->addChild ( lod );
    SLCLODPageNode* lodpage = new SLCLODPageNode();
    lodpage->delayloading = false;
    lodpage->imposter = true;
    nodes.push_back ( lodpage );
    lod->addChild ( lodpage );
    lodpage->addChild ( plate );

    // output to xml
    ofstream o;
    o.open ( filename );
    o << scene.toXML();
    o.close();

    // delete nodes
    for ( list<SLCNode*>::iterator pp=nodes.begin(); pp!=nodes.end(); ++pp )
        delete *pp;
}


