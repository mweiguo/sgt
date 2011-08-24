#include <iostream>
#include <sstream>
#include <ctime>
using namespace std;
#include "sgr_nodes.h"
#include "sgr_lc.h"
#include "sgr_node2lc.h"


string getContent ( int type, int idx, LC& lc )
{
    stringstream ss;
    switch ( type )
    {
    case SLC_SCENE:
        ss << "scenename : " << lc.sceneEntry->LCRecords[idx].name;
        break;
    case SLC_LAYER:
        ss << "layername : " << lc.layerEntry->LCRecords[idx].name;
        break;
    case SLC_LOD:
        ss << "lod ";
        break;
    case SLC_LODPAGE:
        ss << "lodpage ";
        break;
    case SLC_PLINE:
        break;
    case SLC_POLY:
        break;
    case SLC_LINE:
    {
        LineRecord& line = lc.lineEntry->LCRecords[idx];
        vec2f& p0 = line.data[0];
        vec2f& p1 = line.data[1];
        ss << "line : (" << p0.x() << ", " << p0.y() << ")  (" << p1.x() << ", " << p1.y() << ")";
        break;
    }
    case SLC_TRIANGLE:
    {
        TriangleRecord& tri = lc.triangleEntry->LCRecords[idx];
        vec2f& p0 = tri.data[0];
        vec2f& p1 = tri.data[1];
        vec2f& p2 = tri.data[2];
        ss << "triangle : (" << 
            p0.x() << ", " << p0.y() << ") (" <<
            p1.x() << ", " << p1.y() << ") (" << 
            p2.x() << ", " << p2.y() << ")";
        break;
    }
    case SLC_RECT:
    {
        RectRecord& quad = lc.rectEntry->LCRecords[idx];
        vec2f& p0 = quad.data[0];
        vec2f& p1 = quad.data[1];
        vec2f& p2 = quad.data[2];
        vec2f& p3 = quad.data[3];
        ss << "rect : (" << 
            p0.x() << ", " << p0.y() << ") (" <<
            p1.x() << ", " << p1.y() << ") (" << 
            p2.x() << ", " << p2.y() << ") (" << 
            p3.x() << ", " << p3.y() << ")";
        break;
    }
    case SLC_MATERIAL:
    {
        MaterialRecord& mr = lc.materialEntry->LCRecords[idx];
        ss << "name : " << mr.name <<
            ", backgroud_color ( " << mr.backgroud_color.x() << "," << mr.backgroud_color.y() << "," << mr.backgroud_color.z() <<
            " ), foregroud_color ( " << mr.foregroud_color.x() << "," << mr.foregroud_color.y() << "," << mr.foregroud_color.z() <<
            " ), linewidth = " << mr.linewidth;
        switch ( mr.linetype )
        {
        case MaterialRecord::LINETYPE_SOLID:
            ss << "solid";
            break;
        case MaterialRecord::LINETYPE_DASH:
            ss << "dash";
            break;
        }
        break;
    }
    default:
        ss << "unsupport type " << type;
        break;
    }
    return ss.str();
}

int cscene = 0;
int cmat   = 0;
int clayer = 0;
int clod   = 0;
int clodpage = 0;
int cpline = 0;
int cpoly  = 0;
int cline  = 0;
int ctri   = 0;
int cquad  = 0;

void counter ( int type )
{
    switch ( type ) {
    case SLC_SCENE:
        cscene++;
        break;
    case SLC_LOD:
        clod++;
        break;
    case SLC_LODPAGE:
        clodpage++;
        break;
    case SLC_LAYER:
        clayer++;
        break;
    case SLC_PLINE:
        cpline++;
        break;
    case SLC_POLY:
        cpoly++;
        break;
    case SLC_LINE:
        cline++;
        break;
    case SLC_TRIANGLE:
        ctri++;
        break;
    case SLC_RECT:
        cquad++;
        break;
    case SLC_MATERIAL:
        cmat++;
        break;
    }
}

