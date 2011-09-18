/**
 * how to use kdtree
 * caller should provide two function for each primitive.
 *        vec2f getPrimitiveCenter ( T t )
 *        void getPrimitiveMinMax ( T, float* minmax );
 */

#ifndef _KDTREE_H_
#define _KDTREE_H_

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <fstream>
using namespace std;

#include "vec2.h"
#include "sgr_math.h"
#include "sgr_lc.h"
#include "sgr_nodetypes.h"

class Incr
{
public:
    Incr () : seed(0) {}
    int operator ()() { return seed++; }
private:
    int seed;
};

template <class Vec>
class IndexComp
{
public:
    IndexComp (vector<Vec>& vecs, int axis) : _vecs(vecs), _axis(axis) {}
    bool operator() ( int idx1, int idx2 ) {
        return (_vecs[idx1][_axis] < _vecs[idx2][_axis]);
    }
private:
    vector<Vec>& _vecs;
    int _axis;
};

// this class include some parameters needed in building kdtree
template< class GetPriCenterFunc, class GetPriMinMaxFunc >
class BuildKdTreeOption
{
public:
    BuildKdTreeOption ( int numleaf=16, int maxlvl=32 ) : targetnumperleaf(numleaf), maxlevel(maxlvl) {}
    BuildKdTreeOption ( const BuildKdTreeOption& rhs ) { 
	targetnumperleaf = rhs.targetnumperleaf; 
	maxlevel = rhs.maxlevel; 
	getPrimitiveCenter = rhs.getPrimitiveCenter;
	getPrimitiveMinMax = rhs.getPrimitiveMinMax;
    }
    /** 
     * this para define how many object can contained for each leaf node, 
     * but if the leaf node in the maxlevel, but there have more objects than this limit number, 
     * the leaf node will contain all the objects, even object's number > targetnumperleaf.
     */
    int targetnumperleaf;
    // this para define kdtree's max level
    int maxlevel;
    GetPriCenterFunc getPrimitiveCenter;
    GetPriMinMaxFunc getPrimitiveMinMax;
};

template<class T>
class KdTree
{
public:
    struct KdNode
    {
        KdNode ( int f=0, int s=0 ) : first(f), second(s) {}
	/**
	 * leaf node : first = -(index + 1),
	 *             second = size
	 *             index is of primitive
	 * non-leaf node : first = index
	 *                 second = index
	 *                 index is of kdnode
	 */
        int first, second;
	float minmax[4];
    };

//    template<class GetBBoxFunctor>
    template< class GetPriCenterFunc, class GetPriMinMaxFunc >
    class BuildKdTree
    {
    public:
        BuildKdTree ( KdTree<T>& kdtree, const BuildKdTreeOption<GetPriCenterFunc,GetPriMinMaxFunc>& opt );

	bool build ();

	// the following functions should not be called by user
        void computeDivision ( float* minmax );

        void divide ( int nodeidx=0, int level=0 );

        void getMinMax ( int istart, int iend, float* minmax );

        KdTree<T>* _kdtree;
        vector<int> _axisstack;
        vector<int> _objindices;
        vector<vec2f> _objcenters;
	const BuildKdTreeOption<GetPriCenterFunc,GetPriMinMaxFunc>& _opt;
    };
public:
    KdTree ();

    void getMinMax( float* minmax );

    template<class Output>
    bool intersect ( LC* lc, const float* minmax, Output out, int nodeidx=0 );

    void clear ();

    void save ( const char* filename );

    void load ( const char* filename );

    int addNode ( int b, int e );

    vector<T> _primitives;

    vector<KdNode> _nodes;
// protected:
    int intersect_cnt;
    
    
//    friend class template <class T1, class T2> BuildKdTree<T1, T2>;
};

//================================================================================

/** constructor
 */
template<class T>
inline KdTree<T>::KdTree ()
{
}

//================================================================================

/** get bounding box of all the objects in kdtree. 
 *  if no objects in there, NULL returned.
 */
