#ifndef _PARENT_VISITOR_H_
#define _PARENT_VISITOR_H_

#include "nodevisitor.h"


class AGEF_EXPORT ParentVisitor : public NodeVisitor
{
public:
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
    virtual void apply ( GroupNode& node );
    virtual void apply ( SwitchNode& node );
    virtual void apply ( LineNodef& /*node*/ );
};


#endif //_PARENT_VISITOR_H_
