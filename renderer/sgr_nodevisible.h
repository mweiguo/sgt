#ifndef _NODEVISIBLE_H_
#define _NODEVISIBLE_H_
namespace SGR
{

class SGR_DLL NodeVisible : public NodeVisitor
{
public:
    NodeVisible(  ) : isVisible(true) {}
    virtual void apply ( LayerNode& node );
    virtual void apply ( PickableGroup& node );
    virtual void apply ( RectangleNodef& node );
    virtual void apply ( TransformNode& node );
    virtual void apply ( ArrayNode& node );
    virtual void apply ( LODNode& node );
    bool operator() (SGNode* node) 
    {
        node->accept ( *this );
        return isVisible; 
    }
private:
    bool isVisible;
};

inline void NodeVisible::apply ( LayerNode& node )
{
    isVisible = node.isVisible ();
    if ( false == isVisible )
        return;

    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

inline void NodeVisible::apply ( PickableGroup& node )
{
    isVisible = node.isVisible ();
    if ( false == isVisible )
        return;

    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

inline void NodeVisible::apply ( RectangleNodef& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

inline void NodeVisible::apply ( TransformNode& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

inline void NodeVisible::apply ( ArrayNode& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

inline void NodeVisible::apply ( LODNode& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}
}
#endif
