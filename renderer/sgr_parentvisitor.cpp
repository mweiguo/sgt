#include "sgr_includes.h"
#include "sgr_nodes.h"
#include "sgr_parentvisitor.h"
#include "sgr_scenenode.h"
#include "sgr_polylinenode.h"

namespace SGR
{

void ParentVisitor::apply ( SGNode& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( LayerNode& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( RectangleNodef& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( TransformNode& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( ArrayNode& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( LODNode& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( PickableGroup& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( KdTreeNode& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( SceneNode& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( FontNode& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( TextNode& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( GroupNode& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( SwitchNode& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( LineNodef& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( MeshNode3f& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( MeshLineNode& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( PolylineNode2Df& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( PolyNode2Df& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( PointNode& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

void ParentVisitor::apply ( MeshPointNode& node )
{
    SGNode* parent = node.getParentNode();
    if ( parent )
        parent->accept ( *this );
}

}
