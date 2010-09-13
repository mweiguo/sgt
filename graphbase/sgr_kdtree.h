#ifndef _KDTREE_H_
#define _KDTREE_H_

#include "sgr_bbox.h"
#include <sgr_grhagl.h>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

#ifdef _USESTATISTIC_
#include <time.h>
#include <sstream>
#endif //_USESTATISTIC_
using namespace std;
namespace SGR
{

// macro _USESTATISTIC_ will switch static function for kdtree
template<class ObjType, class GetBBoxFunctor>
struct getBBCenter
{
    getBBCenter ( GetBBoxFunctor getBBox ) : _getBBox(getBBox) {}
    vec3f operator() (ObjType node )
    {
        return _getBBox ( node ).center();
        //return node->getBBox().center();
    }
private:
    GetBBoxFunctor _getBBox;
};

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
class BuildKdTreeOption
{
public:
    // this para define how many object can contained for each leaf node, 
    // but if the leaf node in the maxlevel, but there have more objects than this limit number, 
    // the leaf node will contain all the objects, even object's number > targetnumperleaf.
    int targetnumperleaf;
    // this para define kdtree's max level
    int maxlevel;
    BuildKdTreeOption() : targetnumperleaf(16), maxlevel(16) {}
};

template<class ObjectType, class GetBBoxFunctor>
class BuildKdTree;

template<class ObjectType>
class KdTree : public vector<ObjectType>//, public GroupNode
{
public:
    struct KdNode
    {
        KdNode ( int f=0, int s=0 ) : first(f), second(s) {}
        int first, second;
        BBox bb;
    };
public:
    typedef vector<ObjectType> ObjectContainer;
    KdTree ();
    // get bounding box of all the objects in kdtree.
    BBox getBBox();
    // this function will find all objects which reside in or intersect with 'para box'.
    template<class Output, class GetBBoxFunctor >
    bool intersect ( const BBox& box, Output out, GetBBoxFunctor getBBox, int nodeidx=0 );

    void reset () {
        vector<ObjectType>::clear();
        _nodes.clear();
    }

#ifdef _USESTATISTIC_
    string memstatistic ();
    string intersectstatistic ();
private:
    int _buildStartClock, _buildFinishedClock;
    int _bbcompcnt, _selectedCount;
#endif // _USESTATISTIC_
private:
    //void addPrimitive (ObjectType obj) { _objs.push_back (obj); }
    int addNode ( int f, int s ) { _nodes.push_back ( KdNode(f, s) ); return _nodes.size()-1; }
    KdNode& getNode ( int idx ) { return _nodes[idx]; }
private:
public:
    string dump ( int nodeIdx=0, int level=0 );
    vector<KdNode> _nodes;
public:
    template<class GetBBoxFunctor>
    class BuildKdTree
    {
    public:
        BuildKdTree ( KdTree<ObjectType>& kdtree, GetBBoxFunctor getBBox, const BuildKdTreeOption& opt=BuildKdTreeOption() )
        {
            _kdtree = &kdtree;

#ifdef _USESTATISTIC_
            _buildStartClock = clock();
#endif // _USESTATISTIC_

            BBox bb;
            for ( size_t i=0; i<kdtree.size(); i++ )
                bb = bb.unionbox ( getBBox(kdtree[i]) );

            computeDivision (bb, opt);

            generate_n ( back_inserter(_objindices), _kdtree->size(), Incr() );
            _kdtree->_nodes.reserve (_kdtree->size()*5);

            transform ( _kdtree->begin(), _kdtree->end(), back_inserter(_objcenters), getBBCenter<ObjectType,GetBBoxFunctor>(getBBox) );

            /* int root =  */_kdtree->addNode ( -1, _kdtree->size() );
            divide (opt, getBBox/*, bb*/);

            // set kdtree's objs in sorted order
            vector<ObjectType> objs;
            objs.reserve( _kdtree->size());
            for ( size_t i=0; i<_objindices.size(); i++ )
                objs.push_back ( (*_kdtree)[_objindices[i]] );

//cout << "aaa " << _kdtree->_nodes[0].second << endl;
            _kdtree->swap ( objs );

#ifdef _USESTATISTIC_
            _buildFinishedClock = clock();
#endif // _USESTATISTIC_
            //_kdtree->dump ( root, 0 );
        }
        template<class InputIterator>
        BuildKdTree ( KdTree<ObjectType>& kdtree, InputIterator begin, InputIterator end, GetBBoxFunctor getBBox, const BuildKdTreeOption& opt=BuildKdTreeOption() )
        {
            _kdtree = &kdtree;
#ifdef _USESTATISTIC_
            _buildStartClock = clock();
#endif // _USESTATISTIC_
            BBox bb;
            for ( int i=0; i<kdtree.size(); i++ )
                bb = bb.unionbox ( getBBox(kdtree[i]) );

            computeDivision (bb,opt);

            _kdtree->assign ( begin, end );
            _kdtree->_nodes.reserve (_kdtree->size()*5);
            generate_n ( back_inserter(_objindices), _kdtree->size(), Incr() );

            transform ( _kdtree->begin(), _kdtree->end(), back_inserter(_objcenters), getBBCenter<ObjectType,GetBBoxFunctor>(getBBox) );

            int root = _kdtree->addNode ( -1, _kdtree->size() );

            divide (opt, getBBox/*, bb*/);

            // set kdtree's objs in sorted order
            vector<ObjectType> objs;
            objs.reserve( _kdtree->size());
            for ( int i=0; i<_objindices.size(); i++ ) {
                objs.push_back ( (*_kdtree)[_objindices[i]] );
            }
            _kdtree->swap ( objs );

#ifdef _USESTATISTIC_
            _buildFinishedClock = clock();
#endif // _USESTATISTIC_
            //_kdtree->dump ( root, 0 );
        }

