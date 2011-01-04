#ifndef _PARENT_FINDER_H_
#define _PARENT_FINDER_H_
#include "sgr_parentvisitor.h"
#include "sgr_nodes.h"
#include "sgr_grhagl.h"

namespace SGR
{
    template < class T >
    class ParentFinder : public ParentVisitor
    {
    public:
        ParentFinder( const SGNode* p) : _target(0)
        {
            SGNode* parent = const_cast<SGNode*> ( p->getParentNode() );
            if ( parent )
                parent->accept ( *this );
        }
        virtual void apply ( LayerNode& node );
        virtual void apply ( RectangleNodef& node );
        virtual void apply ( TransformNode& node );
        virtual void apply ( ArrayNode& node );
        virtual void apply ( LODNode& node );
        virtual void apply ( PickableGroup& node );
        virtual void apply ( KdTreeNode& node );
        virtual void apply ( SceneNode& node );
        virtual void apply ( FontNode& node );
        virtual void apply ( TextNode& node );
        virtual void apply ( LineNodef& node );
        virtual void apply ( MeshNode3f& node );
        virtual void apply ( MeshLineNode& node );
        virtual void apply ( MeshPointNode& node );
        virtual void apply ( CircleNode& node );
        virtual void apply ( ImageNode& node );

        T* target () { return _target; }

    private:
        T* _target;
    };

    template < class T >
    void ParentFinder<T>::apply ( LayerNode& node )
    {
        if ( false == IsSameType<T, LayerNode>::Result::value ) {
            ParentVisitor::apply ( node );
        } else 
            _target = dynamic_cast<T*>(&node);
    }

    template < class T >
    void ParentFinder<T>::apply ( RectangleNodef& node )
    {
        if ( false == IsSameType<T, RectangleNodef>::Result::value ) {
            ParentVisitor::apply ( node );
        } else 
            _target = dynamic_cast<T*>(&node);
    }

    template < class T >
    void ParentFinder<T>::apply ( TransformNode& node )
    {
        if ( false == IsSameType<T, TransformNode>::Result::value ) {
            ParentVisitor::apply ( node );
        } else 
            _target = dynamic_cast<T*>(&node);
    }

    template < class T >
    void ParentFinder<T>::apply ( ArrayNode& node )
    {
        if ( false == IsSameType<T, ArrayNode>::Result::value ) {
            ParentVisitor::apply ( node );
        } else 
            _target = dynamic_cast<T*>(&node);
    }

    template < class T >
    void ParentFinder<T>::apply ( LODNode& node )
    {
        if ( false == IsSameType<T, LODNode>::Result::value ) {
            ParentVisitor::apply ( node );
        } else 
            _target = dynamic_cast<T*>(&node);
    }

    template < class T >
    void ParentFinder<T>::apply ( PickableGroup& node )
    {
        if ( false == IsSameType<T, PickableGroup>::Result::value ) {
            ParentVisitor::apply ( node );
        } else 
            _target = dynamic_cast<T*>(&node);
    }

    template < class T >
    void ParentFinder<T>::apply ( KdTreeNode& node )
    {
        if ( false == IsSameType<T, KdTreeNode>::Result::value ) {
            ParentVisitor::apply ( node );
        } else 
            _target = dynamic_cast<T*>(&node);
    }

    template < class T >
    void ParentFinder<T>::apply ( SceneNode& node )
    {
        if ( false == IsSameType<T, SceneNode>::Result::value ) {
            ParentVisitor::apply ( node );
        } else 
            _target = dynamic_cast<T*>(&node);
    }

    template < class T >
    void ParentFinder<T>::apply ( FontNode& node )
    {
        if ( false == IsSameType<T, FontNode>::Result::value ) {
            ParentVisitor::apply ( node );
        } else 
            _target = dynamic_cast<T*>(&node);
    }

    template < class T >
    void ParentFinder<T>::apply ( TextNode& node )
    {
        if ( false == IsSameType<T, TextNode>::Result::value ) {
            ParentVisitor::apply ( node );
        } else 
            _target = dynamic_cast<T*>(&node);
    }

    template < class T >
    void ParentFinder<T>::apply ( LineNodef& node )
    {
        if ( false == IsSameType<T, LineNodef>::Result::value ) {
            ParentVisitor::apply ( node );
        } else 
            _target = dynamic_cast<T*>(&node);
    }

    template < class T >
    void ParentFinder<T>::apply ( MeshNode3f& node )
    {
        if ( false == IsSameType<T, MeshNode3f>::Result::value ) {
            ParentVisitor::apply ( node );
        } else 
            _target = dynamic_cast<T*>(&node);
    }

    template < class T >
    void ParentFinder<T>::apply ( MeshLineNode& node )
    {
        if ( false == IsSameType<T, MeshLineNode>::Result::value ) {
            ParentVisitor::apply ( node );
        } else {
            _target = dynamic_cast<T*>(&node);
        }
    }

    template < class T >
    void ParentFinder<T>::apply ( MeshPointNode& node )
    {
        if ( false == IsSameType<T, MeshPointNode>::Result::value ) {
            ParentVisitor::apply ( node );
        } else {
            _target = dynamic_cast<T*>(&node);
        }
    }

    template < class T >
    void ParentFinder<T>::apply ( CircleNode& node )
    {
        if ( false == IsSameType<T, CircleNode>::Result::value ) {
            ParentVisitor::apply ( node );
        } else {
            _target = dynamic_cast<T*>(&node);
        }
    }

    template < class T >
    void ParentFinder<T>::apply ( ImageNode& node )
    {
        if ( false == IsSameType<T, ImageNode>::Result::value ) {
            ParentVisitor::apply ( node );
        } else {
            _target = dynamic_cast<T*>(&node);
        }
    }


}
#endif  // _PARENT_FINDER_H_
