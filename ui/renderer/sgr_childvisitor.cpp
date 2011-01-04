#include "sgr_childvisitor.h"
#include "sgr_includes.h"

namespace SGR
{
    void ChildVisitor::apply ( SGNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
        //for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        //    (*pp)->accept ( *this );
    }

    void ChildVisitor::apply ( LayerNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
        //for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        //    (*pp)->accept ( *this );
    }

    void ChildVisitor::apply ( RectangleNodef& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
        //for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        //    (*pp)->accept ( *this );
    }

    void ChildVisitor::apply ( TransformNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
        //for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        //    (*pp)->accept ( *this );
    }

    void ChildVisitor::apply ( ArrayNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
        //for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        //    (*pp)->accept ( *this );
    }

    void ChildVisitor::apply ( LODNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
        //for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        //    (*pp)->accept ( *this );
    }

    void ChildVisitor::apply ( PickableGroup& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
        //for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        //    (*pp)->accept ( *this );
    }

    void ChildVisitor::apply ( KdTreeNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
        //for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        //    (*pp)->accept ( *this );
    }

    void ChildVisitor::apply ( SceneNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
        //for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        //    (*pp)->accept ( *this );
    }

    void ChildVisitor::apply ( FontNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
        //for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        //    (*pp)->accept ( *this );
    }

    void ChildVisitor::apply ( TextNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
        //for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        //    (*pp)->accept ( *this );
    }

    void ChildVisitor::apply ( GroupNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
        //for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        //    (*pp)->accept ( *this );
    }

    void ChildVisitor::apply ( SwitchNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
        //for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        //    (*pp)->accept ( *this );
    }

    void ChildVisitor::apply ( LineNodef& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
        //for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        //    (*pp)->accept ( *this );
    }

    void ChildVisitor::apply ( AttrSet& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
        //for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        //    (*pp)->accept ( *this );
    }

    void ChildVisitor::apply ( MeshNode3f& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
        //for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        //    (*pp)->accept ( *this );
    }

    void ChildVisitor::apply ( MeshLineNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
    }

    void ChildVisitor::apply ( PolylineNode2Df& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
    }

    void ChildVisitor::apply ( PolyNode2Df& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
    }

    void ChildVisitor::apply ( PointNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
    }

    void ChildVisitor::apply ( MeshPointNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
    }

    void ChildVisitor::apply ( CircleNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
    }

    void ChildVisitor::apply ( ImageNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
    }

    void ChildVisitor::apply ( ImposterNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
    }

}