        ~BuildKdTree () {}
#ifdef _USESTATISTIC_
        string statistic ()
        {
            int axisstack = _axisstack.size() * sizeof(int);
            int objindices = _objindices.size() * sizeof(int);
            int objcenters = _objcenters.size() * sizeof(vec3f);
            int _kdtree = sizeof(KdTree<ObjectType>&);
            stringstream ss;
            double unit = 1000000.0;
            ss << ">> statistic of BuildKdTree" << endl;
            ss << "     axisstack = " << axisstack << " (bytes)" << endl;
            ss << "     objindices = " << objindices/unit << " (M)" << endl;
            ss << "     objcenters = " << objcenters/unit << " (M)" << endl;
            ss << "     _kdtree = " << _kdtree << " (bytes)" << endl;
            ss << "==========================================" << endl;
            ss << "   total memory used = " << (axisstack+objindices+objcenters+_kdtree)/unit << " (M)" << endl;
            ss << "   total time used = " << (_buildFinishedClock - _buildStartClock )/(1.0*CLOCKS_PER_SEC) << " (s)" << endl;
            return ss.str();
        }
    private:
        int _buildStartClock, _buildFinishedClock;
#endif // _USESTATISTIC_
    private:
        void computeDivision(const BBox& bb, const BuildKdTreeOption& opt )
        {
#ifdef _DEBUG_OUTPUT_
            //LOG_DEBUG ( "begin computeDivision ... " );
#endif // _DEBUG_OUTPUT_
            vec3f dimension;
            dimension = bb.maxvec() - bb.minvec();

            int level = 0;
            while ( level < opt.maxlevel ) {
                int axis;
                // get axis
                // 0 == divide x
                // 1 == divide y
                // 2 == divide z
                if (dimension[0]>=dimension[1]) {
                    if (dimension[0]>=dimension[2]) axis = 0;
                    else axis = 2;
                } else if (dimension[1]>=dimension[2]) axis = 1;
                else axis = 2;


                _axisstack.push_back ( axis );

                //LOG_DEBUG ( "level=%d\taxis=%d\tdimension=(%f, %f, %f)", level, axis, dimension[0], dimension[1], dimension[2] );

                dimension[axis] /= 2.0f;
                ++level;
            }

#ifdef _DEBUG_OUTPUT_
            //LOG_DEBUG ( "ok" );
#endif // _DEBUG_OUTPUT_
        }

