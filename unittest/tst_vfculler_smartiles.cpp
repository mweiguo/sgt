#include "gtest/gtest.h"

#include <sgr_lc.h>
#include <sgr_node2lc.h>
#include <sgr_nodes.h>
#include <sgr_bboxupdater.h>
#include <sgr_vfculler.h>

// ********************************************************************************

class VFCullerSmartTilesTestCase : public ::testing::Test
{
protected:
    VFCullerSmartTilesTestCase ()
    {
        initLC ();
        list<SLCNode*> nodes;
        SLCSceneNode scene ("test_scene");
        {
            SLCMaterial* mat_layer = new SLCMaterial ("layer_material");
            mat_layer->foreground_color = vec3i(155, 0, 0);
            mat_layer->background_color = vec3i(0, 0, 155);
            mat_layer->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
            mat_layer->linewidth = 0;
//            mat_layer->fontfilename = "simsun.ttc";

            SLCLayerNode* layer = new SLCLayerNode ( "layer11", mat_layer );
            SLCSmartTilesNode* tiles = new SLCSmartTilesNode ( mat_layer );
            tiles->pnts[0] = vec2f ( 0, 0 );
            tiles->pnts[1] = vec2f ( 25600, 0 );
            tiles->pnts[2] = vec2f ( 25600, 25600 );
            tiles->pnts[3] = vec2f ( 0, 25600 );
            tiles->z = 0;
            tiles->levelcnt = 5;
  
            scene.addChild ( mat_layer );
            scene.addChild ( layer );
            layer->addChild ( tiles );

            nodes.push_back ( mat_layer );
            nodes.push_back ( layer );
            nodes.push_back ( tiles );
        }

        SLCNode2LC node2lc ( &scene );
        node2lc.convert ( "test.slc" );
        lc.load ( "test.slc" );

    }
    ~VFCullerSmartTilesTestCase ()
    {
        lc.free();
    }

    LC lc;
};

// ================================================================================

TEST_F ( VFCullerSmartTilesTestCase, GetTiles_level0 )
{
    vec2f min = vec2f ( 0, 0 );
    vec2f max = vec2f ( 25700, 25800 );
    lc.toElement ( ROOT );
    vfculler::cull ( lc, BBox2d(min, max) );

    EXPECT_EQ ( 1u, vfculler::renderObjects.size() );
    SmartTileRecord& smartile = lc.smartTilesEntry->LCRecords[ lc.globalLCEntry->LCRecords[vfculler::renderObjects.front()].value ];
    EXPECT_EQ ( 1, smartile.tilecnt );

    Tile* tile = smartile.tiles;
    EXPECT_NE ( NULL, (int)tile );
    
    EXPECT_EQ ( 0,     tile->minmax[0] );
    EXPECT_EQ ( 0,     tile->minmax[1] );
    EXPECT_EQ ( 25600, tile->minmax[2] );
    EXPECT_EQ ( 25600, tile->minmax[3] );

    EXPECT_EQ ( 0u, tile->id );
    EXPECT_EQ ( 0u, tile->level );
    EXPECT_STREQ ( "0/0/0/0/0.png", tile->filename );

    vfculler::renderObjects.clear();
}

// ================================================================================

