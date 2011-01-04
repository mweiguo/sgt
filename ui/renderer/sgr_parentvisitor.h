#ifndef _PARENT_VISITOR_H_
#define _PARENT_VISITOR_H_

#include "sgr_nodevisitor.h"
namespace SGR
{


    class SGR_DLL ParentVisitor : public NodeVisitor
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
	virtual void apply ( FontNode& node );
	virtual void apply ( TextNode& node );
	virtual void apply ( GroupNode& node );
	virtual void apply ( SwitchNode& node );
	virtual void apply ( LineNodef& node );
	virtual void apply ( MeshNode3f& node );
	virtual void apply ( MeshLineNode& node );
	virtual void apply ( PolylineNode2Df& node );
	virtual void apply ( PolyNode2Df& node );
	virtual void apply ( PointNode& node );
	virtual void apply ( MeshPointNode& node );
	virtual void apply ( CircleNode& node );
	virtual void apply ( ImageNode& node );
	virtual void apply ( ImposterNode& node );
    };

}
#endif //_PARENT_VISITOR_H_