        void divide ( const BuildKdTreeOption& opt, GetBBoxFunctor getBBox, int nodeidx=0, int level=0 )
        {

            //while (1) 
            //{
            // if ( target number in this node < target number per leaf ) will stop divide 
            typename KdTree<ObjectType>::KdNode& node = _kdtree->getNode(nodeidx);
            int istart = -node.first-1;
            int iend   = istart + node.second;

            // set node.bb
            node.bb = getUnionBBox ( istart, iend, getBBox );

            if ( node.second <= opt.targetnumperleaf || level >= opt.maxlevel )
                return;

            int axis = _axisstack[level];

            // sort the indices from node.first to node.first + node.second
            IndexComp<vec3f> comp(_objcenters, axis);
            std::sort ( _objindices.begin()+istart, _objindices.begin()+iend, comp );
            //sortindices ( istart, iend, axis );

            // calculate left and right node's indices range
            //float mid = bb.center()[axis];
            //int imid = getmidindex ( istart, iend, mid, axis );
            int imid = istart + (iend - istart) / 2;
            //LOG_DEBUG ("\tistart=%d, iend=%d, mid=%f, axis=%d, imid=%d, left=%d, right=%d", istart, iend, mid, axis, imid, -istart-1, imid-istart );
#ifdef _DEBUG_OUTPUT1_
            stringstream ss, ss0;
            //for ( int i=0; i<_objindices.size(); i++ )
            //  ss << _objindices[i] << " ";
            for ( int i=0; i<level; i++ )
                ss0 << "  ";
            //qDebug ("%slevel=%d, istart=%d, iend=%d, imid=%d, axis=%d, indices=[%s]", ss0.str().c_str(), level, istart, iend, imid, axis, ss.str().c_str() );
            qDebug ("%slevel=%d, istart=%d, iend=%d, imid=%d, elementnum=%d, axis=%d", ss0.str().c_str(), level, istart, iend, imid, iend-istart, axis );
#endif // _DEBUG_OUTPUT1_
            node.first = _kdtree->addNode ( -istart-1, imid-istart );
            node.second = _kdtree->addNode( -imid-1, iend-imid );

            //  // use this condition to ensure stack depth bound = log(N)
            //  if ( (imid-istart) >= (iend-imid) ) 
            //  {
            //          float temp = bb.min()[axis];
            //          bb.min()[axis] = mid;
            //          divide ( opt, bb, node.second, level+1 );
            //          bb.min()[axis] = temp;

            //          nodeidx = node.first;
            //          level++;
            //  }
            //  else
            //  {
            //          float temp = bb.max()[axis];
            //          bb.max()[axis] = mid;
            //          divide ( opt, bb, node.first, level+1 );
            //          bb.max()[axis] = temp;

            //          nodeidx = node.second;
            //          level++;
            //  }
            //}
            //float temp = bb.max()[axis];
            //bb.max()[axis] = mid;
            divide ( opt, getBBox, node.first, level+1 );
            //bb.max()[axis] = temp;

            // //LOG_DEBUG ("****istart=%d, iend=%d, mid=%f, axis=%d, imid=%d, left=%d, right=%d", istart, iend, mid, axis, imid, -imid-1, iend-imid );
            //temp = bb.min()[axis];
            //bb.min()[axis] = mid;
            divide ( opt, getBBox, node.second, level+1 );
            //bb.min()[axis] = temp;
        }
        BBox getUnionBBox ( int istart, int iend, GetBBoxFunctor getBBox )
        {
            if ( (iend-istart)<1 )
                return BBox();

            BBox box = getBBox ( (*_kdtree)[_objindices[istart]] );
            for ( int i=istart+1; i<iend; i++ )
                box = box.unionbox ( getBBox ( (*_kdtree)[_objindices[i]] ) );
            //BBox box = (*_kdtree)[_objindices[istart]]->getBBox();
            //for ( int i=istart+1; i<iend; i++ )
            //    box = box.unionbox ( (*_kdtree)[_objindices[i]]->getBBox() );
            return box;
        }