TEST_F ( VFCullerSmartTilesTestCase, GetTiles_level1 )
{
    vec2f min = vec2f ( 0, 0 );
    vec2f max = vec2f ( 25000, 25000 );
    lc.toElement ( ROOT );
    vfculler::cull ( lc, BBox2d(min, max) );

    EXPECT_EQ ( 1u, vfculler::renderObjects.size() );
    SmartTileRecord& smartile = lc.smartTilesEntry->LCRecords[ lc.globalLCEntry->LCRecords[vfculler::renderObjects.front()].value ];
    EXPECT_EQ ( 4, smartile.tilecnt );

    Tile* tile = smartile.tiles;

    EXPECT_NE ( NULL,  (int)tile );
    EXPECT_EQ ( 1u,    tile->level );
    EXPECT_EQ ( 0u,    tile->id );
    EXPECT_EQ ( 0,     tile->minmax[0] );
    EXPECT_EQ ( 0,     tile->minmax[1] );
    EXPECT_EQ ( 12800, tile->minmax[2] );
    EXPECT_EQ ( 12800, tile->minmax[3] );
    EXPECT_STREQ ( "1/0/0/0/1.png", tile->filename );

    tile++;
    EXPECT_NE ( NULL,  (int)tile );
    EXPECT_EQ ( 1u,    tile->level );
    EXPECT_EQ ( 1u,    tile->id );
    EXPECT_EQ ( 12800, tile->minmax[0] );
    EXPECT_EQ ( 0,     tile->minmax[1] );
    EXPECT_EQ ( 25600, tile->minmax[2] );
    EXPECT_EQ ( 12800, tile->minmax[3] );
    EXPECT_STREQ ( "1/0/1/0/1.png", tile->filename );

    tile++;
    EXPECT_NE ( NULL,  (int)tile );
    EXPECT_EQ ( 1u,    tile->level );
    EXPECT_EQ ( 2u,    tile->id );
    EXPECT_EQ ( 0,     tile->minmax[0] );
    EXPECT_EQ ( 12800, tile->minmax[1] );
    EXPECT_EQ ( 12800, tile->minmax[2] );
    EXPECT_EQ ( 25600, tile->minmax[3] );
    EXPECT_STREQ ( "1/0/0/0/0.png", tile->filename );

    tile++;
    EXPECT_NE ( NULL,  (int)tile );
    EXPECT_EQ ( 1u,    tile->level );
    EXPECT_EQ ( 3u,    tile->id );
    EXPECT_EQ ( 12800, tile->minmax[0] );
    EXPECT_EQ ( 12800, tile->minmax[1] );
    EXPECT_EQ ( 25600, tile->minmax[2] );
    EXPECT_EQ ( 25600, tile->minmax[3] );
    EXPECT_STREQ ( "1/0/1/0/0.png", tile->filename );

    vfculler::renderObjects.clear();
}

// ================================================================================

TEST_F ( VFCullerSmartTilesTestCase, GetTiles_level2 )
{
    vec2f min = vec2f ( 0, 0 );
    vec2f max = vec2f ( 12800, 12800 );
    lc.toElement ( ROOT );
    vfculler::cull ( lc, BBox2d(min, max) );

    EXPECT_EQ ( 1u, vfculler::renderObjects.size() );
    SmartTileRecord& smartile = lc.smartTilesEntry->LCRecords[ lc.globalLCEntry->LCRecords[vfculler::renderObjects.front()].value ];
    EXPECT_EQ ( 4, smartile.tilecnt );

    Tile* tile = smartile.tiles;

    EXPECT_NE ( NULL,  (int)tile );
    EXPECT_EQ ( 2u,    tile->level );
    EXPECT_EQ ( 0u,    tile->id );
    EXPECT_EQ ( 0,     tile->minmax[0] );
    EXPECT_EQ ( 0,     tile->minmax[1] );
    EXPECT_EQ ( 6400,  tile->minmax[2] );
    EXPECT_EQ ( 6400,  tile->minmax[3] );
    EXPECT_STREQ ( "2/0/0/0/3.png", tile->filename );

    tile++;
    EXPECT_NE ( NULL,  (int)tile );
    EXPECT_EQ ( 2u,    tile->level );
    EXPECT_EQ ( 1u,    tile->id );
    EXPECT_EQ ( 6400,  tile->minmax[0] );
    EXPECT_EQ ( 0,     tile->minmax[1] );
    EXPECT_EQ ( 12800, tile->minmax[2] );
    EXPECT_EQ ( 6400,  tile->minmax[3] );
    EXPECT_STREQ ( "2/0/1/0/3.png", tile->filename );

    tile++;
    EXPECT_NE ( NULL,  (int)tile );
    EXPECT_EQ ( 2u,    tile->level );
    EXPECT_EQ ( 4u,    tile->id );
    EXPECT_EQ ( 0,     tile->minmax[0] );
    EXPECT_EQ ( 6400,  tile->minmax[1] );
    EXPECT_EQ ( 6400,  tile->minmax[2] );
    EXPECT_EQ ( 12800, tile->minmax[3] );
    EXPECT_STREQ ( "2/0/0/0/2.png", tile->filename );

    tile++;
    EXPECT_NE ( NULL,  (int)tile );
    EXPECT_EQ ( 2u,    tile->level );
    EXPECT_EQ ( 5u,    tile->id );
    EXPECT_EQ ( 6400,  tile->minmax[0] );
    EXPECT_EQ ( 6400,  tile->minmax[1] );
    EXPECT_EQ ( 12800, tile->minmax[2] );
    EXPECT_EQ ( 12800, tile->minmax[3] );
    EXPECT_STREQ ( "2/0/1/0/2.png", tile->filename );

    vfculler::renderObjects.clear();
}

