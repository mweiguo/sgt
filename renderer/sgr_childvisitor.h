#ifndef _CHILDVISITOR_H_
#define _CHILDVISITOR_H_

#include "sgr_nodevisitor.h"
#include "sgr_includes.h"
#include "sgr_polylinenode.h"
//#include "sgr_pickablegroup.h"
//#include "sgr_kdtreenode.h"
//#include "sgr_scenenode.h"
//#include "sgr_fontnode.h"
//#include "sgr_textnode.h"
//#include "sgr_linenode.h"
//#include "sgr_rectitem.h"
//#include "sgr_transformnode.h"
namespace SGR
{

    class SGR_DLL ChildVisitor : public NodeVisitor
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
        virtual void apply ( AttrSet& node );
        virtual void apply ( MeshNode3f& node );
        virtual void apply ( MeshLineNode& node );
        virtual void apply ( PolylineNode2Df& node );
        virtual void apply ( PolyNode2Df& node );
        virtual void apply ( PointNode& node );
        virtual void apply ( MeshPointNode& node );
    };


    inline void ChildVisitor::apply ( SGNode& node )
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

    inline void ChildVisitor::apply ( LayerNode& node )
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

    inline void ChildVisitor::apply ( RectangleNodef& node )
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

    inline void ChildVisitor::apply ( TransformNode& node )
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

    inline void ChildVisitor::apply ( ArrayNode& node )
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

    inline void ChildVisitor::apply ( LODNode& node )
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

    inline void ChildVisitor::apply ( PickableGroup& node )
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

    inline void ChildVisitor::apply ( KdTreeNode& node )
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

    inline void ChildVisitor::apply ( SceneNode& node )
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

    inline void ChildVisitor::apply ( FontNode& node )
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

    inline void ChildVisitor::apply ( TextNode& node )
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

    inline void ChildVisitor::apply ( GroupNode& node )
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

    inline void ChildVisitor::apply ( SwitchNode& node )
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

    inline void ChildVisitor::apply ( LineNodef& node )
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

    inline void ChildVisitor::apply ( AttrSet& node )
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

    inline void ChildVisitor::apply ( MeshNode3f& node )
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

    inline void ChildVisitor::apply ( MeshLineNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
    }

    inline void ChildVisitor::apply ( PolylineNode2Df& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
    }

    inline void ChildVisitor::apply ( PolyNode2Df& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
    }

    inline void ChildVisitor::apply ( PointNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
    }

    inline void ChildVisitor::apply ( MeshPointNode& node )
    {
        SGNode::iterator pp = node.begin();
        while ( pp!=node.end() )
        {
            SGNode::iterator p2 = pp++;
            (*p2)->accept ( *this );
        }
    }
}
#endif // _CHILDVISITOR_H_