    private:
        KdTree<ObjectType>* _kdtree;
        vector<int> _axisstack;
        vector<int> _objindices;
        vector<vec3f> _objcenters;
    };
};

template<class ObjectType>
inline KdTree<ObjectType>::KdTree ()
{
#ifdef _USESTATISTIC_
    _bbcompcnt = 0;
#endif //  _USESTATISTIC_
}

template<class ObjectType>
inline BBox KdTree<ObjectType>::getBBox()
{
    if ( _nodes.empty() )
        return BBox();
    return _nodes.front().bb;
}

template<class ObjectType>
template<class Output, class GetBBoxFunctor >
inline bool KdTree<ObjectType>::intersect ( const BBox& box, Output out, GetBBoxFunctor getBoundingBox, int nodeidx )
{
#ifdef _USESTATISTIC_
    if ( nodeidx == 0 )
    {
        _selectedCount = _bbcompcnt = 0;
        _buildStartClock = clock();
    }
#endif 
    if ( nodeidx >= (int)_nodes.size() || nodeidx < 0 )
    {
#ifdef _USESTATISTIC_
        if ( nodeidx == 0 )
            _buildFinishedClock = clock();
#endif
        return false;
    }

    KdNode& node = _nodes[nodeidx];

#ifdef _USESTATISTIC_
    _bbcompcnt++;
#endif 
    if ( !is_intersect ( box, node.bb ) ) 
    {
#ifdef _USESTATISTIC_
        if ( nodeidx == 0 )
            _buildFinishedClock = clock();
#endif
        return false;
    }

    if ( node.first >= 0 ) // if node is not leaf
    {
        intersect ( box, out, getBoundingBox, node.first);
        intersect ( box, out, getBoundingBox, node.second );
    }
    else   // if node is leaf 
    {
        if ( node.second > 0 ) 
        {
            int istart = -node.first-1;
            int iend = istart + node.second;
            for ( int i=istart; i!=iend; i++ ) 
            {
                //BBox& bb = _objs[i]->getBBox();
#ifdef _USESTATISTIC_
                _bbcompcnt++;
#endif 
                if ( ! is_intersect ( box, getBoundingBox ( (*this)[i] ) ) )
                    continue;

                *out++ = (*this)[i];
#ifdef _USESTATISTIC_
                _selectedCount++;
#endif 
            }
        }
    }

#ifdef _USESTATISTIC_
    if ( nodeidx == 0 )
        _buildFinishedClock = clock();
#endif 
    return true;
}


//template<class ObjectType, class GetBBoxFunctor>
//inline BuildKdTree<ObjectType,GetBBoxFunctor>::~BuildKdTree () 
//{
//}

#ifdef _USESTATISTIC_
template<class ObjectType>
inline string KdTree<ObjectType>::memstatistic ()
{
    int nums = size();
    int objs = size() * sizeof(ObjectType);
    int heapalloc = size() * sizeof(Rectanglef);
    int nodes = _nodes.size() * sizeof(KdNode);
    double unit = 1000000.0;
    stringstream ss;
    ss << ">> statistic of KdTree" << endl;
    ss << "     nums = " << nums / unit << " (M)"  << endl;
    ss << "     objs = " << objs / unit << " (M)" << endl;
    ss << "     heapalloc = " << heapalloc / unit << " (M)"  << endl;
    ss << "     nodes = " << nodes / unit << " (M)"  << endl;
    ss << "==========================================" << endl;
    ss << "   total memory used = " << (objs+nodes+heapalloc)/unit << " (M)" << endl;
    return ss.str();
}

template<class ObjectType>
inline string KdTree<ObjectType>::intersectstatistic ()
{
    stringstream ss;
    ss << ">> statistic of KdTree" << endl;
    ss << "     bb compare times = " << _bbcompcnt  << endl;
    ss << "     time ellapse = " << (_buildFinishedClock - _buildStartClock)/(1.0*CLOCKS_PER_SEC) << " (s)" << endl;
    ss << "     number of selected items  = " << _selectedCount << endl;
    ss << "     select rate  = " << _selectedCount/(1.0*size()) << endl;
    return ss.str();
}
#endif // _USESTATISTIC_

template<class ObjectType>
inline string KdTree<ObjectType>::dump ( int nodeIdx, int level  ) 
{
    KdNode& node = _nodes[nodeIdx];
    stringstream ss, ss0;
    for ( int i=0; i<level; i++ )
        ss0 << "  ";
    ss << ss0.str() << "DUMP kdtree:  level=" << level << "\tnodeidx=" << nodeIdx << "\tleft=" << node.first << "\tright=" << 
        node.second << "\tbbox={"<<node.bb.minvec().x()<<","<<node.bb.minvec().y()<<", "<<node.bb.maxvec().x()<<","<<node.bb.maxvec().y()<<"}" << endl;
    //qDebug ( ss.str().c_str() );
    //LOG_INFO ( "DUMP kdtree:  level=%d\tleft=%d\tright=%d", level, node.first, node.second );
    if ( node.second == 0 )
        return ss.str();

    if ( node.first >=0 ) {
        ss << dump ( node.first, level+1 );
        ss << dump ( node.second, level+1 );
    }

    return ss.str();
    //if ( level == 0 )
    //{
    //    qDebug ( "size of object's container = %d", _objs.size() * 4 );
    //    qDebug ( "size of objects            = %d", _objs.size() * sizeof (*_objs[0]) );
    //    qDebug ( "size of nodes container    = %d", sizeof(_nodes) );
    //    qDebug ( "size of nodes              = %d", _nodes.size() * sizeof(_nodes[0]) );
    //}
}

}
#endif // _KDTREE_H_
