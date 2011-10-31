#include "layoutdocument.h"

#include <sgr_render2d.h>
#include <sgr_nodes.h>
#include <mat4f.h>
#include <sgr_node2lc.h>

#include <glwidget.h>

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

void LayoutDocument::openLayoutScene ( GLWidget* widget, const char* filename )
{
    clock_t t = clock();

    widget->makeCurrent();
    closeLayoutScene ();
    if ( (layoutSceneId = r2d_load_scene ( filename )) != -1 ) {
	cout << "load " << filename << " finished, ellapse " << clock() - t << endl;
    } else {
	cerr << "load " << filename << " failed, ellapse " << clock() - t << endl;
    }
}

void LayoutDocument::closeLayoutScene ()
{
    if ( layoutSceneId != -1 ) {
        r2d_unload_scene ( layoutSceneId );
        layoutSceneId = -1;
    }
}

//--------------------------------------------------------------------------------

void LayoutDocument::buildDemoShapes()
{
    materials.clear();
    objmat = new SLCMaterial( "objectMaterial");
    objmat->foreground_color = vec4i(155, 0, 0, 155);
    objmat->background_color = vec4i(0, 200, 200, 255);
    objmat->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
    objmat->linewidth = 1;
    objmat->fontfilename = "";
    objmat->texturefilename = "";
    materials.insert ( pair<string,SLCMaterial*>("objectMaterial", objmat) );

    objmat1 = new SLCMaterial( "objectMaterial1");
    objmat1->foreground_color = vec4i(155, 0, 0, 155);
    objmat1->background_color = vec4i(200, 0, 0, 155);
    objmat1->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
    objmat1->linewidth = 1;
    objmat1->fontfilename = "";
    objmat1->texturefilename = "";
    materials.insert ( pair<string,SLCMaterial*>("objectMaterial1", objmat1) );

    objmat2 = new SLCMaterial( "objectMaterial2");
    objmat2->foreground_color = vec4i(155, 0, 0, 155);
    objmat2->background_color = vec4i(200, 200, 0, 155);
    objmat2->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
    objmat2->linewidth = 1;
    objmat2->fontfilename = "";
    objmat2->texturefilename = "";
    materials.insert ( pair<string,SLCMaterial*>("objectMaterial2", objmat2) );

    objmat3 = new SLCMaterial( "objectMaterial3");
    objmat3->foreground_color = vec4i(155, 0, 0, 155);
    objmat3->background_color = vec4i(0, 0, 200, 155);
    objmat3->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
    objmat3->linewidth = 1;
    objmat3->fontfilename = "";
    objmat3->texturefilename = "";
    materials.insert ( pair<string,SLCMaterial*>("objectMaterial3", objmat3) );

    objmat4 = new SLCMaterial( "objectMaterial4");
    objmat4->foreground_color = vec4i(155, 0, 0, 155);
    objmat4->background_color = vec4i(200, 0, 200, 155);
    objmat4->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
    objmat4->linewidth = 1;
    objmat4->fontfilename = "";
    objmat4->texturefilename = "";
    materials.insert ( pair<string,SLCMaterial*>("objectMaterial4", objmat4) );

    platemat = new SLCMaterial( "plateMaterial" );
    platemat->foreground_color = vec4i(155, 0, 0, 155);
    platemat->background_color = vec4i(200, 200, 200, 255);
    platemat->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
    platemat->linewidth = 1;
    platemat->fontfilename = "";
    platemat->texturefilename = "";
    materials.insert ( pair<string,SLCMaterial*>("plateMaterial", platemat) );

    int shapeid = 0;
    {
        SLCTextNode* txt = new SLCTextNode ( objmat );
        txt->text = "SHAPE";
        txt->pos = vec3f ( -15, 0, 2 );
        txt->scale = 4;
        shapesInfo.insert ( pair<int,ShapeInfo>(shapeid,ShapeInfo()) );
        ShapeInfo& sinfo = shapesInfo[shapeid];
        sinfo.node = txt;
        shapeid ++;

        SLCTextNode* txt1 = new SLCTextNode ( objmat );
        txt1->text = "COUNT";
        txt1->pos = vec3f ( -15, -5, 2 );
        txt1->scale = 4;
        sinfo.cntnode = txt1;

        SLCTextNode* txt2 = new SLCTextNode ( objmat );
        txt2->text = "SIZE";
        txt2->pos = vec3f ( -15, -10, 2 );
        txt2->scale = 4;
        sinfo.sizenode = txt2;

        SLCTextNode* txt3 = new SLCTextNode ( objmat );
        txt3->text = "COLOR";
        txt3->pos = vec3f ( -15, -15, 2 );
        txt3->scale = 4;
        sinfo.colornode = txt3;
    }

    {
        SLCRectNode* rc1 = new SLCRectNode ( objmat1 );
        rc1->pnts[0] = vec2f ( 0, 0 );
        rc1->pnts[1] = vec2f ( 5, 0 );
        rc1->pnts[2] = vec2f ( 5, 5 );
        rc1->pnts[3] = vec2f ( 0, 5 );
        rc1->z = 2;
        shapesInfo.insert ( pair<int,ShapeInfo>(shapeid,ShapeInfo()) );
        ShapeInfo& sinfo = shapesInfo[shapeid];
        sinfo.node = rc1;
        shapeid ++;

        SLCTextNode* txt = new SLCTextNode ( objmat );
        stringstream ss;
        txt->text = "1";
        txt->pos = vec3f ( 0, -5, 2 );
        txt->scale = 4;
        sinfo.cntnode = txt;

        SLCTextNode* txt1 = new SLCTextNode ( objmat );
        ss.str("");
        ss << "5 X 5";
        txt1->text = ss.str();
        txt1->pos = vec3f ( 0, -10, 2 );
        txt1->scale = 4;
        sinfo.sizenode = txt1;

        SLCRectNode* rc2 = new SLCRectNode ( objmat1 );
        rc2->pnts[0] = vec2f ( 0,  -15 );
        rc2->pnts[1] = vec2f ( 10, -15 );
        rc2->pnts[2] = vec2f ( 10, -13 );
        rc2->pnts[3] = vec2f ( 0,  -13 );
        rc2->z = 2;
        sinfo.colornode = rc2;
    }

    {
        SLCRectNode* rc1 = new SLCRectNode ( objmat2 );
        rc1->pnts[0] = vec2f ( 10,  0 );
        rc1->pnts[1] = vec2f ( 16, 0 );
        rc1->pnts[2] = vec2f ( 16, 3 );
        rc1->pnts[3] = vec2f ( 10, 3 );
        rc1->z = 2;
        shapesInfo.insert ( pair<int,ShapeInfo>(shapeid,ShapeInfo()) );
        ShapeInfo& sinfo = shapesInfo[shapeid];
        sinfo.node = rc1;
        shapeid ++;

        SLCTextNode* txt = new SLCTextNode ( objmat );
        txt->text = "1";
        txt->pos = vec3f ( 10, -5, 2 );
        txt->scale = 4;
        sinfo.cntnode = txt;

        SLCTextNode* txt1 = new SLCTextNode ( objmat );
        stringstream ss;
        ss << "6 X 3";
        txt1->text = ss.str();
        txt1->pos = vec3f ( 10, -10, 2 );
        txt1->scale = 4;
        sinfo.sizenode = txt1;

        SLCTextNode* txt2 = new SLCTextNode ( objmat );
        ss.str("");
        ss << 3;
        txt2->text = ss.str();
        txt2->pos = vec3f ( 10, -15, 2 );
        txt2->scale = 4;
        sinfo.colornode = txt2;

        SLCRectNode* rc2 = new SLCRectNode ( objmat2 );
        rc2->pnts[0] = vec2f ( 10,  -15 );
        rc2->pnts[1] = vec2f ( 20, -15 );
        rc2->pnts[2] = vec2f ( 20, -13 );
        rc2->pnts[3] = vec2f ( 10,  -13 );
        rc2->z = 2;
        sinfo.colornode = rc2;
    }

    {
        SLCRectNode* rc1 = new SLCRectNode ( objmat3 );
        rc1->pnts[0] = vec2f ( 20, 0 );
        rc1->pnts[1] = vec2f ( 22, 0 );
        rc1->pnts[2] = vec2f ( 22, 10 );
        rc1->pnts[3] = vec2f ( 20, 10 );
        rc1->z = 2;
        shapesInfo.insert ( pair<int,ShapeInfo>(shapeid,ShapeInfo()) );
        ShapeInfo& sinfo = shapesInfo[shapeid];
        sinfo.node = rc1;
        shapeid ++;

        SLCTextNode* txt = new SLCTextNode ( objmat );
        txt->text = "1";
        txt->pos = vec3f ( 20, -5, 2 );
        txt->scale = 4;
        sinfo.cntnode = txt;

        SLCTextNode* txt1 = new SLCTextNode ( objmat );
        stringstream ss;
        ss << "2 X 10";
        txt1->text = ss.str();
        txt1->pos = vec3f ( 20, -10, 2 );
        txt1->scale = 4;
        sinfo.sizenode = txt1;

        SLCRectNode* rc2 = new SLCRectNode ( objmat3 );
        rc2->pnts[0] = vec2f ( 20,  -15 );
        rc2->pnts[1] = vec2f ( 30, -15 );
        rc2->pnts[2] = vec2f ( 30, -13 );
        rc2->pnts[3] = vec2f ( 20,  -13 );
        rc2->z = 2;
        sinfo.colornode = rc2;
    }

    {
        SLCRectNode* rc1 = new SLCRectNode ( objmat4 );
        rc1->pnts[0] = vec2f ( 30, 0 );
        rc1->pnts[1] = vec2f ( 40, 0 );
        rc1->pnts[2] = vec2f ( 40, 7 );
        rc1->pnts[3] = vec2f ( 30, 7 );
        rc1->z = 2;
        shapesInfo.insert ( pair<int,ShapeInfo>(shapeid,ShapeInfo()) );
        ShapeInfo& sinfo = shapesInfo[shapeid];
        sinfo.node = rc1;
        shapeid ++;

        SLCTextNode* txt = new SLCTextNode ( objmat );
        txt->text = "1";
        txt->pos = vec3f ( 30, -5, 2 );
        txt->scale = 4;
        sinfo.cntnode = txt;

        SLCTextNode* txt1 = new SLCTextNode ( objmat );
        stringstream ss;
        ss << "10 X 7";
        txt1->text = ss.str();
        txt1->pos = vec3f ( 30, -10, 2 );
        txt1->scale = 4;
        sinfo.sizenode = txt1;

        SLCRectNode* rc2 = new SLCRectNode ( objmat4 );
        rc2->pnts[0] = vec2f ( 30,  -15 );
        rc2->pnts[1] = vec2f ( 40, -15 );
        rc2->pnts[2] = vec2f ( 40, -13 );
        rc2->pnts[3] = vec2f ( 30,  -13 );
        rc2->z = 2;
        sinfo.colornode = rc2;
    }

    float w = 500;
    float h = 1000;
    plate = new SLCRectNode ( platemat );
    plate->pnts[0] = vec2f ( 0, 0 );
    plate->pnts[1] = vec2f ( w, 0 );
    plate->pnts[2] = vec2f ( w, h );
    plate->pnts[3] = vec2f ( 0, h );
    plate->z = -1;

    {
        SLCTextNode* txt = new SLCTextNode ( objmat );
        txt->text = "USERATE:";
        txt->pos = vec3f ( -55, 0, 2 );
        txt->scale = 4;
        plateInfo.userateNode = txt;

        SLCTextNode* txt1 = new SLCTextNode ( objmat );
        txt1->text = "SIZE:";
        txt1->pos = vec3f ( -55, -5, 2 );
        txt1->scale = 4;
        plateInfo.sizeNode = txt1;

        SLCTextNode* txt2 = new SLCTextNode ( objmat );
        txt2->text = "MARGIN:";
        txt2->pos = vec3f ( -55, -10, 2 );
        txt2->scale = 4;
        plateInfo.marginNode = txt2;

        SLCTextNode* txt3 = new SLCTextNode ( objmat );
        txt3->text = "COLOR:";
        txt3->pos = vec3f ( -55, -15, 2 );
        txt3->scale = 4;
        plateInfo.colorNode = txt3;
    }
    {
        SLCTextNode* txt = new SLCTextNode ( objmat );
        txt->text = "95%";
        txt->pos = vec3f ( -40, 0, 2 );
        txt->scale = 4;
        plateInfo.userateValueNode = txt;

        SLCTextNode* txt1 = new SLCTextNode ( objmat );
        stringstream ss;
        ss << w << " x " << h;
        txt1->text = ss.str();
        txt1->pos = vec3f ( -40, -5, 2 );
        txt1->scale = 4;
        plateInfo.sizeValueNode = txt1;

        SLCTextNode* txt2 = new SLCTextNode ( objmat );
        txt2->text = "10";
        txt2->pos = vec3f ( -40, -10, 2 );
        txt2->scale = 4;
        plateInfo.marginValueNode = txt2;

        SLCRectNode* rc2 = new SLCRectNode ( objmat4 );
        rc2->pnts[0] = vec2f ( -40, -15 );
        rc2->pnts[1] = vec2f (  -30, -15 );
        rc2->pnts[2] = vec2f (  -30, -13 );
        rc2->pnts[3] = vec2f ( -40, -13 );
        rc2->z = 2;
        plateInfo.colorValueNode = rc2;
    }

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

    //delete plate;
    //delete objmat;
    //delete objmat1;
    //delete objmat2;
    //delete objmat3;
    //delete objmat4;
    //delete platemat;
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
        stringstream ss;
        ss << count;
        sinfo.cntnode->text = ss.str();
    }
}