template<class T>
inline void KdTree<T>::getMinMax( float* minmax )
{
    if ( _nodes.empty() )
        return;
    memcpy ( minmax, _nodes.front().minmax, sizeof(float)*4 );
}

//================================================================================

/** this function will find all objects which reside in or intersect with 'para box'.
 */
template<class T>
template<class Output >
bool KdTree<T>::intersect ( LC* lc, const float* minmax, Output out, int nodeidx )
{
    intersect_cnt ++;
    KdNode& node = _nodes[nodeidx];

    if ( true == is_outside ( node.minmax, minmax ) )
	return false;
	 
    if ( node.first >= 0 ) // if node is not leaf
    {
	if ( true == is_inside ( node.minmax, minmax ) )
	{
	    // find most left leaf node
	    KdNode* leftnode = &node;
	    KdNode* rightnode = &node;
	    do {
		leftnode = &(_nodes[leftnode->first]); 
	    } while ( leftnode->first >= 0 );
	    
	    // find most right leaf node
	    do {
		rightnode = &(_nodes[rightnode->second]); 
	    } while ( rightnode->first >= 0 );
	    
	    int istart = -rightnode->first-1;
	    int iend = istart + rightnode->second;
	    istart = -leftnode->first-1;
	    for ( int i=istart; i!=iend; i++ ) 
		*out++ = _primitives[i];
	}
	else
	{
	    bool isHit = false;
	    if ( true == intersect ( lc, minmax, out, node.first) )
		isHit = true;
	    if ( true == intersect ( lc, minmax, out, node.second ) )
		isHit = true;
	    return isHit;
	}
    }
    else   // if node is leaf 
    {
	if ( node.second > 0 ) // there have primitives in this leaf
	{
	    int istart = -node.first-1;
	    int iend = istart + node.second;
	    for ( int i=istart; i!=iend; i++ ) 
	    {
		GlobalLCRecord& grd = lc->globalLCEntry->LCRecords[ _primitives[i] ];
		switch ( grd.type )
		{
		case SLC_LINE:
		{
		    LineRecord& lrd = lc->lineEntry->LCRecords[grd.value];
		    if ( false == line_outside ( (float*)lrd.data, minmax ) )
			*out++ = _primitives[i];
		    break;
		}
		case SLC_PLINE:
		{
		    PLineRecord& pline = lc->plineEntry->LCRecords[grd.value];
		    for ( int ii=pline.start; ii<pline.end; ii++ )
		    {
			if ( false == line_outside ( (float*)(lc->plineBufferEntry->LCRecords + ii), minmax ) ) {
			    *out++ = _primitives[i];
			    break;
			}
		    }
		    break;
		}
		case SLC_TRIANGLE:
		{
		    TriangleRecord& trd = lc->triangleEntry->LCRecords[grd.value];
		    if ( false == tri_outside ( (float*)trd.data, minmax ) )
			*out++ = _primitives[i];
		    break;
		}
		case SLC_RECT:
		{
		    RectRecord& rrd = lc->rectEntry->LCRecords[grd.value];
		    if ( false == rect_outside ( (float*)rrd.data, minmax ) )
			*out++ = _primitives[i];
		    break;
		}
		case SLC_TEXT:
		{
		    TextRecord& text = lc->textEntry->LCRecords[grd.value];
		    if ( false == poly_outside ( (float*)(lc->textSilhouetteBufferEntry->LCRecords + text.silhouetteStart),
						 text.silhouetteEnd - text.silhouetteStart , minmax ) )
			*out++ = _primitives[i];
		    break;
		}
		}
	    }
	}
    }

    return true;
}

//================================================================================

/**
 * clear kdtree's inner state
 */
template<class T>
void KdTree<T>::clear ()
{
    _primitives.clear();
    _nodes.clear();
}

//================================================================================

/**
 * dump kdtree
 */
