#ifndef _NODELETER_H_
#define _NODELETER_H_

#include "nodevisitor.h"

class AGEF_EXPORT Nodeleter : public NodeVisitor
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
    virtual void apply ( SceneNode& node );
    virtual void apply ( LineNodef& /*node*/ );
    void operator() ( SGNode* node );
};

#endif
