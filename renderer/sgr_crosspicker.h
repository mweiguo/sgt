#ifndef _CROSSPICKER_H_
#define _CROSSPICKER_H_

#include "sgr_childvisitor.h"
#include <vector>
#include "sgr_mat4.h"
#include "sgr_bbox.h"

namespace SGR
{

// volume : a box in 3D
class CrossPicker : public ChildVisitor
{
public:
    CrossPicker ();
    CrossPicker ( const BBox& box, int camid );
    virtual ~CrossPicker ();
    void initialize ( const BBox& box, int camid );
    virtual void apply ( LayerNode& node );
    virtual void apply ( RectangleNodef& node );
    virtual void apply ( TransformNode& node );
    virtual void apply ( LODNode& node );
    virtual void apply ( PickableGroup& node );
    virtual void apply ( KdTreeNode& node );
    virtual void apply ( TextNode& node );
    virtual void apply ( GroupNode& node );
    virtual void apply ( SwitchNode& node );
    virtual void apply ( LineNodef& node );
    virtual void apply ( SceneNode& node );
    virtual void apply ( MeshNode3f& node );
    virtual void apply ( MeshLineNode& node );
    virtual void apply ( PolylineNode2Df& node );
    virtual void apply ( PolyNode2Df& node );
    virtual void apply ( PointNode& node );
    virtual void apply ( MeshPointNode& node );
    virtual void apply ( CircleNode& node );
    virtual void apply ( ImageNode& node );
    virtual void apply ( ImposterNode& node );
    void doAction ( SGNode& node );
    std::vector<SGNode*>& pickedNodes () { return _pickedNodes; }
    void clearPicked();
private:
    BBox _bbox;
    int _camid;
    mat4f _curmat;
    // this stack will be checked in it's children visitation, if stack is not empty, then child node will be picked
    std::vector<MeshNode3f*> _meshnodeStack;

    // store picked nodes
    std::vector<SGNode*> _pickedNodes;
};

}
#endif // _VOLUMNPICKER_H_