template<class T>
void KdTree<T>::save ( const char* filename )
{
    ofstream out;
    out.open ( filename , ofstream::binary );
    int size = _primitives.size();
/*     cout << "_primitives.size = " << size << endl; */
    out.write ( (const char*)&size, sizeof(int) );
    out.write ( (const char*)&(_primitives[0]), sizeof(T) * size );
//     for ( typename vector<T>::iterator pp=_primitives.begin();
// 	  pp!=_primitives.end();
// 	  ++pp )
// 	out.write ( (const char*)&(*pp), sizeof(T) );
    
    size = _nodes.size();
    out.write ( (const char*)&size, sizeof(int) );
    out.write ( (const char*)&(_nodes[0]), size * sizeof(KdNode) );
/*     cout << "_nodes.size = " << size << endl; */
//     for ( typename vector<KdNode>::iterator pp=_nodes.begin(); pp!=_nodes.end(); ++pp )
//     {
// 	out.write ( (const char*)&(pp->first), sizeof(pp->first) );
// 	out.write ( (const char*)&(pp->second), sizeof(pp->second) );
// 	out.write ( (const char*)&(pp->minmax[0]), sizeof(pp->minmax[0]) );
// 	out.write ( (const char*)&(pp->minmax[1]), sizeof(pp->minmax[1]) );
// 	out.write ( (const char*)&(pp->minmax[2]), sizeof(pp->minmax[2]) );
// 	out.write ( (const char*)&(pp->minmax[3]), sizeof(pp->minmax[3]) );
//     }

    out.close ();
}

//================================================================================

/**
 * load kdtree
 */
template<class T>
void KdTree<T>::load ( const char* filename )
{
    ifstream in;
    in.open ( filename , ifstream::binary );
    int size;
    in.read ( (char*)&size, sizeof(int) );
/*     cout << "load primitive.size = " << size << endl; */
    _primitives.reserve ( size );
    in.read ( (char*)&(_primitives[0]), sizeof(T) * size );

//     _primitives.reserve ( size );
//     T tmp;
//     for ( int i=0; i<size; i++ ) {
// 	in.read ( (char*)&tmp, sizeof(T) );
// 	_primitives.push_back ( tmp );
//     }
    in.read ( (char*)&size, sizeof(int) );
//    cout << "load nodes.size = " << size << endl; 
    _nodes.reserve ( size );
    _nodes.resize ( size );
    in.read ( (char*)&(_nodes[0]), sizeof(KdNode) * size );
//     for ( int i=0; i<size; i++ ) {
// 	KdNode node;
// 	in.read ( (char*)&node.first, sizeof(node.first) );
// 	in.read ( (char*)&node.second, sizeof(node.second) );
// 	in.read ( (char*)&node.minmax[0], sizeof(node.minmax[0]) );
// 	in.read ( (char*)&node.minmax[1], sizeof(node.minmax[1]) );
// 	in.read ( (char*)&node.minmax[2], sizeof(node.minmax[2]) );
// 	in.read ( (char*)&node.minmax[3], sizeof(node.minmax[3]) );
// 	_nodes.push_back ( node );
//     }
    in.close ();
}

//================================================================================

/**
 * constructor of BuildKdTree
 */
template<class T>
int KdTree<T>::addNode ( int b, int e )
{
    _nodes.push_back ( KdNode(b, e) );
    return _nodes.size()-1;
}

//================================================================================

/**
 * constructor of BuildKdTree
 */
template<class T>
template< class T1, class T2 >
KdTree<T>::BuildKdTree<T1,T2>::BuildKdTree ( KdTree<T>& kdtree, const BuildKdTreeOption<T1,T2>& opt ) : 
    _opt(opt)
{
    _kdtree = &kdtree;
}

//================================================================================

/**
 * build kdtree
 * if build succeeded, return true, else return false
 */
