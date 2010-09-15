#include "sgr_nodetransformer.h"

namespace SGR
{
    void NodeTransformer::translate ( SGNode* node, float tx, float ty, float tz )
    {
        _transformNodeStack.clear();
        _mat = mat4f::translate_matrix ( tx, ty, tz );
        node->accept ( *this );
    }

    void NodeTransformer::scale ( SGNode* node, float scale )
    {
        _transformNodeStack.clear();
        _mat = mat4f::scale_matrix ( scale, scale, scale );
        node->accept ( *this );
    }

    void NodeTransformer::apply ( SGNode& node )
    {
        transformBBox ( node );
        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( LayerNode& node )
    {
        transformBBox ( node );
        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( RectangleNodef& node )
    {
        transformBBox ( node );

        if ( _transformNodeStack.empty() )
        {
            vec4f p1 ( node.x(), node.y(), 0, 1 );
            vec4f p2 ( node.x()+node.w(), node.y()+node.h(), 0, 1 );
            p1 = _mat * p1;
            p2 = _mat * p2;
            node.setRect ( p1.x(), p1.y(), p2.x() - p1.x(), p2.y() - p1.y() );
        }

        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( TransformNode& node )
    {
        transformBBox ( node );
        node.setMatrix ( _mat * node.getMatrix() );
        _transformNodeStack.push_back ( 1 );
        ChildVisitor::apply ( node );
        _transformNodeStack.pop_back ();
    }
    void NodeTransformer::apply ( ArrayNode& node )
    {
        transformBBox ( node );
        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( LODNode& node )
    {
        transformBBox ( node );
        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( PickableGroup& node )
    {
        transformBBox ( node );
        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( KdTreeNode& node )
    {
        transformBBox ( node );
        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( SceneNode& node )
    {
        transformBBox ( node );
        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( FontNode& node )
    {
        transformBBox ( node );
        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( TextNode& node )
    {
        transformBBox ( node );
        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( GroupNode& node )
    {
        transformBBox ( node );
        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( SwitchNode& node )
    {
        transformBBox ( node );
        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( LineNodef& node )
    {
        transformBBox ( node );

        if ( _transformNodeStack.empty() )
        {
            vec4f p1 = _mat * vec4f( node.point1() );
            vec4f p2 = _mat * vec4f( node.point2() );
            node.setPoints ( p1.x(), p1.y(), p2.x(), p2.y() );
        }

        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( AttrSet& node )
    {
        transformBBox ( node );
        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( MeshNode3f& node )
    {
        transformBBox ( node );

        if ( _transformNodeStack.empty() )
        {
            MeshNode3f::coorditerator pp, end = node.coordend();
            int idx = 0;
            for ( pp=node.coordbegin(); pp!=end; ++pp, idx++ )
            {
                vec4f newpos = _mat * vec4f (node[idx]);
                node.setCoords ( idx, newpos.xyz() );
            }
        }

        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( MeshLineNode& node )
    {
        transformBBox ( node );

        // change mesh coord
        ParentFinder<MeshNode3f> finder(&node);
        if ( finder.target() )
        {
            MeshNode3f& meshnode = *(finder.target());
            for ( MeshLineNode::pntiterator pp=node.pntbegin(); pp!=node.pntend(); ++pp )
            {
                vec4f newpos = _mat * vec4f (meshnode[*pp]);
                meshnode[*pp] = newpos.xyz();
            }
        }

        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( PolylineNode2Df& node )
    {
        transformBBox ( node );

        if ( _transformNodeStack.empty() )
        {
            for ( PolylineNode2Df::pointiterator pp=node.pointbegin(); pp!=node.pointend(); ++pp )
            {
                vec4f newpos = _mat * vec4f ( *pp );
                *pp = newpos.xy();
            }
        }

        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( PolyNode2Df& node )
    {
        transformBBox ( node );

        if ( _transformNodeStack.empty() )
        {
            for ( PolyNode2Df::pointiterator pp=node.pointbegin(); pp!=node.pointend(); ++pp )
            {
                vec4f newpos = _mat * vec4f ( *pp );
                *pp = newpos.xy();
            }
        }

        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( PointNode& node )
    {
        transformBBox ( node );

        if ( _transformNodeStack.empty() )
        {
            vec4f newpos = _mat * vec4f ( node.xyz() );
            node.xyz ( newpos.x(), newpos.y(), newpos.z() );
        }

        ChildVisitor::apply ( node );
    }
    void NodeTransformer::apply ( MeshPointNode& node )
    {
        transformBBox ( node );

        vec3f coord = node.getCoord();
        node.setCoord ( (_mat * vec4f ( coord )).xyz() );

        ChildVisitor::apply ( node );
    }
    void NodeTransformer::transformBBox ( SGNode& node )
    {
        BBox bbox = node.getBBox();
        vec4f nmin = _mat * vec4f ( bbox.minvec() );
        vec4f nmax = _mat * vec4f ( bbox.maxvec() );
        bbox.setminmax ( nmin.xyz(), nmax.xyz() );
        node.setBBox ( bbox );
    }

}