// ================================================================================

TEST_F ( VFCullerSmartTilesTestCase, GetTiles_level3 )
{
    vec2f min = vec2f ( 0, 0 );
    vec2f max = vec2f ( 6400, 6400 );
    lc.toElement ( ROOT );
    vfculler::cull ( lc, BBox2d(min, max) );

    EXPECT_EQ ( 1u, vfculler::renderObjects.size() );
    SmartTileRecord& smartile = lc.smartTilesEntry->LCRecords[ lc.globalLCEntry->LCRecords[vfculler::renderObjects.front()].value ];
    EXPECT_EQ ( 4, smartile.tilecnt );

    Tile* tile = smartile.tiles;

    EXPECT_NE ( NULL,  (int)tile );
    EXPECT_EQ ( 3u,    tile->level );
    EXPECT_EQ ( 0u,    tile->id );
    EXPECT_EQ ( 0,     tile->minmax[0] );
    EXPECT_EQ ( 0,     tile->minmax[1] );
    EXPECT_EQ ( 3200,  tile->minmax[2] );
    EXPECT_EQ ( 3200,  tile->minmax[3] );
    EXPECT_STREQ ( "3/0/0/0/7.png", tile->filename );

    tile++;
    EXPECT_NE ( NULL,  (int)tile );
    EXPECT_EQ ( 3u,    tile->level );
    EXPECT_EQ ( 1u,    tile->id );
    EXPECT_EQ ( 3200,  tile->minmax[0] );
    EXPECT_EQ ( 0,     tile->minmax[1] );
    EXPECT_EQ ( 6400,  tile->minmax[2] );
    EXPECT_EQ ( 3200,  tile->minmax[3] );
    EXPECT_STREQ ( "3/0/1/0/7.png", tile->filename );

    tile++;
    EXPECT_NE ( NULL,  (int)tile );
    EXPECT_EQ ( 3u,    tile->level );
    EXPECT_EQ ( 8u,    tile->id );
    EXPECT_EQ ( 0,     tile->minmax[0] );
    EXPECT_EQ ( 3200,  tile->minmax[1] );
    EXPECT_EQ ( 3200,  tile->minmax[2] );
    EXPECT_EQ ( 6400,  tile->minmax[3] );
    EXPECT_STREQ ( "3/0/0/0/6.png", tile->filename );

    tile++;
    EXPECT_NE ( NULL,  (int)tile );
    EXPECT_EQ ( 3u,    tile->level );
    EXPECT_EQ ( 9u,    tile->id );
    EXPECT_EQ ( 3200,  tile->minmax[0] );
    EXPECT_EQ ( 3200,  tile->minmax[1] );
    EXPECT_EQ ( 6400,  tile->minmax[2] );
    EXPECT_EQ ( 6400,  tile->minmax[3] );
    EXPECT_STREQ ( "3/0/1/0/6.png", tile->filename );

    vfculler::renderObjects.clear();
}

// ================================================================================

