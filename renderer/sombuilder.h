#ifndef _SOM_BUILDER_H_
#define _SOM_BUILDER_H_

#include "nodevisitor.h"

// scene object manager
//class SomBuilder : public NodeVisitor
//{
//public:
//    virtual void apply ( SGNode& node );
//    virtual void apply ( LayerNode& node );
//    virtual void apply ( Rectanglef& node );
//    virtual void apply ( TransformNode& node );
//    virtual void apply ( ArrayNode& node );
//    virtual void apply ( LODNode& node );
//};
//
//inline void SomBuilder::apply ( LODNode& node )
//{
//    node.buildsom ();
//}
//
//inline void SomBuilder::apply ( SGNode& node )
//{
//    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
//        (*pp)->accept ( *this );
//}
//
//inline void SomBuilder::apply ( LayerNode& node )
//{
//    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
//        (*pp)->accept ( *this );
//}
//
//inline void SomBuilder::apply ( Rectanglef& node )
//{
//    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
//        (*pp)->accept ( *this );
//}
//
//inline void SomBuilder::apply ( TransformNode& node )
//{
//    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
//        (*pp)->accept ( *this );
//}
//
//inline void SomBuilder::apply ( ArrayNode& node )
//{
//    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
//        (*pp)->accept ( *this );
//}
//
//

#endif
