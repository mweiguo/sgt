#include "gtest/gtest.h"

#include <sgr_lc.h>
#include <sgr_node2lc.h>
#include <sgr_nodes.h>
#include <sgr_bboxupdater.h>

// ********************************************************************************

class BBoxUpdaterTestCase1 : public ::testing::Test
{
protected:
    BBoxUpdaterTestCase1 ()
    {
	// build scene data
	list<SLCNode*> nodes;
	SLCSceneNode scene ("test_scene");
	SLCMaterial* mat_layer = new SLCMaterial ("layer_material");
	mat_layer->foreground_color = vec3i(155, 0, 0);
	mat_layer->background_color = vec3i(0, 0, 155);
	mat_layer->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
	mat_layer->linewidth = 0;
	SLCMaterial* mat = new SLCMaterial ( "mat" );
	mat->foreground_color = vec3i(55, 0, 0);
	mat->background_color = vec3i(0, 44, 155);
	mat->linetype = 0xFFFF;//SLCMaterial::LINETYPE_DASH;
	mat->linewidth = 1;
	SLCLayerNode* layer = new SLCLayerNode ( "layer1", mat_layer );
	SLCLODNode* lod = new SLCLODNode();
	SLCLODPageNode* lodpage = new SLCLODPageNode();
	lodpage->delayloading = false;
	lodpage->imposter = true;
	SLCRectNode* rc1 = new SLCRectNode ( mat );
	rc1->pnts[0] = vec2f (0, 0); rc1->pnts[1] = vec2f (1, 0);
	rc1->pnts[2] = vec2f (1, 1); rc1->pnts[3] = vec2f (0, 1);
	SLCRectNode* rc2 = new SLCRectNode ( mat );
	rc2->pnts[0] = vec2f (1, 0); rc2->pnts[1] = vec2f (2, 0);
	rc2->pnts[2] = vec2f (2, 1); rc2->pnts[3] = vec2f (1, 1);
	SLCRectNode* rc3 = new SLCRectNode ( mat_layer );
	rc3->pnts[0] = vec2f (2, 0); rc3->pnts[1] = vec2f (3, 0);
	rc3->pnts[2] = vec2f (3, 1); rc3->pnts[3] = vec2f (2, 1);
	SLCRectNode* rc4 = new SLCRectNode ( mat_layer );
	rc4->pnts[0] = vec2f (3, 0); rc4->pnts[1] = vec2f (4, 0);
	rc4->pnts[2] = vec2f (4, 1); rc4->pnts[3] = vec2f (3, 1);
	SLCLODPageNode* lodpage2 = new SLCLODPageNode();
	lodpage2->delayloading = true;
	lodpage2->imposter = false;
	scene.addChild ( mat_layer );
	scene.addChild ( mat );
	scene.addChild ( layer );
	layer->addChild ( lod );
	lod->addChild ( lodpage );
	lod->addChild ( lodpage2 );
	lodpage->addChild ( rc1 );
	lodpage->addChild ( rc2 );
	lodpage->addChild ( rc3 );
	lodpage->addChild ( rc4 );
	nodes.push_back ( mat_layer );
	nodes.push_back ( mat );
	nodes.push_back ( layer );
	nodes.push_back ( lod );
	nodes.push_back ( lodpage );
	nodes.push_back ( rc1 );
	nodes.push_back ( rc2 );
	nodes.push_back ( rc3 );
	nodes.push_back ( rc4 );
	nodes.push_back ( lodpage2 );
	node2lc = new SLCNode2LC ( &scene );
	_lc = node2lc->generateLC ();
	for ( list<SLCNode*>::iterator pp=nodes.begin(); pp!=nodes.end(); ++pp )
	    delete *pp;
    }
    ~BBoxUpdaterTestCase1 ()
    {
	node2lc->freeLC ( _lc );
	delete node2lc;
    }

    LC* _lc;
    SLCNode2LC* node2lc;
};

// ================================================================================

