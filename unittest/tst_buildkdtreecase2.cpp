#include "gtest/gtest.h"

#include <sgr_lc.h>
#include <sgr_node2lc.h>
#include <sgr_nodes.h>
#include <sgr_bboxupdater.h>
#include <sgr_lc2kdtree.h>
#include <kdtree.h>

#include <iostream>


// ********************************************************************************

class BuildKdTreeTestCase2 : public ::testing::Test
{
protected:
    BuildKdTreeTestCase2 ()
    {
	// build scene data
	list<SLCNode*> nodes;
	SLCSceneNode scene ("test_scene");
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
	lodpage->addChild ( rc4 );
	lodpage->addChild ( rc3 );
	lodpage->addChild ( rc2 );
	lodpage->addChild ( rc1 );
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

	// update bbox
	BBox2dUpdater::forward_update ( *_lc );
     
	// lc2kdtree
	_kdt = new KdTree<int>();
	LC2KDT::collectPrimitive ( _lc->getType(), _lc->getGIndex(), *_kdt );
	LC2KDT::traverse ( *_lc, *_kdt );
	// buildKdTree
	GetPrimitiveCenter getPrimitiveCenter;
	GetPrimitiveMinMax getPrimitiveMinMax;
	getPrimitiveCenter.init ( _lc );
	getPrimitiveMinMax.init ( _lc );
	option.targetnumperleaf = 1;
	option.maxlevel = 32;
	option.getPrimitiveCenter = getPrimitiveCenter;
	option.getPrimitiveMinMax = getPrimitiveMinMax;
	_buildkdt = new BuildLCKdTree ( *_kdt, option );
    }
    ~BuildKdTreeTestCase2 ()
    {
	node2lc->freeLC ( _lc );
	delete node2lc;

	delete _kdt;
	delete _buildkdt;
    }

    typedef KdTree<int>::BuildKdTree<GetPrimitiveCenter, GetPrimitiveMinMax> BuildLCKdTree;
    typedef BuildKdTreeOption<GetPrimitiveCenter, GetPrimitiveMinMax> BuildLCKdTreeOption;
    LC* _lc;
    SLCNode2LC* node2lc;
    KdTree<int>* _kdt;
    BuildLCKdTree* _buildkdt;
    BuildLCKdTreeOption option;
};

// ================================================================================

TEST_F ( BuildKdTreeTestCase2, getMinMax ) {
    // generate object indices
    generate_n ( back_inserter(_buildkdt->_objindices), _buildkdt->_kdtree->_primitives.size(), Incr() );
    // pre alloc kdnode's room
    _buildkdt->_kdtree->_nodes.reserve ( _buildkdt->_kdtree->_primitives.size() * 5 );
    // get minmax of whole scene
    float minmax[4];
    _buildkdt->getMinMax ( 0, _buildkdt->_kdtree->_primitives.size(), minmax );
    // should check minmax parameter
    EXPECT_EQ ( 0u, minmax[0] );
    EXPECT_EQ ( 0u, minmax[1] );
    EXPECT_EQ ( 4u, minmax[2] );
    EXPECT_EQ ( 1u, minmax[3] );
}

// ================================================================================

TEST_F ( BuildKdTreeTestCase2, computeDivision ) {
    // prepare data
    generate_n ( back_inserter(_buildkdt->_objindices), _buildkdt->_kdtree->_primitives.size(), Incr() );
    _buildkdt->_kdtree->_nodes.reserve ( _buildkdt->_kdtree->_primitives.size() * 5 );
    float minmax[4];
    _buildkdt->getMinMax ( 0, _buildkdt->_kdtree->_primitives.size(), minmax );
    _buildkdt->computeDivision ( minmax );
    // should check _buildkdt->_axisstack
    EXPECT_EQ ( 32u, _buildkdt->_axisstack.size() );
    EXPECT_EQ ( 0, _buildkdt->_axisstack[0] );
    EXPECT_EQ ( 0, _buildkdt->_axisstack[1] );
    for ( int i=2; i<16; i++ ) 
    {
	if ( i%2 == 0 )
	    EXPECT_EQ ( 0, _buildkdt->_axisstack[2] );
	else
	    EXPECT_EQ ( 1, _buildkdt->_axisstack[3] );
    }
}

// ================================================================================