void LayoutDocument::setPlateSize ( float width, float height )
{
    plate->setSize ( width, height );
}

void LayoutDocument::saveShapeXMLFile ( const char* filename )
{
    list<SLCNode*> nodes;
    // create scene node
    SLCSceneNode scene ( "shapes_scene");
    scene.addChild ( objmat );
    scene.addChild ( objmat1 );
    scene.addChild ( objmat2 );
    scene.addChild ( objmat3 );
    scene.addChild ( objmat4 );
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
    // add shapes to scene
    for ( map<int, ShapeInfo>::iterator pp=shapesInfo.begin();
          pp!=shapesInfo.end(); ++pp )
    {
        ShapeInfo& sinfo = pp->second;

        SLCTransformNode* t1 = new SLCTransformNode ();
        mat_translatematrix ( t1->mat, x, y, 0 );
        nodes.push_back ( t1 );
        lodpage->addChild ( t1 );

	int primitivecnt = atoi ( sinfo.cntnode->text.c_str() );
	for ( int i=0; i<primitivecnt; i++ )
        {
            if ( sinfo.node == NULL )
                continue;
            SLCPrimitiveNode* node = sinfo.node->copy();
            nodes.push_back ( node );
            t1->addChild ( node );
        }

        {
            if ( sinfo.cntnode )
            {
                SLCPrimitiveNode* node = sinfo.cntnode->copy();
                nodes.push_back ( node );
                t1->addChild ( node );
            }
        }

        {
            if ( sinfo.sizenode )
            {
                SLCPrimitiveNode* node = sinfo.sizenode->copy();
                nodes.push_back ( node );
                t1->addChild ( node );
            }
        }

        float minmax[6];
        sinfo.node->getMinMax ( minmax );
//        x += minmax[3] - minmax[0];
        x += 15;//minmax[3] - minmax[0];
        //      y += minmax[4] - minmax[1];
    }

    // output to xml
    ofstream o;
    o.open ( filename );
    o << scene.toXML();
    o.close();

    //SLCNode2LC node2lc ( &scene );
    //node2lc.convert ( filename );

    // delete nodes
    for ( list<SLCNode*>::iterator pp=nodes.begin(); pp!=nodes.end(); ++pp )
        delete *pp;
}


