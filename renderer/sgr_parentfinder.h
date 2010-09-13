#ifndef _PARENT_FINDER_H_
#define _PARENT_FINDER_H_
#include "sgr_parentvisitor.h"
namespace SGR
{

struct FalseType { enum { value = false }; };
struct TrueType { enum { value = true }; };

template <typename T1, typename T2>
struct IsSameType
{
    typedef FalseType Result;
};

template <typename T>
struct IsSameType<T,T>
{
    typedef TrueType Result;
};

template < class T >
class ParentFinder : public ParentVisitor
{
public:
    ParentFinder(SGNode* p) : _target(0)
    {
        SGNode* parent = p->getParentNode();
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
    } else 
        _target = dynamic_cast<T*>(&node);
}
}
#endif  // _PARENT_FINDER_H_