TEST_F ( BBoxUpdaterTestCase1, forward_update ) {
    // generate data
    BBox2dUpdater::forward_update ( *_lc );
    // should check all element's minmax field
    // scene
    EXPECT_EQ ( SLC_SCENE, _lc->globalLCEntry->LCRecords[0].type );
    EXPECT_EQ ( 0u, _lc->globalLCEntry->LCRecords[0].minmax[0] );
    EXPECT_EQ ( 0u, _lc->globalLCEntry->LCRecords[0].minmax[1] );
    EXPECT_EQ ( 4u, _lc->globalLCEntry->LCRecords[0].minmax[2] );
    EXPECT_EQ ( 1u, _lc->globalLCEntry->LCRecords[0].minmax[3] );
    // mat_layer
    // _lc->globalLCEntry->LCRecords[1].minmax;
    // mat
    // _lc->globalLCEntry->LCRecords[2].minmax;
    // layer
    EXPECT_EQ ( SLC_LAYER, _lc->globalLCEntry->LCRecords[3].type );
    EXPECT_EQ ( 0u, _lc->globalLCEntry->LCRecords[3].minmax[0] );
    EXPECT_EQ ( 0u, _lc->globalLCEntry->LCRecords[3].minmax[1] );
    EXPECT_EQ ( 4u, _lc->globalLCEntry->LCRecords[3].minmax[2] );
    EXPECT_EQ ( 1u, _lc->globalLCEntry->LCRecords[3].minmax[3] );
    // lod
    EXPECT_EQ ( SLC_LOD, _lc->globalLCEntry->LCRecords[4].type );
    EXPECT_EQ ( 0u, _lc->globalLCEntry->LCRecords[4].minmax[0] );
    EXPECT_EQ ( 0u, _lc->globalLCEntry->LCRecords[4].minmax[1] );
    EXPECT_EQ ( 4u, _lc->globalLCEntry->LCRecords[4].minmax[2] );
    EXPECT_EQ ( 1u, _lc->globalLCEntry->LCRecords[4].minmax[3] );
    // lodpage
    EXPECT_EQ ( SLC_LODPAGE, _lc->globalLCEntry->LCRecords[5].type );
    EXPECT_EQ ( 0u, _lc->globalLCEntry->LCRecords[5].minmax[0] );
    EXPECT_EQ ( 0u, _lc->globalLCEntry->LCRecords[5].minmax[1] );
    EXPECT_EQ ( 4u, _lc->globalLCEntry->LCRecords[5].minmax[2] );
    EXPECT_EQ ( 1u, _lc->globalLCEntry->LCRecords[5].minmax[3] );
    // rc1
    EXPECT_EQ ( SLC_RECT, _lc->globalLCEntry->LCRecords[6].type );
    EXPECT_EQ ( 0u, _lc->globalLCEntry->LCRecords[6].minmax[0] );
    EXPECT_EQ ( 0u, _lc->globalLCEntry->LCRecords[6].minmax[1] );
    EXPECT_EQ ( 1u, _lc->globalLCEntry->LCRecords[6].minmax[2] );
    EXPECT_EQ ( 1u, _lc->globalLCEntry->LCRecords[6].minmax[3] );
    // rc2
    EXPECT_EQ ( SLC_RECT, _lc->globalLCEntry->LCRecords[7].type );
    EXPECT_EQ ( 1u, _lc->globalLCEntry->LCRecords[7].minmax[0] );
    EXPECT_EQ ( 0u, _lc->globalLCEntry->LCRecords[7].minmax[1] );
    EXPECT_EQ ( 2u, _lc->globalLCEntry->LCRecords[7].minmax[2] );
    EXPECT_EQ ( 1u, _lc->globalLCEntry->LCRecords[7].minmax[3] );
    // rc3
    EXPECT_EQ ( SLC_RECT, _lc->globalLCEntry->LCRecords[8].type );
    EXPECT_EQ ( 2u, _lc->globalLCEntry->LCRecords[8].minmax[0] );
    EXPECT_EQ ( 0u, _lc->globalLCEntry->LCRecords[8].minmax[1] );
    EXPECT_EQ ( 3u, _lc->globalLCEntry->LCRecords[8].minmax[2] );
    EXPECT_EQ ( 1u, _lc->globalLCEntry->LCRecords[8].minmax[3] );
    // rc4
    EXPECT_EQ ( SLC_RECT, _lc->globalLCEntry->LCRecords[9].type );
    EXPECT_EQ ( 3u, _lc->globalLCEntry->LCRecords[9].minmax[0] );
    EXPECT_EQ ( 0u, _lc->globalLCEntry->LCRecords[9].minmax[1] );
    EXPECT_EQ ( 4u, _lc->globalLCEntry->LCRecords[9].minmax[2] );
    EXPECT_EQ ( 1u, _lc->globalLCEntry->LCRecords[9].minmax[3] );
    // lodpage2
//     _lc->globalLCEntry->LCRecords[10].minmax;
}

// ================================================================================

TEST_F ( BBoxUpdaterTestCase1, lazy_update ) {
    // should check _buildkdt->_axisstack
//    EXPECT_EQ ( 32u, _buildkdt->_axisstack.size() );
//     EXPECT_EQ ( 0, _buildkdt->_axisstack[0] );
//     EXPECT_EQ ( 0, _buildkdt->_axisstack[1] );
//     EXPECT_EQ ( 0, _buildkdt->_axisstack[2] );
//     EXPECT_EQ ( 0, _buildkdt->_axisstack[3] );
//     EXPECT_EQ ( 0, _buildkdt->_axisstack[4] );
//     EXPECT_EQ ( 0, _buildkdt->_axisstack[5] );
}

// ********************************************************************************

