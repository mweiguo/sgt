#ifndef _UNITTEST_KDTREE_H_
#define _UNITTEST_KDTREE_H_

#include <list>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <iostream>

#include <rectangle.h>
#include <bbox.h>
#include <kdtree.h>

using namespace std;

template <class T>
class GetBBox
{
public:
    BBox operator() ( T t ) { return t->boundingbox(); }
};


class kdtreeTest
{
    template < class T, class GetBBoxFunctor >
    void checkNodeCenterPosValid ( const KdTree<T>& kdtree, int nodeIdx, int level, GetBBoxFunctor getBBox );
    template < class T, class GetBBoxFunctor >
    void checkNodeBBoxValid ( const KdTree<T>& kdtree, int nodeIdx, GetBBoxFunctor getBBox );
private:
    // after building succeeded, flowing conditions should be valid
    // precondition:
    //    each node have flowing content
    // 	  ------------------------------------------------------------
    // 	  | left1, left2, ..., leftn,   right1, right2, ..., rightn  |
    // 	  ------------------------------------------------------------
    //    node.first < 0, node.first is element's index
    //    node.first > 0, -node.first-1 is node's index,  node.second + (-node.first-1) is length of the range
    // goals:
    // 	  A.  left1 < left2 < ... < leftN,   right1 < right2 < ... < rightN
    // 	  B.  leftN < right1
    // 	  C.  node's bbox = left1.bbox | left2.bbox | ... | leftN.bbox | right1.bbox | right2.bbox | ... | rightN.bbox
    //    D.  leafnode's bounding box  = element1InLeaf.bbox | element2InLeaf.bbox | element3InLeaf.bbox | ... | elementNInLeaf.bbox
    template < class T >
    void IsKdTreeValid ( KdTree<T>& kdtree );
    // every node in kdtree should be checked
    // record all nodes which bounding-box is intersected with give bbox
    // check if these two set are equal.
    template < class T, class GetBBoxFunctor >
    void IntersectValid ( KdTree<T>& kdtree, const BBox& box, GetBBoxFunctor getBBox );
public:
    kdtreeTest()
    {
	// prepare test data
	Rectanglef* rect[5];
	rect[0] = new Rectanglef ( 0, 0, 2, 2 );
	rect[1] = new Rectanglef ( 0, 0, 2, 2 );
	rect[2] = new Rectanglef ( 0, 0, 2, 2 );
	rect[3] = new Rectanglef ( 0, 0, 2, 2 );
	rect[4] = new Rectanglef ( 0, 0, 2, 2 );

	KdTree<Rectanglef*> kdtree;
	kdtree.assign ( rect, rect+5 );
	// should satisfy these conditions
	IsKdTreeValid ( kdtree );
	BBox box;
	box.expandby ( vec3f (0, 0, 0) );
	box.expandby ( vec3f (2, 2, 0) );
	IntersectValid ( kdtree, box, GetBBox<Rectanglef*>() );
    }
};


template < class T >
void kdtreeTest::IsKdTreeValid ( KdTree<T>& kdtree )
{
//    cout << kdtree.size() << endl;
    // test
    BuildKdTreeOption opt;
    opt.targetnumperleaf = 2;
    KdTree<Rectanglef*>::BuildKdTree<GetBBox<Rectanglef*> > build ( kdtree, GetBBox<T>(), opt );

    // check center position
    checkNodeCenterPosValid ( kdtree, 0, 0, GetBBox<T>() );

    // check node's bounding box
    checkNodeBBoxValid ( kdtree, 0, GetBBox<T>() );
}

// all left child node's center pos < all right child's node's center pos
template < class T, class GetBBoxFunctor >
void kdtreeTest::checkNodeCenterPosValid ( const KdTree<T>& kdtree, int nodeIdx, int level, GetBBoxFunctor getBBox )
{
    int dimension = level % 2;

    const typename KdTree<T>::KdNode& node = kdtree._nodes[nodeIdx];
    if ( node.first < 0 ) // leaf
    {
	int istart = -node.first-1;
	int iend   = istart + node.second;
	for ( int i=istart; i<iend-1; i++ )
	{
	    BBox first = getBBox ( kdtree[i] );
	    BBox second = getBBox ( kdtree[i] );
	    if ( first.center()[dimension] > second.center()[dimension] )
		throw std::logic_error ( "checkNodeCenterPosValid faild: node nodeIex's left node's i index" );
	}
    }
    else
    {
	checkNodeCenterPosValid ( kdtree, -node.first, level+1, getBBox );
	if ( node.second < 0 )
	    checkNodeCenterPosValid ( kdtree, -node.second, level+1, getBBox );
    }
    
}

