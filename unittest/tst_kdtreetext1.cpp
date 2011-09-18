#include "gtest/gtest.h"

#include <sgr_lc.h>
#include <sgr_node2lc.h>
#include <sgr_nodes.h>
#include <sgr_bboxupdater.h>
#include <sgr_lc2kdtree.h>
#include <kdtree.h>

#include <iostream>


// ********************************************************************************

class KdTreeText1 : public ::testing::Test
{
protected:
    KdTreeText1 ()
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
	mat->fontfilename = "simsun.ttc";
	SLCLayerNode* layer = new SLCLayerNode ( "layer1", mat_layer );
	SLCLODNode* lod = new SLCLODNode();
	SLCLODPageNode* lodpage = new SLCLODPageNode();
	lodpage->delayloading = false;
	lodpage->imposter = true;
	SLCTextNode* txt = new SLCTextNode ( mat );
	txt->text = "中";
	txt->pos.xy ( 0, 0 );
	SLCLODPageNode* lodpage2 = new SLCLODPageNode();
	lodpage2->delayloading = true;
	lodpage2->imposter = false;
	scene.addChild ( mat_layer );
	scene.addChild ( mat );
	scene.addChild ( layer );
	layer->addChild ( lod );
	lod->addChild ( lodpage );
	lod->addChild ( lodpage2 );
	lodpage->addChild ( txt );
	nodes.push_back ( mat_layer );
	nodes.push_back ( mat );
	nodes.push_back ( layer );
	nodes.push_back ( lod );
	nodes.push_back ( lodpage );
	nodes.push_back ( txt );
	nodes.push_back ( lodpage2 );
	node2lc = new SLCNode2LC ( &scene );
	_lc = node2lc->generateLC ();
	for ( list<SLCNode*>::iterator pp=nodes.begin(); pp!=nodes.end(); ++pp )
	    delete *pp;

	// update bbox
	BBox2dUpdater::forward_update ( *_lc );
     
	// lc2kdtree
	KdTree<int> tmpkdt;
	LC2KDT::collectPrimitive ( _lc->getType(), _lc->getGIndex(), tmpkdt );
	LC2KDT::traverse ( *_lc, tmpkdt );
	// buildKdTree
	GetPrimitiveCenter getPrimitiveCenter;
	GetPrimitiveMinMax getPrimitiveMinMax;
	getPrimitiveCenter.init ( _lc );
	getPrimitiveMinMax.init ( _lc );
	option.targetnumperleaf = 1;
	option.maxlevel = 32;
	option.getPrimitiveCenter = getPrimitiveCenter;
	option.getPrimitiveMinMax = getPrimitiveMinMax;
	_buildkdt = new BuildLCKdTree ( tmpkdt, option );
	_buildkdt->build ();

	tmpkdt.save ( "test.idx" );
	_kdt = new KdTree<int>();
	_kdt->load ( "test.idx" );
    }
    ~KdTreeText1 ()
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

TEST_F ( KdTreeText1, getMinMax ) {
    float minmax[4];
    _kdt->getMinMax ( minmax );
    EXPECT_EQ ( 0, minmax[0] );
    EXPECT_EQ ( 0, minmax[1] );
    EXPECT_EQ ( 1, minmax[2] );
    EXPECT_EQ ( 1, minmax[3] );
}

// ================================================================================

