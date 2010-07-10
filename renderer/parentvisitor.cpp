#include "sgl_includes.h"
#include "nodes.h"
#include "parentvisitor.h"
#include "meshnode.h"


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

void ParentVisitor::apply ( MeshNode& node )
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

