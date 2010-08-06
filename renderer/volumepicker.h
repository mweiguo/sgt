#ifndef _VOLUMEPICKER_H_
#define _VOLUMEPICKER_H_

#include "childvisitor.h"
#include "kdtreenode.h"
#include "pickablegroup.h"
#include "grhagl.h"

// volume : a box in 3D
template <class Output>
class VolumePicker : public ChildVisitor
{
public:
    VolumePicker ( const BBox& box, int camid/*this is for lod filter*/, Output output ) : _bbox(box), _output(output), _camid(camid) {}
    virtual void apply ( LayerNode& node );
    virtual void apply ( RectangleNodef& node );
    virtual void apply ( TransformNode& node );
    virtual void apply ( LODNode& node );
    virtual void apply ( PickableGroup& node );
    virtual void apply ( KdTreeNode& node );
    virtual void apply ( TextNode& node );
    virtual void apply ( LineNodef& node );
    virtual void apply ( SceneNode& node );
    virtual void apply ( MeshNode3f& node );
    virtual void apply ( MeshLineNode& node );
    void operator () ( SGNode& node ) { node.accept ( *this ); }
private:
    BBox _bbox;
    Output _output;
    int _camid;
    mat4f _curmat;
    vector<MeshNode3f*> _meshnodeStack;
};

template <class Output>
void VolumePicker<Output>::apply ( LayerNode& node )
{
    if ( node.isVisible () )
    {
        ChildVisitor::apply ( node );
    }
}

template <class Output>
void VolumePicker<Output>::apply ( SceneNode& node )
{
    if ( node.isVisible () )
    {
        const mat4f& m = node.getMatrix(), oldmat = _curmat;
        _curmat = m * _curmat;
        ChildVisitor::apply ( node );
        _curmat = oldmat;
    }
}

template <class Output>
void VolumePicker<Output>::apply ( RectangleNodef& node )
{
    if ( node.isVisible () )
    {
        vec2f pos = (_curmat * vec4f (0,0,0,1)).xy();

        BBox box;
        box.init ( vec3f(pos.x(), pos.y(), 0) );
        box.expandby ( vec3f(pos.x()+node.w(), pos.y()+node.h(), 0 ) );

        //qDebug ( "node's box %s", box.dump().c_str() );
        //qDebug ( "view _bbox %s", _bbox.dump().c_str() );
        if ( is_intersect ( box, _bbox ) )
        {
            RectangleNodef* rect = new RectangleNodef ( node );
            rect->setRect ( pos.x(), pos.y(), node.w(), node.h() );
            *_output++ = rect;
        }
    }
    ChildVisitor::apply ( node );
}

template <class Output>
void VolumePicker<Output>::apply ( TransformNode& node )
{
    if ( node.isVisible () )
    {
        const mat4f& m = node.getMatrix(), oldmat = _curmat;
        _curmat = m * _curmat;
        ChildVisitor::apply ( node );
        _curmat = oldmat;
    }
}

template <class Output>
void VolumePicker<Output>::apply ( LODNode& node )
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

template <class Output>
void VolumePicker<Output>::apply ( PickableGroup& node )
{
    if ( node.isVisible () )
    {
        ChildVisitor::apply ( node );
    }
}

template <class Output>
void VolumePicker<Output>::apply ( KdTreeNode& node )
{
    node.intersect ( _bbox, _output );
}

template <class Output>
void VolumePicker<Output>::apply ( TextNode& node )
{
    if ( node.isVisible () )
    {
        //   vec2f pos = (_curmat * vec4f (0,0,0,1)).xy();

        //qDebug ("rect (%f, %f, %f, %f)", pos.x(), pos.y(), node.w(), node.h());
        //qDebug ( "%s", _bbox.dump().c_str() );

        const BBox box = node.getBBox();
        if ( is_intersect ( box, _bbox ) )
        {
            TextNode* textnode = new TextNode ( node );
            textnode->removeAllChild ();
            //textnode->anchorPoint ( pos.x(), pos.y(), 0 );
            *_output++ = textnode;// ( pos.x(), pos.y(), box.max().x(), box.max().y() );
        }
    }
    ChildVisitor::apply ( node );
}

template <class Output>
void VolumePicker<Output>::apply ( LineNodef& node )
{
    if ( node.isVisible () )
    {
        const BBox box = node.getBBox();
        if ( is_intersect ( box, _bbox ) )
        {
            vec2f pos = (_curmat * vec4f (0,0,0,1)).xy();
            vec3f p1 ( pos.x()+node.x1(), pos.y()+node.y1(), 0.f );
            vec3f p2 ( pos.x()+node.x2(), pos.y()+node.y2(), 0.f );
            if ( is_contain ( _bbox, box ) ||
                is_intersect ( _bbox, Linef(p1.x(), p1.y(), p2.x(), p2.y() ) ) )
            {
                LineNodef* linenode = new LineNodef ( node );
                linenode->setPoints ( p1.x(), p1.y(), p2.x(), p2.y() );
                *_output++ = linenode;
            }
        }
    }
    ChildVisitor::apply ( node );
}

template <class Output>
void VolumePicker<Output>::apply ( MeshNode3f& node )
{
    if ( node.isVisible () )
    {
        const BBox box = node.getBBox();
        if ( is_intersect ( box, _bbox ) )
        {
            vec2f pos = (_curmat * vec4f (0,0,0,1)).xy();

            MeshNode3f* meshnode = new MeshNode3f ( node );
            MeshNode3f::coorditerator pp, end = meshnode->coordend();
            int idx = 0;
            for ( pp=meshnode->coordbegin(); pp!=end; ++pp, idx++ )
            {
                vec3f& pnt = node[idx];
                vec3f newpos ( pos.x()+pnt.x(), pos.y()+pnt.y(), 0.f );
                node.setCoords ( idx, newpos );
            }

            // this stack will be checked in it's children visitation, if stack is not empty, then child node will be picked
            _meshnodeStack.push_back ( meshnode );
        }
    }
    ChildVisitor::apply ( node );
}

template <class Output>
void VolumePicker<Output>::apply ( MeshLineNode& node )
{
    if ( !_meshnodeStack.empty() )
    {
        if ( node.isVisible () )
        {
            const BBox box = node.getBBox();
            if ( is_intersect ( box, _bbox ) )
            {
                MeshLineNode* meshlinenode = new MeshLineNode ( node );
                *_output++ = meshlinenode;
            }
        }
    }
    ChildVisitor::apply ( node );
}

#endif // _VOLUMNPICKER_H_