template<class T>
template< class T1, class T2 >
bool KdTree<T>::BuildKdTree<T1,T2>::build ()
{
    // generate object indices
    generate_n ( back_inserter(_objindices), _kdtree->_primitives.size(), Incr() );
    // pre alloc kdnode's room
    _kdtree->_nodes.reserve (_kdtree->_primitives.size()*5);
    // get minmax of whole scene
    float minmax[4];
    getMinMax ( 0, _kdtree->_primitives.size(), minmax );

    // generate 
    computeDivision ( minmax );
    transform ( _kdtree->_primitives.begin(), 
		_kdtree->_primitives.end(), 
		back_inserter(_objcenters), 
		_opt.getPrimitiveCenter );

    _kdtree->addNode ( -1, _kdtree->_primitives.size() );
    divide ();

    // set kdtree's objs in sorted order
    vector<T> objs;
    objs.reserve( _kdtree->_primitives.size());
    for ( size_t i=0; i<_objindices.size(); i++ )
	objs.push_back ( _kdtree->_primitives[_objindices[i]] );

    _kdtree->_primitives.swap ( objs );

    return true;
}

//================================================================================

/**
 * do space division, result will stored in _axisstack
 */
template<class T>
template< class T1, class T2 >
void KdTree<T>::BuildKdTree<T1,T2>::computeDivision ( float* minmax )
{
    float dimension[2];
    dimension[0] = minmax[2] - minmax[0];
    dimension[1] = minmax[3] - minmax[1];

    int level = 0;
    while ( level < _opt.maxlevel ) {
	int axis;
	// get axis
	// 0 == divide x
	// 1 == divide y
	if (dimension[0]>=dimension[1])
	    axis = 0;
	else
	    axis = 1;

	_axisstack.push_back ( axis );

	dimension[axis] /= 2.0f;
	++level;
    }
}

//================================================================================

/**
 * build the tree
 */
template<class T>
template< class T1, class T2 >
void KdTree<T>::BuildKdTree<T1,T2>::divide ( int nodeidx, int level )
{
    typename KdTree<T>::KdNode& node = _kdtree->_nodes[nodeidx];
    // istart & iend is index of primitive
    int istart = -(node.first+1);
    int iend   = istart + node.second;

    // set node.bb
    getMinMax ( istart, iend, node.minmax );

    if ( node.second <= _opt.targetnumperleaf || level >= _opt.maxlevel )
	return;

    int axis = _axisstack[level];

    // sort the indices from node.first to node.first + node.second
    if ( level==0 || axis != _axisstack[level-1] ) {
	IndexComp<vec2f> comp(_objcenters, axis);
	std::sort ( _objindices.begin()+istart, _objindices.begin()+iend, comp );
    }

    // calculate left and right node's indices range
    int imid = istart + node.second / 2;

    // refill node's first & second member variable, now it indicate kdnode's index
    node.first = _kdtree->addNode ( -(istart+1), imid-istart );
    node.second = _kdtree->addNode( -(imid+1), iend-imid );

    divide ( node.first, level+1 );
    divide ( node.second, level+1 );
}

//================================================================================

/**
 * get minmax, 
 * caller should ensure iend - istart >= 1
 */
template<class T>
template< class T1, class T2 >
void KdTree<T>::BuildKdTree<T1,T2>::getMinMax ( int istart, int iend, float* minmax )
{
    _opt.getPrimitiveMinMax ( _kdtree->_primitives[_objindices[istart]], minmax );
/*     cout << 0 << ' ' << minmax[0] << ' ' << minmax[1] << ' ' << minmax[2] << ' ' << minmax[3] << endl; */

    float tmp[4];
    for ( int i=istart+1; i<iend; i++ )
    {
	_opt.getPrimitiveMinMax ( _kdtree->_primitives[_objindices[i]], tmp );
	minmax[0] = minmax[0] < tmp[0] ? minmax[0] : tmp[0];
	minmax[1] = minmax[1] < tmp[1] ? minmax[1] : tmp[1];
	minmax[2] = minmax[2] > tmp[2] ? minmax[2] : tmp[2];
	minmax[3] = minmax[3] > tmp[3] ? minmax[3] : tmp[3];
/* 	cout << i << ' ' << minmax[0] << ' ' << minmax[1] << ' ' << minmax[2] << ' ' << minmax[3] << endl; */
    }
}

#endif // _KDTREE_H_
