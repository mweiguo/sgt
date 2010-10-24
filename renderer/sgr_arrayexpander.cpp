#include "sgr_includes.h"
#include "sgr_arrayexpander.h"
//#include "sgr_rectitem.h"
//#include "sgr_transformnode.h"
//#include "sgr_arraynode.h"
//#include "sgr_mat4.h"
//#include "sgr_vec2.h"
//#include "sgr_vec4.h"
//#include "sgr_pickablegroup.h"
//#include "sgr_textnode.h"

#include <string>
#include <sstream>
#include <algorithm>
//#include "sgr_interface.h"
#include "sgr_seedgen.h"
#include "sgr_polylinenode.h"

namespace SGR
{

ArrayExpander::ArrayExpander ( SGNode* parent, bool isExport ) : _parent(parent), _curparent(parent), _isExport(isExport) 
{
}

void ArrayExpander::operator () ( ArrayNode& node )
{
    //vector<SGNode*> tmpChildren;
    //copy ( node.begin(), node.end(), back_inserter(tmpChildren) );

    int id = SeedGenerator::getInst().minseed();
    GroupNode* group = NodeMgr::getInst().addNode<GroupNode>(id);
//     groupnode_create ( id );
//     GroupNode* group = NodeMgr::getInst().getNodePtr<GroupNode>(id);
    //GroupNode* group = new GroupNode();
    //if ( _isExport )
    //    NodeMgr::getInst().addNode ( group );

    _curparent->addChild ( group );
    _curparent = group;

    //_result.reserve ( node.rowCnt() * node.columnCnt() * 8);
    for ( int j=0; j<node.rowCnt(); j++ ) {
        float y = node.getVOffset ( j );
        for ( int k=0; k<node.columnCnt(); k++ ) {
            float x = node.getHOffset ( k );
            // generate new matrix & use this matrix
            id = SeedGenerator::getInst().minseed();
            TransformNode* trans = NodeMgr::getInst().addNode<TransformNode>(id);
//             transform_create ( id );
//             TransformNode* trans = NodeMgr::getInst().getNodePtr<TransformNode>(id);
            //TransformNode* trans = new TransformNode();
            //if ( _isExport )
            //    NodeMgr::getInst().addNode ( trans );

            trans->setTranslate ( x, y, 0 );
            _curparent->addChild ( trans );
            SGNode* oldparent = _curparent;
            _curparent = trans;

            for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
                (*pp)->accept ( *this );
            _curparent = oldparent;
        }
    }

    //for ( vector<SGNode*>::iterator pp=tmpChildren.begin(); pp!=tmpChildren.end(); ++pp )
    //    (*pp)->setParentNode ( NULL );
}

void ArrayExpander::apply ( SGNode& node )
{
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
}

void ArrayExpander::apply ( LayerNode& node )
{
    int id = SeedGenerator::getInst().minseed();
    LayerNode* layer = NodeMgr::getInst().addNode<LayerNode> ( id, node );
    //LayerNode* layer = new LayerNode ( node );
    //if ( _isExport )
    //    NodeMgr::getInst().addNode ( layer );

    layer->clear();
    _curparent->addChild ( layer );

    SGNode* oldparent = _curparent;
    _curparent = layer;
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    _curparent = oldparent;
}

void ArrayExpander::apply ( RectangleNodef& node )
{
    int id = SeedGenerator::getInst().minseed();
    RectangleNodef* rc = NodeMgr::getInst().addNode<RectangleNodef> ( id, node );
    //RectangleNodef* rc = new RectangleNodef ( node );
    //if ( _isExport )
    //    NodeMgr::getInst().addNode ( rc );

    rc->clear();
    _curparent->addChild ( rc );

    SGNode* oldparent = _curparent;
    _curparent = rc;
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    _curparent = oldparent;
}

void ArrayExpander::apply ( TransformNode& node )
{
    int id = SeedGenerator::getInst().minseed();
    TransformNode* trans = NodeMgr::getInst().addNode<TransformNode> ( id, node );
    //TransformNode* trans = new TransformNode ( node );
    //if ( _isExport )
    //    NodeMgr::getInst().addNode ( trans );

    trans->clear();
    _curparent->addChild ( trans );

    SGNode* oldparent = _curparent;
    _curparent = trans;
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    _curparent = oldparent;
}

void ArrayExpander::apply ( ArrayNode& node )
{
    SGNode* oldparent = _curparent;
    ArrayExpander expander( _curparent );
    expander( node );
    _curparent = oldparent;
}

void ArrayExpander::apply ( LODNode& node )
{
    int id = SeedGenerator::getInst().minseed();
    LODNode* lod = NodeMgr::getInst().addNode<LODNode> ( id, node );
    //LODNode* lod = new LODNode ( node );
    //if ( _isExport )
    //    NodeMgr::getInst().addNode ( lod );

    lod->clear();
    _curparent->addChild ( lod );

    SGNode* oldparent = _curparent;
    _curparent = lod;
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    _curparent = oldparent;
}

void ArrayExpander::apply ( PickableGroup& node )
{
    int id = SeedGenerator::getInst().minseed();
    PickableGroup* pick = NodeMgr::getInst().addNode<PickableGroup> ( id, node );
    //PickableGroup* pick = new PickableGroup ( node );
    //if ( _isExport )
    //    NodeMgr::getInst().addNode ( pick );

    pick->clear();
    _curparent->addChild ( pick );

    SGNode* oldparent = _curparent;
    _curparent = pick;
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    _curparent = oldparent;
}

void ArrayExpander::apply ( KdTreeNode& node )
{
    int id = SeedGenerator::getInst().minseed();
    KdTreeNode* kdtreenode = NodeMgr::getInst().addNode<KdTreeNode> ( id, node );
    //KdTreeNode* kdtreenode = new KdTreeNode ( node );
    //if ( _isExport )
    //    NodeMgr::getInst().addNode ( kdtreenode );

    kdtreenode->clear();
    _curparent->addChild ( kdtreenode );
    _kdtreenodes.push_back ( kdtreenode );

    SGNode* oldparent = _curparent;
    _curparent = kdtreenode;
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    _curparent = oldparent;
}

void ArrayExpander::apply ( TextNode& node )
{
    int id = SeedGenerator::getInst().minseed();
    TextNode* textnode = NodeMgr::getInst().addNode<TextNode> ( id, node );
    //TextNode* textnode = new TextNode ( node );
    //if ( _isExport )
    //    NodeMgr::getInst().addNode ( textnode );

    textnode->clear();
    _curparent->addChild ( textnode );

    SGNode* oldparent = _curparent;
    _curparent = textnode;
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    _curparent = oldparent;
}

void ArrayExpander::apply ( LineNodef& node )
{
    int id = SeedGenerator::getInst().minseed();
    LineNodef* linenode = NodeMgr::getInst().addNode<LineNodef>(id, node );
    //LineNodef* linenode = new LineNodef ( node );
    //if ( _isExport )
    //    NodeMgr::getInst().addNode ( linenode );

    linenode->clear();
    _curparent->addChild ( linenode );

    SGNode* oldparent = _curparent;
    _curparent = linenode;
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    _curparent = oldparent;
}

void ArrayExpander::apply ( PolylineNode2Df& node )
{
    int id = SeedGenerator::getInst().minseed();
    PolylineNode2Df* linenode = NodeMgr::getInst().addNode<PolylineNode2Df>(id, node );

    linenode->clear();
    _curparent->addChild ( linenode );

    SGNode* oldparent = _curparent;
    _curparent = linenode;
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    _curparent = oldparent;
}

void ArrayExpander::apply ( PolyNode2Df& node )
{
    int id = SeedGenerator::getInst().minseed();
    PolyNode2Df* linenode = NodeMgr::getInst().addNode<PolyNode2Df>(id, node );

    linenode->clear();
    _curparent->addChild ( linenode );

    SGNode* oldparent = _curparent;
    _curparent = linenode;
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    _curparent = oldparent;
}

void ArrayExpander::apply ( PointNode& node )
{
    int id = SeedGenerator::getInst().minseed();
    PointNode* pointnode = NodeMgr::getInst().addNode<PointNode>(id, node );

    pointnode->clear();
    _curparent->addChild ( pointnode );

    SGNode* oldparent = _curparent;
    _curparent = pointnode;
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    _curparent = oldparent;
}

void ArrayExpander::apply ( MeshPointNode& node )
{
    int id = SeedGenerator::getInst().minseed();
    MeshPointNode* pointnode = NodeMgr::getInst().addNode<MeshPointNode>(id, node );

    pointnode->clear();
    _curparent->addChild ( pointnode );

    SGNode* oldparent = _curparent;
    _curparent = pointnode;
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    _curparent = oldparent;
}

}

