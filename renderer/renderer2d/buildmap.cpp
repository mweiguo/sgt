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
    gmat->foreground_color = vec4i(255, 255, 255, 255);
    gmat->background_color = vec4i(255, 255, 255, 255);
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
    SLCSceneNode scene ( "map_scene");
    scene.addChild ( gmat );

    {
	SLCLayerNode* layer = new SLCLayerNode ( "background", gmat );
	nodes.push_back ( layer );
	scene.addChild ( layer );

        SLCSmartTilesNode* tiles = new SLCSmartTilesNode ( gmat );
        tiles->pnts[0] = vec2f ( 0, 0 );
        tiles->pnts[1] = vec2f ( 25600, 0 );
        tiles->pnts[2] = vec2f ( 25600, 25600 );
        tiles->pnts[3] = vec2f ( 0, 25600 );
        tiles->z = 1;
        tiles->levelcnt = 17;
        tiles->dbname = "satelite.db";
        layer->addChild ( tiles );
    }

    ofstream o;
    o.open ("map.xml" );
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

