#ifndef _CHILDREN_FINDER_H_
#define _CHILDREN_FINDER_H_
#include "sgr_nodes.h"
#include "sgr_childvisitor.h"
#include "sgr_grhagl.h"

namespace SGR
{
    template < class T >
    class ChildrenFinder : public NodeVisitor
    {
    public:
        ChildrenFinder( SGNode* p)
        {
            for ( SGNode::iterator pp=p->begin(); pp!=p->end(); ++pp )
            {
                (*pp)->accept ( *this );
            }
        }
        virtual void apply ( SGNode& node )
        {
            if ( true == IsSameType<T, SGNode>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( LayerNode& node )
        {
            if ( true == IsSameType<T, LayerNode>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( RectangleNodef& node )
        {
            if ( true == IsSameType<T, RectangleNodef>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( TransformNode& node )
        {
            if ( true == IsSameType<T, TransformNode>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( ArrayNode& node )
        {
            if ( true == IsSameType<T, ArrayNode>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( LODNode& node )
        {
            if ( true == IsSameType<T, LODNode>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( PickableGroup& node )
        {
            if ( true == IsSameType<T, PickableGroup>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( KdTreeNode& node )
        {
            if ( true == IsSameType<T, KdTreeNode>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( SceneNode& node )
        {
            if ( true == IsSameType<T, SceneNode>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( FontNode& node )
        {
            if ( true == IsSameType<T, FontNode>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( TextNode& node )
        {
            if ( true == IsSameType<T, TextNode>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( GroupNode& node )
        {
            if ( true == IsSameType<T, GroupNode>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( SwitchNode& node )
        {
            if ( true == IsSameType<T, SwitchNode>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( LineNodef& node )
        {
            if ( true == IsSameType<T, LineNodef>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( MeshNode3f& node )
        {
            if ( true == IsSameType<T, MeshNode3f>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( MeshLineNode& node )
        {
            if ( true == IsSameType<T, MeshLineNode>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( MeshPointNode& node )
        {
            if ( true == IsSameType<T, MeshPointNode>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( PolylineNode2Df& node )
        {
            if ( true == IsSameType<T, PolylineNode2Df>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( PolyNode2Df& node )
        {
            if ( true == IsSameType<T, PolyNode2Df>::Result::value )
                _values.push_back ( &node );
        }
        virtual void apply ( PointNode& node )
        {
            if ( true == IsSameType<T, PointNode>::Result::value )
                _values.push_back ( &node );
        }
        std::list<SGNode*>& results () { return _values; }
    protected:
        std::list<SGNode*> _values;
    };

}
#endif  // _CHILDREN_FINDER_H_
