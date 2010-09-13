#ifndef _NODEVISITOR_NODE_H_
#define _NODEVISITOR_NODE_H_

#include "sgr_global.h"
#include <sgr_vec3.h>
namespace SGR
{

    //#include "sgr_nodes.h"
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
    class SceneNode;
    class FontNode;
    class TextNode;
    class GroupNode;
    class SwitchNode;
    typedef KdTreeNodeT<DrawableNode*> KdTreeNode;
    template < class T >
    class LineNode;
    typedef LineNode<float> LineNodef;
    class AttrSet;
    class MeshLineNode;
    template < class COORDT >
    class MeshNode;
    typedef MeshNode<vec3f >      MeshNode3f;
    template < class T >
    class PolylineNode;
    typedef PolylineNode<vec2f>  PolylineNode2Df;
    template < class T >
    class PolyNode;
    typedef PolyNode<vec2f>  PolyNode2Df;
    class PointNode;
    class MeshPointNode;

    class SGR_DLL NodeVisitor 
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
        virtual void apply ( const SceneNode& /*node*/ ) {}
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
        virtual void apply ( SceneNode& /*node*/ ) {}
        virtual void apply ( FontNode& /*node*/ ) {}
        virtual void apply ( TextNode& /*node*/ ) {}
        virtual void apply ( GroupNode& /*node*/ ) {}
        virtual void apply ( SwitchNode& /*node*/ ) {}
        virtual void apply ( LineNodef& /*node*/ ) {}
        virtual void apply ( AttrSet& /*node*/ ){}
        virtual void apply ( MeshNode3f& /*node*/ ) {}
        virtual void apply ( MeshLineNode& /*node*/ ) {}
        virtual void apply ( PolylineNode2Df& /*node*/ ) {}
        virtual void apply ( PolyNode2Df& /*node*/ ) {}
        virtual void apply ( PointNode& /*node*/ ) {}
        virtual void apply ( MeshPointNode& /*node*/ ) {}
   };
}

#endif
