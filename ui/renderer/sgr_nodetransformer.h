#ifndef _SGR_NODETRANSFORMER_H_
#define _SGR_NODETRANSFORMER_H_

#include "sgr_childvisitor.h"
#include "sgr_mat4.h"
#include <vector>

namespace SGR
{

class NodeTransformer : public ChildVisitor
{
public:
    void translate ( SGNode* node, float tx, float ty, float tz );
    void scale ( SGNode* node, float scale );
    virtual ~NodeTransformer ();
    virtual void apply ( SGNode& node );
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
    virtual void apply ( GroupNode& node );
    virtual void apply ( SwitchNode& node );
    virtual void apply ( LineNodef& node );
    virtual void apply ( AttrSet& node );
    virtual void apply ( MeshNode3f& node );
    virtual void apply ( MeshLineNode& node );
    virtual void apply ( PolylineNode2Df& node );
    virtual void apply ( PolyNode2Df& node );
    virtual void apply ( PointNode& node );
    virtual void apply ( MeshPointNode& node );
    virtual void apply ( CircleNode& node );
    virtual void apply ( ImageNode& node );
    virtual void apply ( ImposterNode& node );
protected:
    void transformBBox ( SGNode& node );
protected:
    mat4f _mat;
    std::vector < int > _transformNodeStack;
};

}

#endif //_SGR_NODETRANSFORMER_H_
