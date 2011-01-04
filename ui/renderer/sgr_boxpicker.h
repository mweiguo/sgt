#ifndef _BOXPICKER_H_
#define _BOXPICKER_H_

#include "sgr_childvisitor.h"
#include "sgr_kdtreenode.h"
#include "sgr_pickablegroup.h"
#include "sgr_grhagl.h"
#include "sgr_polylinenode.h"
#include "sgr_polynode.h"

namespace SGR
{

// volume : a box in 3D

class BoxPicker : public ChildVisitor
{
public:
    BoxPicker ()
    {
    }
    BoxPicker ( const BBox& box, int camid ) : _bbox(box), _camid(camid)
    {
        _pickedNodes.clear();
    }
    virtual ~BoxPicker ()
    {
    }
    void initialize ( const BBox& box, int camid )
    {
        _bbox = box;
        _camid = camid;
        _pickedNodes.clear();
    }
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

    void doAction ( SGNode& node ) { node.accept ( *this ); }
    vector<SGNode*>& pickedNodes () { return _pickedNodes; }
    void clearPicked() { _pickedNodes.clear(); }
private:
    BBox _bbox;
    int _camid;
    mat4f _curmat;
    // this stack will be checked in it's children visitation, if stack is not empty, then child node will be picked
    vector<MeshNode3f*> _meshnodeStack;

    // store picked nodes
    vector<SGNode*> _pickedNodes;
};


void BoxPicker::apply ( LayerNode& node )
{
    if ( !node.isVisible () || is_separate (_bbox, node.getBBox()) )
        return;

    {
        ChildVisitor::apply ( node );
    }
}


void BoxPicker::apply ( SceneNode& node )
{
    if ( !node.isVisible () || is_separate (_bbox, node.getBBox()) )
        return;

    {
        const mat4f& m = node.getMatrix(), oldmat = _curmat;
        _curmat = m * _curmat;
        ChildVisitor::apply ( node );
        _curmat = oldmat;
    }
}


void BoxPicker::apply ( RectangleNodef& node )
{
    if ( node.isVisible () && is_contain (_bbox, node.getBBox()) )
    {
        // add to picked list
        _pickedNodes.push_back ( &node );
    }
    ChildVisitor::apply ( node );
}


void BoxPicker::apply ( TransformNode& node )
{
    if ( !node.isVisible () || is_separate (_bbox, node.getBBox()) )
        return;

    {
        const mat4f& m = node.getMatrix(), oldmat = _curmat;
        _curmat = m * _curmat;
        ChildVisitor::apply ( node );
        _curmat = oldmat;
    }
}


void BoxPicker::apply ( LODNode& node )
{
    if ( !node.isVisible () || is_separate (_bbox, node.getBBox()) )
        return;

    {
        CameraOrtho* cam = NodeMgr::getInst().getNodePtr<CameraOrtho>(_camid);
        if ( cam )
        {
            SGNode* sgnode = node.selectPresentation ( cam->mvmatrix().sx() );
            if ( sgnode )
            {
                sgnode->accept ( *this );
            }
        }
    }
}


void BoxPicker::apply ( PickableGroup& node )
{
    if ( !node.isVisible () || is_separate (_bbox, node.getBBox()) )
        return;

    {
        // add to picked list
        _pickedNodes.push_back ( &node );

        ChildVisitor::apply ( node );
    }
}


void BoxPicker::apply ( KdTreeNode& node )
{
    if ( !node.isVisible () || is_separate (_bbox, node.getBBox()) )
        return;

    {
        //node.intersect ( _bbox, _output );
    }
}


void BoxPicker::apply ( TextNode& node )
{
    if ( node.isVisible () && is_contain (_bbox, node.getBBox()) )
    {
        // add to picked list
        _pickedNodes.push_back ( &node );

    }
    ChildVisitor::apply ( node );
}


void BoxPicker::apply ( GroupNode& node )
{
    if ( !node.isVisible () || is_separate (_bbox, node.getBBox()) )
        return;

    {
        ChildVisitor::apply ( node );
    }
}


void BoxPicker::apply ( SwitchNode& node )
{
    if ( !node.isVisible () || is_separate (_bbox, node.getBBox()) )
        return;

    {
        if ( node.selection() >= 0 && (unsigned int)(node.selection()) < node.size() )
        {
            int i=0;
            SGNode::iterator pp = node.begin();
            while ( i<node.selection() )
            {
                ++pp;
                ++i;
            }

            (*pp)->accept ( *this );
        }
        else if ( node.selection() < 0 )
        {
            ChildVisitor::apply ( node );
        }
    }
}


void BoxPicker::apply ( LineNodef& node )
{
    if ( node.isVisible () && is_contain (_bbox, node.getBBox()) )
    {
        // add to picked list
        _pickedNodes.push_back ( &node );
        ChildVisitor::apply ( node );
    }
}


void BoxPicker::apply ( MeshNode3f& node )
{
    if ( !node.isVisible () || is_separate (_bbox, node.getBBox()) )
        return;

    {
        // this stack will be checked in it's children visitation, if stack is not empty, then child node will be picked
        _meshnodeStack.push_back ( &node );
        ChildVisitor::apply ( node );
    }
}


void BoxPicker::apply ( MeshLineNode& node )
{
    if ( !_meshnodeStack.empty() && node.isVisible () && is_contain (_bbox, node.getBBox()) )
    {
        // add to picked list
        _pickedNodes.push_back ( &node );

        ChildVisitor::apply ( node );
    }
}


void BoxPicker::apply ( PolylineNode2Df& node )
{
    if ( node.isVisible () && is_contain (_bbox, node.getBBox()) )
    {
        // add to picked list
        _pickedNodes.push_back ( &node );

        ChildVisitor::apply ( node );
    }
}


void BoxPicker::apply ( PolyNode2Df& node )
{
    if ( node.isVisible () && is_contain (_bbox, node.getBBox()) )
    {
        // add to picked list
        _pickedNodes.push_back ( &node );

        ChildVisitor::apply ( node );
    }
}


void BoxPicker::apply ( PointNode& node )
{
    if ( node.isVisible () && is_contain (_bbox, node.getBBox()) )
    {
        // add to picked list
        _pickedNodes.push_back ( &node );
        ChildVisitor::apply ( node );
    }
}


void BoxPicker::apply ( MeshPointNode& node )
{
    if ( node.isVisible () && is_contain (_bbox, node.getBBox()) )
    {
        // add to picked list
        _pickedNodes.push_back ( &node );
        ChildVisitor::apply ( node );
    }
}

void BoxPicker::apply ( CircleNode& node )
{
    if ( node.isVisible () && is_contain (_bbox, node.getBBox()) )
    {
        // add to picked list
        _pickedNodes.push_back ( &node );
        ChildVisitor::apply ( node );
    }
}

void BoxPicker::apply ( ImageNode& node )
{
    if ( node.isVisible () && is_contain (_bbox, node.getBBox()) )
    {
        // add to picked list
        _pickedNodes.push_back ( &node );
        ChildVisitor::apply ( node );
    }
}

void BoxPicker::apply ( ImposterNode& node )
{
    if ( node.isVisible () && is_contain (_bbox, node.getBBox()) )
    {
        // add to picked list
        _pickedNodes.push_back ( &node );
        ChildVisitor::apply ( node );
    }
}


}
#endif // _VOLUMNPICKER_H_
