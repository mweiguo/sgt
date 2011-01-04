#include "sgr_crosspicker.h"

#include "sgr_childvisitor.h"
#include "sgr_kdtreenode.h"
#include "sgr_pickablegroup.h"
#include "sgr_grhagl.h"
#include "sgr_polylinenode.h"
#include "sgr_polynode.h"
#include "sgr_parentfinder.h"

namespace SGR
{

    // volume : a box in 3D

    CrossPicker::CrossPicker ()
    {
    }
    CrossPicker::CrossPicker ( const BBox& box, int camid ) : _bbox(box), _camid(camid)
    {
        _pickedNodes.clear();
    }
    CrossPicker::~CrossPicker ()
    {
    }
    void CrossPicker::initialize ( const BBox& box, int camid )
    {
        _bbox = box;
        _camid = camid;
        _pickedNodes.clear();
    }

    void CrossPicker::apply ( LayerNode& node )
    {
        if ( node.isVisible () )
        {
            if ( is_contain ( _bbox, node.getBBox() ) ||
                is_contain ( node.getBBox(), _bbox ) ||
                is_intersect ( node.getBBox(), _bbox ) )
                ChildVisitor::apply ( node );
        }
    }


    void CrossPicker::apply ( SceneNode& node )
    {
        if ( node.isVisible() )
        {
            if ( is_contain ( _bbox, node.getBBox() ) ||
                is_contain ( node.getBBox(), _bbox ) ||
                is_intersect ( node.getBBox(), _bbox ) )
            {
                const mat4f& m = node.getMatrix(), oldmat = _curmat;
                _curmat = m * _curmat;
                ChildVisitor::apply ( node );
                _curmat = oldmat;
            }
        }
    }


    void CrossPicker::apply ( RectangleNodef& node )
    {
        if ( node.isVisible () )
        {
            if ( is_contain ( _bbox, node.getBBox() ) ||
                is_contain ( node.getBBox(), _bbox ) ||
                is_intersect ( node.getBBox(), _bbox ) )
            {
                // add to picked list
                _pickedNodes.push_back ( &node );
            }
        }
        ChildVisitor::apply ( node );
    }


    void CrossPicker::apply ( TransformNode& node )
    {
        if ( node.isVisible () )
        {
            if ( is_contain ( _bbox, node.getBBox() ) ||
                is_contain ( node.getBBox(), _bbox ) ||
                is_intersect ( node.getBBox(), _bbox ) )
            {
                const mat4f& m = node.getMatrix(), oldmat = _curmat;
                _curmat = m * _curmat;
                ChildVisitor::apply ( node );
                _curmat = oldmat;
            }
        }
    }