// parent node's bbox should be union of all children's bbox
template < class T, class GetBBoxFunctor >
void kdtreeTest::checkNodeBBoxValid ( const KdTree<T>& kdtree, int nodeIdx, GetBBoxFunctor getBBox )
{
    const typename KdTree<T>::KdNode& node = kdtree._nodes[nodeIdx];
    int istart = -node.first-1;
    int iend   = istart + node.second;

    cout << istart << ',' << iend << endl;
    if ( node.first < 0 )
    {
	int istart = -node.first-1;
	int iend   = istart + node.second;
	BBox mergedbox;
	for ( int i=istart; i<iend-1; i++ )
	{
	    BBox first = getBBox ( kdtree[i] );
	    BBox second = getBBox ( kdtree[i+1] );
	    cout << "first: " << first.dump() << endl;
	    cout << "second: " << second.dump() << endl;
	    mergedbox = mergedbox.unionbox ( first );
	    mergedbox = mergedbox.unionbox ( second );
	    cout << "mergedbox: " << mergedbox.dump() << endl << endl;
	}

	BBox nodeBBox = getBBox ( kdtree[nodeIdx] );
	if ( nodeBBox != mergedbox )
	{
	    stringstream ss;
	    ss << "nodeBBox min(x,y,z): " << nodeBBox.min().x() << ", " << nodeBBox.min().y() << ", " << nodeBBox.min().z() << endl;
	    ss << "nodeBBox max(x,y,z): " << nodeBBox.max().x() << ", " << nodeBBox.max().y() << ", " << nodeBBox.max().z() << endl;
	    ss << "mergedbox min(x,y,z): " << mergedbox.min().x() << ", " << mergedbox.min().y() << ", " << mergedbox.min().z() << endl;
	    ss << "mergedbox max(x,y,z): " << mergedbox.max().x() << ", " << mergedbox.max().y() << ", " << mergedbox.max().z() << endl;
	    throw std::logic_error ( "checkNodeBBoxValid faild: \n" + ss.str() );
	}
    }
    else
    {
	checkNodeBBoxValid ( kdtree, -node.first, getBBox );
	if ( node.second < 0 )
	    checkNodeBBoxValid ( kdtree, -node.second, getBBox );
    }
}

template < class T, class GetBBoxFunctor >
void kdtreeTest::IntersectValid ( KdTree<T>& kdtree, const BBox& box, GetBBoxFunctor getBBox )
{
// do intersect action
    vector<T> out;
    kdtree.intersect ( box, back_inserter(out), getBBox );

// do intersect on normal way
    vector<T> out1;
    for ( typename vector<T>::iterator pp=kdtree.begin(); pp!=kdtree.end(); ++pp )
    {
	BBox t = getBBox ( *pp );
	if ( t.isIntersect ( box ) )
	    out1.push_back ( *pp );
    }

// compare two container
    if ( out.size() != out1.size() )
	return;

    for ( typename vector<T>::iterator pp=out.begin(); pp!=out.end(); ++pp )
    {
	bool isFind = false;
	for ( typename vector<T>::iterator pp1=out1.begin(); pp1!=out1.end(); ++pp1 )
	{
	    if ( *pp == *pp1 )
		isFind = true;
	}

	if ( !isFind )
	    throw std::logic_error ( "KdTree's intersection failed" );
    }
}

#endif


#ifndef _MAIN_EXISTS_

#include <exception>

int main ()
{
    try
    {
	kdtreeTest test;
    }
    catch ( std::exception& ex )
    {
	cout << ex.what() << endl;
    }	
}

#endif// _MAIN_EXISTS_

