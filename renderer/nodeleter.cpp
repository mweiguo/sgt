#include "sgl_includes.h"
#include "nodeleter.h"

void Nodeleter::operator() ( SGNode* node )
{
    node->accept ( *this );
}

void Nodeleter::apply ( SGNode& node )
{
    for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
}

void Nodeleter::apply ( LayerNode& node )
{
    for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    delete &node;
}

void Nodeleter::apply ( PickableGroup& node )
{
    for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    delete &node;
}

void Nodeleter::apply ( RectangleNodef& node )
{
    for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    delete &node;
}

void Nodeleter::apply ( TransformNode& node )
{
    for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    delete &node;
}

void Nodeleter::apply ( ArrayNode& node )
{
    for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    delete &node;
}

void Nodeleter::apply ( LODNode& node )
{
    for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    delete &node;
}

void Nodeleter::apply ( KdTreeNode& node )
{
    for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    delete &node;
}

void Nodeleter::apply ( MeshNode& node )
{
    for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    delete &node;
}

void Nodeleter::apply ( LineNodef& node )
{
    for ( SGNode::const_iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    delete &node;
}