    void CrossPicker::apply ( LODNode& node )
    {
        if ( node.isVisible() )
        {
            if ( is_contain ( _bbox, node.getBBox() ) ||
                is_contain ( node.getBBox(), _bbox ) ||
                is_intersect ( node.getBBox(), _bbox ) )
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
    }


    void CrossPicker::apply ( PickableGroup& node )
    {
        if ( node.isVisible () )
        {
            if ( is_contain ( _bbox, node.getBBox() ) ||
                is_contain ( node.getBBox(), _bbox ) ||
                is_intersect ( node.getBBox(), _bbox ) )
            {
                // add to picked list
                _pickedNodes.push_back ( &node );

                ChildVisitor::apply ( node );
            }
        }
    }


    void CrossPicker::apply ( KdTreeNode& node )
    {
        if ( node.isVisible () )
        {
            if ( is_contain ( _bbox, node.getBBox() ) ||
                is_contain ( node.getBBox(), _bbox ) ||
                is_intersect ( node.getBBox(), _bbox ) )
            {
                node.intersect ( _bbox, back_inserter(_pickedNodes) );
            }
        }
    }


    void CrossPicker::apply ( TextNode& node )
    {
        if ( node.isVisible () )
        {
            if ( is_contain ( _bbox, node.getBBox() ) ||
                is_contain ( node.getBBox(), _bbox ) ||
                is_intersect ( node.getBBox(), _bbox ) )
            {
                // add to picked list
                _pickedNodes.push_back ( &node );
            }

        }
        ChildVisitor::apply ( node );
    }


    void CrossPicker::apply ( GroupNode& node )
    {
        if ( node.isVisible () )
        {
            if ( is_contain ( _bbox, node.getBBox() ) ||
                is_contain ( node.getBBox(), _bbox ) ||
                is_intersect ( node.getBBox(), _bbox ) )
            {
                ChildVisitor::apply ( node );
            }
        }
    }


    void CrossPicker::apply ( SwitchNode& node )
    {
        if ( node.isVisible () )
        {
            if ( is_contain ( _bbox, node.getBBox() ) ||
                is_contain ( node.getBBox(), _bbox ) ||
                is_intersect ( node.getBBox(), _bbox ) )
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
    }


    void CrossPicker::apply ( LineNodef& node )
    {
        if ( node.isVisible () )
        {
            // check if line's end point in _bbox
            vec2f p1 = (_curmat * vec4f ( node.point1() )).xy();
            vec2f p2 = (_curmat * vec4f ( node.point2() )).xy();
            if ( is_intersect ( _bbox, Linef( p1.x(), p1.y(), p2.x(), p2.y() ) ) )
            {
                // add to picked list
                _pickedNodes.push_back ( &node );
            }
            ChildVisitor::apply ( node );
        }
    }


    void CrossPicker::apply ( MeshNode3f& node )
    {
        if ( node.isVisible () )
        {
            if ( is_contain ( _bbox, node.getBBox() ) ||
                is_contain ( node.getBBox(), _bbox ) ||
                is_intersect ( node.getBBox(), _bbox ) )
            {
                // this stack will be checked in it's children visitation, if stack is not empty, then child node will be picked
                _meshnodeStack.push_back ( &node );
                ChildVisitor::apply ( node );
            }
        }
    }


    void CrossPicker::apply ( MeshLineNode& node )
    {
        if ( !_meshnodeStack.empty() )
        {
            //// add to picked list
            //_pickedNodes.push_back ( &node );

            // get meshnode
            ParentFinder<MeshNode3f> finder (&node);
            if ( finder.target() )
            {
                MeshNode3f& meshnode = *(finder.target());
                MeshLineNode::pntiterator pp1=node.pntbegin(), pp=node.pntbegin();
                for ( ++pp1; pp1!=node.pntend(); ++pp, ++pp1 )
                {
                    vec2f p1 = (_curmat * vec4f ( meshnode[*pp] )).xy();
                    vec2f p2 = (_curmat * vec4f ( meshnode[*pp1] )).xy();
                    if ( is_intersect ( _bbox, Linef( p1.x(), p1.y(), p2.x(), p2.y() ) ) )
                    {
                        // add to picked list
                        _pickedNodes.push_back ( &node );
                        break;
                    }
                }

                ChildVisitor::apply ( node );
            }
        }
    }


    void CrossPicker::apply ( PolylineNode2Df& node )
    {
        if ( node.isVisible () )
        {
            PolylineNode2Df::pointiterator pp=node.pointbegin(), pp1=node.pointbegin();
            for ( ++pp1; pp1!=node.pointend(); ++pp1 )
            {
                vec2f p1 = (_curmat * vec4f ( *pp )).xy();
                vec2f p2 = (_curmat * vec4f ( *pp1 )).xy();
                if ( is_intersect ( _bbox, Linef( p1.x(), p1.y(), p2.x(), p2.y() ) ) )
                {
                    // add to picked list
                    _pickedNodes.push_back ( &node );
                    break;
                }
            }

            ChildVisitor::apply ( node );
        }
    }


    void CrossPicker::apply ( PolyNode2Df& node )
    {
        if ( node.isVisible () && is_contain (_bbox, node.getBBox()) )
        {
            bool isPicked = false;
            PolylineNode2Df::pointiterator pp=node.pointbegin(), pp1=node.pointbegin();
            for ( ++pp1; pp1!=node.pointend(); ++pp1 )
            {
                vec2f p1 = (_curmat * vec4f ( *pp )).xy();
                vec2f p2 = (_curmat * vec4f ( *pp1 )).xy();
                if ( is_intersect ( _bbox, Linef( p1.x(), p1.y(), p2.x(), p2.y() ) ) )
                {
                    // add to picked list
                    _pickedNodes.push_back ( &node );
                    isPicked = true;
                    break;
                }
            }

            // check front-back line seg
            if ( !isPicked )
            {
                vec2f p1 = (_curmat * vec4f ( node.pointfront() )).xy();
                vec2f p2 = (_curmat * vec4f ( node.pointback() )).xy();
                if ( is_intersect ( _bbox, Linef( p1.x(), p1.y(), p2.x(), p2.y() ) ) )
                {
                    // add to picked list
                    _pickedNodes.push_back ( &node );
                }
            }

            ChildVisitor::apply ( node );
        }
    }


    void CrossPicker::apply ( PointNode& node )
    {
        if ( node.isVisible () )
        {
            if ( is_contain (_bbox, node.getBBox()) ||
                is_contain (node.getBBox(), _bbox ) )
            {
                // add to picked list
                _pickedNodes.push_back ( &node );
                ChildVisitor::apply ( node );
            }
        }
    }


    void CrossPicker::apply ( MeshPointNode& node )
    {
        if ( node.isVisible () )
        {
            if ( is_contain (_bbox, node.getBBox()) ||
                is_contain (node.getBBox(), _bbox ) )
            {
                // add to picked list
                _pickedNodes.push_back ( &node );
                ChildVisitor::apply ( node );
            }
        }
    }

    void CrossPicker::apply ( CircleNode& node )
    {
        if ( node.isVisible () )
        {
            if ( is_contain (_bbox, node.getBBox()) ||
                is_contain (node.getBBox(), _bbox ) )
            {
                // add to picked list
                _pickedNodes.push_back ( &node );
                ChildVisitor::apply ( node );
            }
        }
    }

    void CrossPicker::apply ( ImageNode& node )
    {
        if ( node.isVisible () )
        {
            if ( is_contain (_bbox, node.getBBox()) ||
                is_contain (node.getBBox(), _bbox ) )
            {
                // add to picked list
                _pickedNodes.push_back ( &node );
                ChildVisitor::apply ( node );
            }
        }
    }

    void CrossPicker::apply ( ImposterNode& node )
    {
        if ( node.isVisible () )
        {
            if ( is_contain (_bbox, node.getBBox()) ||
                is_contain (node.getBBox(), _bbox ) )
            {
                // add to picked list
                _pickedNodes.push_back ( &node );
                ChildVisitor::apply ( node );
            }
        }
    }

    void CrossPicker::doAction ( SGNode& node )
    {
        node.accept ( *this ); 
    }

    void CrossPicker::clearPicked()
    {
        _pickedNodes.clear();
    }

}