TEST_F ( VFCullerSmartTilesTestCase, GetTiles_level4 )
{
    vec2f min = vec2f ( 0, 0 );
    vec2f max = vec2f ( 3200, 3200 );
    lc.toElement ( ROOT );
    vfculler::cull ( lc, BBox2d(min, max) );

    EXPECT_EQ ( 1u, vfculler::renderObjects.size() );
    SmartTileRecord& smartile = lc.smartTilesEntry->LCRecords[ lc.globalLCEntry->LCRecords[vfculler::renderObjects.front()].value ];
    EXPECT_EQ ( 4, smartile.tilecnt );

    Tile* tile = smartile.tiles;

    EXPECT_NE ( NULL,  (int)tile );
    EXPECT_EQ ( 4u,    tile->level );
    EXPECT_EQ ( 0u,    tile->id );
    EXPECT_EQ ( 0,     tile->minmax[0] );
    EXPECT_EQ ( 0,     tile->minmax[1] );
    EXPECT_EQ ( 1600,  tile->minmax[2] );
    EXPECT_EQ ( 1600,  tile->minmax[3] );
    EXPECT_STREQ ( "4/0/0/0/15.png", tile->filename );

    tile++;
    EXPECT_NE ( NULL,  (int)tile );
    EXPECT_EQ ( 4u,    tile->level );
    EXPECT_EQ ( 1u,    tile->id );
    EXPECT_EQ ( 1600,  tile->minmax[0] );
    EXPECT_EQ ( 0,     tile->minmax[1] );
    EXPECT_EQ ( 3200,  tile->minmax[2] );
    EXPECT_EQ ( 1600,  tile->minmax[3] );
    EXPECT_STREQ ( "4/0/1/0/15.png", tile->filename );

    tile++;
    EXPECT_NE ( NULL,  (int)tile );
    EXPECT_EQ ( 4u,    tile->level );
    EXPECT_EQ ( 16u,   tile->id );
    EXPECT_EQ ( 0,     tile->minmax[0] );
    EXPECT_EQ ( 1600,  tile->minmax[1] );
    EXPECT_EQ ( 1600,  tile->minmax[2] );
    EXPECT_EQ ( 3200,  tile->minmax[3] );
    EXPECT_STREQ ( "4/0/0/0/14.png", tile->filename );

    tile++;
    EXPECT_NE ( NULL,  (int)tile );
    EXPECT_EQ ( 4u,    tile->level );
    EXPECT_EQ ( 17u,   tile->id );
    EXPECT_EQ ( 1600,  tile->minmax[0] );
    EXPECT_EQ ( 1600,  tile->minmax[1] );
    EXPECT_EQ ( 3200,  tile->minmax[2] );
    EXPECT_EQ ( 3200,  tile->minmax[3] );
    EXPECT_STREQ ( "4/0/1/0/14.png", tile->filename );

    vfculler::renderObjects.clear();
}

// ================================================================================

TEST_F ( VFCullerSmartTilesTestCase, GetTiles_level5 )
{
    vec2f min = vec2f ( 0, 0 );
    vec2f max = vec2f ( 1600, 1600 );
    lc.toElement ( ROOT );
    vfculler::cull ( lc, BBox2d(min, max) );

    EXPECT_EQ ( 1u, vfculler::renderObjects.size() );
    SmartTileRecord& smartile = lc.smartTilesEntry->LCRecords[ lc.globalLCEntry->LCRecords[vfculler::renderObjects.front()].value ];
    EXPECT_EQ ( 1, smartile.tilecnt );

    Tile* tile = smartile.tiles;

    EXPECT_NE ( NULL,  (int)tile );
    EXPECT_EQ ( 4u,    tile->level );
    EXPECT_EQ ( 0u,    tile->id );
    EXPECT_EQ ( 0,     tile->minmax[0] );
    EXPECT_EQ ( 0,     tile->minmax[1] );
    EXPECT_EQ ( 1600,  tile->minmax[2] );
    EXPECT_EQ ( 1600,  tile->minmax[3] );
    EXPECT_STREQ ( "4/0/0/0/15.png", tile->filename );

    vfculler::renderObjects.clear();
}

// ================================================================================

