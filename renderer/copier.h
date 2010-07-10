#ifndef _COPYIER_H_
#define _COPYIER_H_

#include "childvisitor.h"

class AGEF_EXPORT Copier : public ChildVisitor
{
public:
    Copier () : _currparent(0) {}
    virtual void apply ( SGNode& node );
    virtual void apply ( LayerNode& node );
    virtual void apply ( RectangleNodef& node );
    virtual void apply ( TransformNode& node );
    virtual void apply ( ArrayNode& node );
    virtual void apply ( LODNode& node );
    virtual void apply ( PickableGroup& node );
    virtual void apply ( KdTreeNode& node );
    virtual void apply ( MeshNode& node );
    virtual void apply ( FontNode& node );
    virtual void apply ( TextNode& node );
    virtual void apply ( LineNodef& node );
    template < class T >
    T* operator () ( T& rhs )
    {
        SGNode tmp;
        _currparent = &tmp;

        rhs.accept ( *this );

        T* t = *(tmp.begin());
        tmp.removeAllChild ();
        return t;
    }
private:
    SGNode* _currparent;
};

inline void Copier::apply ( SGNode& node )
{
    SGNode* p = new SGNode ( node );
    p->setParentNode ( NULL );
    p->removeAllChild ();
    _currparent->addChild ( p );
    SGNode* old = _currparent;
    _currparent = p;
    ChildVisitor::apply ( node );
    _currparent = old;
}

inline void Copier::apply ( LayerNode& node )
{
    LayerNode* p = new LayerNode ( node );
    p->setParentNode ( NULL );
    p->removeAllChild ();
    _currparent->addChild ( p );
    SGNode* old = _currparent;
    _currparent = p;
    ChildVisitor::apply ( node );
    _currparent = old;
}

inline void Copier::apply ( RectangleNodef& node )
{
    RectangleNodef* p = new RectangleNodef ( node );
    p->setParentNode ( NULL );
    p->removeAllChild ();
    _currparent->addChild ( p );
    SGNode* old = _currparent;
    _currparent = p;
    ChildVisitor::apply ( node );
    _currparent = old;
}

inline void Copier::apply ( TransformNode& node )
{
    TransformNode* p = new TransformNode ( node );
    p->setParentNode ( NULL );
    p->removeAllChild ();
    _currparent->addChild ( p );
    SGNode* old = _currparent;
    _currparent = p;
    ChildVisitor::apply ( node );
    _currparent = old;
}

inline void Copier::apply ( ArrayNode& node )
{
    ArrayNode* p = new ArrayNode ( node );
    p->setParentNode ( NULL );
    p->removeAllChild ();
    _currparent->addChild ( p );
    SGNode* old = _currparent;
    _currparent = p;
    ChildVisitor::apply ( node );
    _currparent = old;
}

inline void Copier::apply ( LODNode& node )
{
    LODNode* p = new LODNode ( node );
    p->setParentNode ( NULL );
    p->removeAllChild ();
    _currparent->addChild ( p );
    SGNode* old = _currparent;
    _currparent = p;
    ChildVisitor::apply ( node );
    _currparent = old;
}

inline void Copier::apply ( PickableGroup& node )
{
    PickableGroup* p = new PickableGroup ( node );
    p->setParentNode ( NULL );
    p->removeAllChild ();
    _currparent->addChild ( p );
    SGNode* old = _currparent;
    _currparent = p;
    ChildVisitor::apply ( node );
    _currparent = old;
}

inline void Copier::apply ( KdTreeNode& node )
{
    KdTreeNode* p = new KdTreeNode ( node );
    p->setParentNode ( NULL );
    p->removeAllChild ();
    _currparent->addChild ( p );
    SGNode* old = _currparent;
    _currparent = p;
    ChildVisitor::apply ( node );
    _currparent = old;
}

inline void Copier::apply ( MeshNode& node )
{
    MeshNode* p = new MeshNode ( node );
    p->setParentNode ( NULL );
    p->removeAllChild ();
    _currparent->addChild ( p );
    SGNode* old = _currparent;
    _currparent = p;
    ChildVisitor::apply ( node );
    _currparent = old;
}

inline void Copier::apply ( FontNode& node )
{
    FontNode* p = new FontNode ( node );
    p->setParentNode ( NULL );
    p->removeAllChild ();
    _currparent->addChild ( p );
    SGNode* old = _currparent;
    _currparent = p;
    ChildVisitor::apply ( node );
    _currparent = old;
}

inline void Copier::apply ( TextNode& node )
{
    TextNode* p = new TextNode ( node );
    p->setParentNode ( NULL );
    p->removeAllChild ();
    _currparent->addChild ( p );
    SGNode* old = _currparent;
    _currparent = p;
    ChildVisitor::apply ( node );
    _currparent = old;
}

inline void Copier::apply ( LineNodef& node )
{
    LineNodef* p = new LineNodef ( node );
    p->setParentNode ( NULL );
    p->removeAllChild ();
    _currparent->addChild ( p );
    SGNode* old = _currparent;
    _currparent = p;
    ChildVisitor::apply ( node );
    _currparent = old;
}

#endif // _COPYIER_H_
