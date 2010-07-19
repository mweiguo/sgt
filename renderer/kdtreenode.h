#ifndef _KDTREE_NODE_H_
#define _KDTREE_NODE_H_

#include "kdtree.h"
#include "nodemgr.h"
#include "groupnode.h"

#include "drawablenode.h"
#include "rendernodecollector.h"
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
class AGEF_EXPORT KdTreeNodeT : public GroupNode
{
public:
    KdTreeNodeT () {}
    KdTreeNodeT ( const KdTreeNodeT& rhs ) : GroupNode (rhs)
    {
        _kdtree = rhs._kdtree;
    }
    template<class Output >
    bool intersect ( const BBox& box, Output out )
    {
        return _kdtree.intersect ( box, out, GetBBox<DrawableNode*>() );
    }
    void buildKdTree ()
    {
        _kdtree.reset ();
        RenderNodeCollector< back_insert_iterator<KdTree<T> > > collector( back_inserter(_kdtree) );
        collector ( *this );
        BuildSpatialObjectMgr build ( _kdtree, GetBBox<DrawableNode*>() );
    }
    virtual void accept ( NodeVisitor& pvisitor ) const { pvisitor.apply ( *this ); }
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~KdTreeNodeT () {}
private:
    KdTree<T> _kdtree;
};

typedef KdTreeNodeT<DrawableNode*> KdTreeNode;
//typedef NodeMgr<KdTreeNode>        KdTreeNodeMgr;

#endif // _KDTREE_NODE_H_