TEST_F ( BuildKdTreeTestCase2, divide ) {
    // prepare data
    generate_n ( back_inserter(_buildkdt->_objindices), _buildkdt->_kdtree->_primitives.size(), Incr() );
    _buildkdt->_kdtree->_nodes.reserve ( _buildkdt->_kdtree->_primitives.size() * 5 );
    float minmax[4];
    _buildkdt->getMinMax ( 0, _buildkdt->_kdtree->_primitives.size(), minmax );
    _buildkdt->computeDivision ( minmax );
    transform ( _buildkdt->_kdtree->_primitives.begin(), 
		_buildkdt->_kdtree->_primitives.end(), 
		back_inserter(_buildkdt->_objcenters), 
		_buildkdt->_opt.getPrimitiveCenter );
    _buildkdt->_kdtree->addNode ( -1, _buildkdt->_kdtree->_primitives.size() );
    _buildkdt->divide ();

    // should check _buildkdt->_objcenters
    EXPECT_EQ ( 4u, _buildkdt->_objcenters.size() );
    EXPECT_EQ ( 3.5f, _buildkdt->_objcenters[0].x() );
    EXPECT_EQ ( 0.5f, _buildkdt->_objcenters[0].y() );
    EXPECT_EQ ( 2.5f, _buildkdt->_objcenters[1].x() );
    EXPECT_EQ ( 0.5f, _buildkdt->_objcenters[1].y() );
    EXPECT_EQ ( 1.5f, _buildkdt->_objcenters[2].x() );
    EXPECT_EQ ( 0.5f, _buildkdt->_objcenters[2].y() );
    EXPECT_EQ ( 0.5f, _buildkdt->_objcenters[3].x() );
    EXPECT_EQ ( 0.5f, _buildkdt->_objcenters[3].y() );
    // should check _buildkdt->_objindices
    EXPECT_EQ ( 4u, _buildkdt->_objindices.size() );
    EXPECT_EQ ( 3, _buildkdt->_objindices[0] );
    EXPECT_EQ ( 2, _buildkdt->_objindices[1] );
    EXPECT_EQ ( 1, _buildkdt->_objindices[2] );
    EXPECT_EQ ( 0, _buildkdt->_objindices[3] );
    // should check _buildkdt->_kdtree->_nodes
    EXPECT_EQ ( 7u, _buildkdt->_kdtree->_nodes.size() );
    EXPECT_EQ ( 1, _buildkdt->_kdtree->_nodes[0].first );
    EXPECT_EQ ( 2, _buildkdt->_kdtree->_nodes[0].second );

    EXPECT_EQ ( 3, _buildkdt->_kdtree->_nodes[1].first );
    EXPECT_EQ ( 4, _buildkdt->_kdtree->_nodes[1].second );

    EXPECT_EQ ( -1, _buildkdt->_kdtree->_nodes[3].first );
    EXPECT_EQ ( 1, _buildkdt->_kdtree->_nodes[3].second );

    EXPECT_EQ ( -2, _buildkdt->_kdtree->_nodes[4].first );
    EXPECT_EQ ( 1, _buildkdt->_kdtree->_nodes[4].second );

    EXPECT_EQ ( 5, _buildkdt->_kdtree->_nodes[2].first );
    EXPECT_EQ ( 6, _buildkdt->_kdtree->_nodes[2].second );

    EXPECT_EQ ( -3, _buildkdt->_kdtree->_nodes[5].first );
    EXPECT_EQ ( 1, _buildkdt->_kdtree->_nodes[5].second );

    EXPECT_EQ ( -4, _buildkdt->_kdtree->_nodes[6].first );
    EXPECT_EQ ( 1, _buildkdt->_kdtree->_nodes[6].second );
}


// ================================================================================

TEST_F ( BuildKdTreeTestCase2, build ) {
    _buildkdt->build ();
    KdTree<int>& kdt = *_buildkdt->_kdtree;
//     _buildkdt->_kdtree->save ( "BuildKdTreeTestCase2.idx" );
//     kdt.load ( "BuildKdTreeTestCase2.idx" );
    // should check _buildkdt->_primitives & _nodes
    EXPECT_EQ ( 4u, kdt._primitives.size() );
    EXPECT_EQ ( 9,  kdt._primitives[0] );
    EXPECT_EQ ( 8,  kdt._primitives[1] );
    EXPECT_EQ ( 7,  kdt._primitives[2] );
    EXPECT_EQ ( 6,  kdt._primitives[3] );
    EXPECT_EQ ( 7u, kdt._nodes.size() );
    EXPECT_EQ ( 1,  kdt._nodes[0].first );
    EXPECT_EQ ( 2,  kdt._nodes[0].second );
    EXPECT_EQ ( 3,  kdt._nodes[1].first );
    EXPECT_EQ ( 4,  kdt._nodes[1].second );
    EXPECT_EQ ( -1, kdt._nodes[3].first );
    EXPECT_EQ ( 1,  kdt._nodes[3].second );
    EXPECT_EQ ( -2, kdt._nodes[4].first );
    EXPECT_EQ ( 1,  kdt._nodes[4].second );
    EXPECT_EQ ( 5,  kdt._nodes[2].first );
    EXPECT_EQ ( 6,  kdt._nodes[2].second );
    EXPECT_EQ ( -3, kdt._nodes[5].first );
    EXPECT_EQ ( 1,  kdt._nodes[5].second );
    EXPECT_EQ ( -4, kdt._nodes[6].first );
    EXPECT_EQ ( 1,  kdt._nodes[6].second );
}

// ********************************************************************************