void printCounter ()
{
    cout << "SLC_SCENE    count : " << cscene << endl;
    cout << "SLC_MATERIAL count : " << cmat << endl;
    cout << "SLC_LAYER    count : " << clayer << endl;
    cout << "SLC_LOD      count : " << clod << endl;
    cout << "SLC_LODPAGE  count : " << clodpage << endl;
    cout << "SLC_PLINE    count : " << cpline << endl;
    cout << "SLC_POLY     count : " << cpoly << endl;
    cout << "SLC_LINE     count : " << cline << endl;
    cout << "SLC_TRIANGLE count : " << ctri << endl;
    cout << "SLC_QUAD     count : " << cquad << endl;
}

void traverse ( LC& lc )
{
    if ( lc.toElement ( FIRST_CHILD ) >= 0 ) {
        counter ( lc.getType() );
//         string prefix = "";
//         for ( int i=0; i<lc.getDepth(); i++ )
//             prefix += "    ";
//         cout << prefix << "type : " << lc.getTypeStr() << ", depth : " << lc.getDepth() << ", " << getContent(lc.getType(), lc.getValue(), lc ) << endl;
	traverse ( lc );
        while ( lc.toElement ( NEXT_SIBLING )>=0 ) {
            counter ( lc.getType() );
//             string prefix = "";
//             for ( int i=0; i<lc.getDepth(); i++ )
//                 prefix += "    ";
//             cout << prefix << "type : " << lc.getTypeStr() << ", depth : " << lc.getDepth() << ", " << getContent(lc.getType(), lc.getValue(), lc ) << endl;
	    traverse ( lc );
        }
        lc.toElement ( PARENT );
    }
}



int main ( int argc, char* argv[] )
{
    list<SLCNode*> nodes;
    SLCSceneNode scene ("test_scene");
    for ( int i=0; i<25e4; i++ )
    {
        SLCMaterial* mat_layer = new SLCMaterial ("layer_material");
        mat_layer->foreground_color = vec3i(155, 0, 0);
        mat_layer->background_color = vec3i(0, 0, 155);
        mat_layer->linetype = SLCMaterial::LINETYPE_SOLID;
        mat_layer->linewidth = 0;
        SLCMaterial* mat = new SLCMaterial ( "mat" );
        mat->foreground_color = vec3i(55, 0, 0);
        mat->background_color = vec3i(0, 44, 155);
        mat->linetype = SLCMaterial::LINETYPE_DASH;
        mat->linewidth = 1;
        SLCLayerNode* layer = new SLCLayerNode ( "layer1", mat_layer );
        SLCLODNode* lod = new SLCLODNode();
        SLCLODPageNode* lodpage = new SLCLODPageNode();
        lodpage->delayloading = false;
        lodpage->kdtree = "./lodpage1.xml";
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
        SLCLODPageNode* lodpage2 = new SLCLODPageNode();
        lodpage2->delayloading = true;
        lodpage2->kdtree = "./lodpage2.xml";
        lodpage2->imposter = false;

        SLCLayerNode* layer2 = new SLCLayerNode ( "layer2", mat_layer );
        SLCLODNode* lod2 = new SLCLODNode ();
        SLCLODPageNode* lodpage3 = new SLCLODPageNode();
        lodpage3->delayloading = false;
        lodpage3->kdtree = "./lodpage3.xml";
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
        nodes.push_back ( lodpage2 );
        nodes.push_back ( layer2 );
        nodes.push_back ( lod2 );
        nodes.push_back ( lodpage3 );
        nodes.push_back ( rc4 );
    }

    SLCNode2LC node2lc ( &scene );
    LC* lc = node2lc.generateLC ();
    lc->saveLC ( "geos.slc" );
    node2lc.freeLC ( lc );

    for ( list<SLCNode*>::iterator pp=nodes.begin(); pp!=nodes.end(); ++pp )
        delete *pp;

    
    LC nlc;
    clock_t t = clock();
    nlc.loadLC ( "geos.slc" );
    cout << "parse finished, elapse " << clock() - t << "(ms)" << endl;
    t = clock();
    traverse ( nlc );
    nlc.freeLC();
    cout << "traverse finished, elapse " << clock() - t << "(ms)" << endl;
    printCounter ();
    return 0;
}
