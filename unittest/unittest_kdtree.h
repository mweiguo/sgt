#ifndef _UNITTEST_KDTREE_H_
#define _UNITTEST_KDTREE_H_

#include <list>
#include <vector>
#include <algorithm>
#include <stdexcept>

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


//    A.  (center pos)  left1 < left2 < ... < leftN,   right1 < right2 < ... < rightN ,  leftN < right1 ( not leaf )
//    B.  node's bbox = left1.bbox | left2.bbox | ... | leftN.bbox | right1.bbox | right2.bbox | ... | rightN.bbox     ( not leaf )
//    C.  leafnode's bounding box  = element1InLeaf.bbox | element2InLeaf.bbox | element3InLeaf.bbox | ... | elementNInLeaf.bbox ( leaf )
template < class T >
void kdtreeTest::IsKdTreeValid ( KdTree<T>& kdtree )
{
    // test
    KdTree<Rectanglef*>::BuildKdTree<GetBBox<Rectanglef*> > build ( kdtree, GetBBox<T>() );

    // check center position
    checkNodeCenterPosValid ( kdtree, 0, 0, GetBBox<T>() );
    // check node's bounding box
    checkNodeBBoxValid ( kdtree, 0, GetBBox<T>() );
}

// all left child node's center pos < all right child's node's center pos
template < class T, class GetBBoxFunctor >
void kdtreeTest::checkNodeCenterPosValid ( const KdTree<T>& kdtree, int nodeIdx, int level, GetBBoxFunctor getBBox )
{
    int dimension = level % 3;

    const typename KdTree<T>::KdNode& node = kdtree._nodes[nodeIdx];
    if ( node.first < 0 )
    {
	checkNodeCenterPosValid ( kdtree, -node.first, level+1, getBBox );
	if ( node.second < 0 )
	    checkNodeCenterPosValid ( kdtree, -node.second, level+1, getBBox );
    }
    else
    {
	int BeginIdx   = node.first;    // begin
	int EndIdx     = node.second;   // length
	for ( int i=BeginIdx; i<EndIdx-1; i++ )
	{
	    BBox first = getBBox ( kdtree[i] );
	    BBox second = getBBox ( kdtree[i] );
	    if ( first.center()[dimension] > second.center()[dimension] )
		throw std::logic_error ( "checkNodeCenterPosValid faild: node nodeIex's left node's i index" );
	}
    }
    
}

// parent node's bbox should be union of all children's bbox
template < class T, class GetBBoxFunctor >
void kdtreeTest::checkNodeBBoxValid ( const KdTree<T>& kdtree, int nodeIdx, GetBBoxFunctor getBBox )
{
    const typename KdTree<T>::KdNode& node = kdtree._nodes[nodeIdx];
    if ( node.first < 0 )
    {
	checkNodeBBoxValid ( kdtree, -node.first, getBBox );
	if ( node.second < 0 )
	    checkNodeBBoxValid ( kdtree, -node.second, getBBox );
    }
    else
    {
	int BeginIdx   = node.first;    // begin
	int EndIdx     = node.second;   // length
	BBox mergedbox;
	for ( int i=BeginIdx; i<EndIdx-1; i++ )
	{
	    BBox first = getBBox ( kdtree[i] );
	    BBox second = getBBox ( kdtree[i] );
	    mergedbox = mergedbox.unionbox ( first );
	    mergedbox = mergedbox.unionbox ( second );
	}

	BBox nodeBBox = getBBox ( kdtree[nodeIdx] );
	
	if ( nodeBBox != mergedbox )
	    throw std::logic_error ( "checkNodeBBoxValid faild" );

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

#include <iostream>
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

