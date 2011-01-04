#ifndef _KDTREE_NODE_H_
#define _KDTREE_NODE_H_

#include <sgr_kdtree.h>
#include "sgr_nodemgr.h"
#include "sgr_groupnode.h"

#include "sgr_drawablenode.h"

#include "sgr_rendernodecollector.h"

namespace SGR
{

template<class T>
class GetBBox
{
public:
    const BBox& operator() ( T t ) { return t->getBBox(); }
};
//typedef KdTree<DrawableNode*> Kdtree;
typedef KdTree<DrawableNode*> SpatialObjectMgr;
typedef KdTree<DrawableNode*>::BuildKdTree<GetBBox<DrawableNode*> > BuildSpatialObjectMgr;


template < class T>
class SGR_DLL KdTreeNodeT : public GroupNode
{
public:
    KdTreeNodeT () : GroupNode() {}
    KdTreeNodeT ( const KdTreeNodeT& rhs ) : GroupNode (rhs)
    {
        _kdtree = rhs._kdtree;
    }
    virtual SGNode* clone ()
    {
        return new KdTreeNodeT(*this);
    }

    template<class Output >
    bool intersect ( const BBox& box, Output out )
    {
        return _kdtree.intersect ( box, out, GetBBox<DrawableNode*>() );
    }
    void buildKdTree ();
/*     { */
/*         _kdtree.reset (); */
/*         RenderNodeCollector< back_insert_iterator<KdTree<T> > > collector( back_inserter(_kdtree) ); */
/*         collector ( *this ); */
/* 	qDebug ("\t\tbefore buildKdTree size =%d", _kdtree.size() ); */
/*         BuildSpatialObjectMgr build ( _kdtree, GetBBox<DrawableNode*>() ); */
/* 	qDebug ("\t\tafter buildKdTree size =%d", _kdtree.size() ); */
/*     } */
    
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~KdTreeNodeT () {}
private:
    KdTree<T> _kdtree;
};

typedef KdTreeNodeT<DrawableNode*> KdTreeNode;
//typedef NodeMgr<KdTreeNode>        KdTreeNodeMgr;


template <class T>
void KdTreeNodeT<T>::buildKdTree ()
{
    _kdtree.reset ();
    RenderNodeCollector< back_insert_iterator<KdTree<T> > > collector( back_inserter(_kdtree) );
    collector ( *this );
    qDebug ("\t\tbefore buildKdTree size =%d", _kdtree.size() );
    BuildSpatialObjectMgr build ( _kdtree, GetBBox<DrawableNode*>() );
    qDebug ("\t\tafter buildKdTree size =%d", _kdtree.size() );
}

}
#endif // _KDTREE_NODE_H_