void LayoutDocument::saveShapeSLCFile ( const char* filename )
{
    list<SLCNode*> nodes;
    // create scene node
    SLCSceneNode scene ( "shapes_scene");
    scene.addChild ( objmat );
    scene.addChild ( objmat1 );
    scene.addChild ( objmat2 );
    scene.addChild ( objmat3 );
    scene.addChild ( objmat4 );
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

    // add plateinfo to scene
    if ( plateInfo.userateNode )
    {
        SLCPrimitiveNode* node = plateInfo.userateNode->copy();
        nodes.push_back ( node );
        lodpage->addChild ( node );
    }
    if ( plateInfo.sizeNode )
    {
        SLCPrimitiveNode* node = plateInfo.sizeNode->copy();
        nodes.push_back ( node );
        lodpage->addChild ( node );
    }
    if ( plateInfo.marginNode )
    {
        SLCPrimitiveNode* node = plateInfo.marginNode->copy();
        nodes.push_back ( node );
        lodpage->addChild ( node );
    }
    if ( plateInfo.colorNode )
    {
        SLCPrimitiveNode* node = plateInfo.colorNode->copy();
        nodes.push_back ( node );
        lodpage->addChild ( node );
    }
    if ( plateInfo.userateValueNode )
    {
        SLCPrimitiveNode* node = plateInfo.userateValueNode->copy();
        nodes.push_back ( node );
        lodpage->addChild ( node );
    }
    if ( plateInfo.sizeValueNode )
    {
        SLCPrimitiveNode* node = plateInfo.sizeValueNode->copy();
        nodes.push_back ( node );
        lodpage->addChild ( node );
    }
    if ( plateInfo.marginValueNode )
    {
        SLCPrimitiveNode* node = plateInfo.marginValueNode->copy();
        nodes.push_back ( node );
        lodpage->addChild ( node );
    }
    if ( plateInfo.colorValueNode )
    {
        SLCPrimitiveNode* node = plateInfo.colorValueNode->copy();
        nodes.push_back ( node );
        lodpage->addChild ( node );
    }

    float x = 0;
    float y = 0;
    // add shapes to scene
    for ( map<int, ShapeInfo>::iterator pp=shapesInfo.begin();
          pp!=shapesInfo.end(); ++pp )
    {
        ShapeInfo& sinfo = pp->second;

        SLCTransformNode* t1 = new SLCTransformNode ();
        mat_translatematrix ( t1->mat, x, y, 0 );
        nodes.push_back ( t1 );
        lodpage->addChild ( t1 );

        {
            if ( sinfo.node == NULL )
                continue;
            SLCPrimitiveNode* node = sinfo.node->copy();
            nodes.push_back ( node );
            t1->addChild ( node );
        }

        {
            if ( sinfo.cntnode )
            {
                SLCPrimitiveNode* node = sinfo.cntnode->copy();
                nodes.push_back ( node );
                t1->addChild ( node );
            }
        }

        {
            if ( sinfo.sizenode )
            {
                SLCPrimitiveNode* node = sinfo.sizenode->copy();
                nodes.push_back ( node );
                t1->addChild ( node );
            }
        }

        {
            if ( sinfo.colornode )
            {
                SLCPrimitiveNode* node = sinfo.colornode->copy();
                nodes.push_back ( node );
                t1->addChild ( node );
            }
        }

        float minmax[6];
        sinfo.node->getMinMax ( minmax );
//        x += minmax[3] - minmax[0];
        x += 15;//minmax[3] - minmax[0];
        //      y += minmax[4] - minmax[1];
    }

    //// output to xml
    //ofstream o;
    //o.open ( filename );
    //o << scene.toXML();
    //o.close();

    SLCNode2LC node2lc ( &scene );
    node2lc.convert ( filename );

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


