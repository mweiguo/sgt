#include "sgr_includes.h"
#include "sgr_nodeleter.h"

namespace SGR
{

void Nodeleter::operator() ( SGNode* node )
{
    node->accept ( *this );
}

//void Nodeleter::apply ( SGNode& node )
//{
//    ChildVisitor::apply ( node );
//    //for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
//    //    (*pp)->accept ( *this );
//}

void Nodeleter::apply ( LayerNode& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
    //for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
    //    (*pp)->accept ( *this );
    //delete &node;
}

void Nodeleter::apply ( PickableGroup& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
    //for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
    //    (*pp)->accept ( *this );
    //delete &node;
}

void Nodeleter::apply ( RectangleNodef& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
    //for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
    //    (*pp)->accept ( *this );
    //delete &node;
}

void Nodeleter::apply ( TransformNode& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
    //for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
    //    (*pp)->accept ( *this );
    //delete &node;
}

void Nodeleter::apply ( ArrayNode& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
    //for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
    //    (*pp)->accept ( *this );
    //delete &node;
}

void Nodeleter::apply ( LODNode& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
    //for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
    //    (*pp)->accept ( *this );
    //delete &node;
}

void Nodeleter::apply ( KdTreeNode& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
    //for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
    //    (*pp)->accept ( *this );
    //delete &node;
}

void Nodeleter::apply ( SceneNode& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
    //for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
    //    (*pp)->accept ( *this );
    //delete &node;
}

void Nodeleter::apply ( LineNodef& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
    //for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
    //    (*pp)->accept ( *this );
    //delete &node;
}

void Nodeleter::apply ( FontNode& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
}

void Nodeleter::apply ( TextNode& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
}

void Nodeleter::apply ( GroupNode& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
}

void Nodeleter::apply ( SwitchNode& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
}

void Nodeleter::apply ( AttrSet& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
}

void Nodeleter::apply ( MeshNode3f& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
}

void Nodeleter::apply ( MeshLineNode& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
}

void Nodeleter::apply ( PolylineNode2Df& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
}

void Nodeleter::apply ( PolyNode2Df& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
}

void Nodeleter::apply ( PointNode& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
}

void Nodeleter::apply ( MeshPointNode& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
}

void Nodeleter::apply ( CircleNode& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
}

void Nodeleter::apply ( ImageNode& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
}

void Nodeleter::apply ( ImposterNode& node )
{
    ChildVisitor::apply ( node );
    NodeMgr::getInst().deleteNode ( &node );
}

}
