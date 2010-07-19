#ifndef _NODEVISITOR_NODE_H_
#define _NODEVISITOR_NODE_H_

#include "agef_global.h"
//#include "nodes.h"
class SGNode;

template < class T >
class RectangleNode;
typedef RectangleNode<float> RectangleNodef;
class TransformNode;
class ArrayNode;
class LODNode;
class LayerNode;
class PickableGroup;
class DrawableNode;
template < class T>
class KdTreeNodeT;
class MeshNode;
class FontNode;
class TextNode;
class GroupNode;
class SwitchNode;
typedef KdTreeNodeT<DrawableNode*> KdTreeNode;
template < class T >
class LineNode;
typedef LineNode<float> LineNodef;
class AttrSet;

class AGEF_EXPORT NodeVisitor 
{
public:
    NodeVisitor(){}
    virtual void apply ( const SGNode& /*node*/ ) {}
    virtual void apply ( const LayerNode& /*node*/ ) {}
    virtual void apply ( const RectangleNodef& /*node*/ ) {}
    virtual void apply ( const TransformNode& /*node*/ ) {}
    virtual void apply ( const ArrayNode& /*node*/ ) {}
    virtual void apply ( const LODNode& /*node*/ ) {}
    virtual void apply ( const KdTreeNode& /*node*/ ) {}
    virtual void apply ( const MeshNode& /*node*/ ) {}
    virtual void apply ( const FontNode& node ) {}
    virtual void apply ( const TextNode& node ) {}
    virtual void apply ( const GroupNode& node ) {}
    virtual void apply ( const SwitchNode& node ) {}
    virtual void apply ( const LineNodef& node ) {}
    virtual void apply ( SGNode& /*node*/ ) {}
    virtual void apply ( LayerNode& /*node*/ ) {}
    virtual void apply ( RectangleNodef& /*node*/ ) {}
    virtual void apply ( TransformNode& /*node*/ ) {}
    virtual void apply ( ArrayNode& /*node*/ ) {}
    virtual void apply ( LODNode& /*node*/ ) {}
    virtual void apply ( PickableGroup& /*node*/ ) {}
    virtual void apply ( KdTreeNode& /*node*/ ) {}
    virtual void apply ( MeshNode& /*node*/ ) {}
    virtual void apply ( FontNode& node ) {}
    virtual void apply ( TextNode& node ) {}
    virtual void apply ( GroupNode& node ) {}
    virtual void apply ( SwitchNode& node ) {}
    virtual void apply ( LineNodef& node ) {}
    virtual void apply ( AttrSet& node ){}
};


#endif