TEST_F ( KdTreeText1, IntersectRow1Col1 ) {
    { 
	vector<int> out;
	float box[] = { -2, -2, -1, -1 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
    { 
	vector<int> out;
	float box[] = { -1, -2, 0, -1 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
    { 
	vector<int> out;
	float box[] = { -1, -2, 0.5, -1 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
    { 
	vector<int> out;
	float box[] = { 0, -2, 1, -1 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
    { 
	vector<int> out;
	float box[] = { 0.5, -2, 1.5, -1 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
    { 
	vector<int> out;
	float box[] = { 1, -2, 2, -1 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
    { 
	vector<int> out;
	float box[] = { 1.5 , -2, 2, -1 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
}

// ================================================================================

TEST_F ( KdTreeText1, IntersectRow1Col2 ) {
    { 
	vector<int> out;
	float box[] = { -2, -1, -1, 0 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
    { 
	vector<int> out;
	float box[] = { -1, -1, 0, 0 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { -1, -1, 0.5, 0 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 0, -1, 1, 0 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 0.5, -1, 1.5, 0 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 1, -1, 2, 0 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 1.5 , -1, 2, 0 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
}

// ================================================================================

TEST_F ( KdTreeText1, IntersectRow1Col3 ) {
    { 
	vector<int> out;
	float box[] = { -2, -0.5, -1, 0.5 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
    { 
	vector<int> out;
	float box[] = { -1, -0.5, 0, 0.5 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { -1, -0.5, 0.5, 0.5 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 0, -0.5, 1, 0.5 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 0.5, -0.5, 1.5, 0.5 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 1, -0.5, 2, 0.5 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 1.5, -0.5, 2, 0.5 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
}

// ================================================================================

TEST_F ( KdTreeText1, IntersectRow1Col4 ) {
    { 
	vector<int> out;
	float box[] = { -2, 0, -1, 1 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
    { 
	vector<int> out;
	float box[] = { -1, 0, 0, 1 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { -1, 0, 0.5, 1 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 0, 0, 1, 1 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 0.5, 0, 1.5, 1 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 1, 0, 2, 1 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 1.5, 0, 2, 1 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
}

// ================================================================================

TEST_F ( KdTreeText1, IntersectRow1Col5 ) {
    { 
	vector<int> out;
	float box[] = { -2, 0.5, -1, 1.5 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
    { 
	vector<int> out;
	float box[] = { -1, 0.5, 0, 1.5 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { -1, 0.5, 0.5, 1.5 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 0, 0.5, 1, 1.5 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 0.5, 0.5, 1.5, 1.5 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 1, 0.5, 2, 1.5 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 1.5, 0.5, 2, 1.5 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
}

// ================================================================================

TEST_F ( KdTreeText1, IntersectRow1Col6 ) {
    { 
	vector<int> out;
	float box[] = { -2, 1, -1, 2 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
    { 
	vector<int> out;
	float box[] = { -1, 1, 0, 2 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { -1, 1, 0.5, 2 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 0, 1, 1, 2 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 0.5, 1, 1.5, 2 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 1, 1, 2, 2 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( true, isHit );
	EXPECT_EQ ( 1u, out.size() );
	EXPECT_EQ ( 6, out[0] );
    }
    { 
	vector<int> out;
	float box[] = { 1.5, 1, 2, 2 };
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
}

// ================================================================================

TEST_F ( KdTreeText1, IntersectRow1Col7 ) {
    { 
	vector<int> out;
	float box[] = { -2, 2, -1, 3 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
    { 
	vector<int> out;
	float box[] = { -1, 2, 0, 3 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
    { 
	vector<int> out;
	float box[] = { -1, 2, 0.5, 3 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
    { 
	vector<int> out;
	float box[] = { 0, 2, 1, 3 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
    { 
	vector<int> out;
	float box[] = { 0.5, 2, 1.5, 3 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
    { 
	vector<int> out;
	float box[] = { 1, 2, 2, 3 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
    { 
	vector<int> out;
	float box[] = { 1.5 , 2, 2, 3 }; 
	bool isHit = _kdt->intersect( _lc, box, back_inserter(out) );
	EXPECT_EQ ( false, isHit );
    }
}

// ================================================================================

TEST_F ( KdTreeText1, clear ) {
    _kdt->clear();
    EXPECT_EQ ( 0u, _kdt->_primitives.size() );
    EXPECT_EQ ( 0u, _kdt->_nodes.size() );
}


// ********************************************************************************

