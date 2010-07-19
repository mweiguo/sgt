#include "sgl_includes.h"
#include "arrayexpander.h"
//#include "rectitem.h"
//#include "transformnode.h"
//#include "arraynode.h"
//#include "mat4.h"
//#include "vec2.h"
//#include "vec4.h"
//#include "pickablegroup.h"
//#include "textnode.h"

#include <string>
#include <sstream>
#include <algorithm>


ArrayExpander::ArrayExpander ( SGNode* parent, bool isExport ) : _parent(parent), _curparent(parent), _isExport(isExport) 
{
}

void ArrayExpander::operator () ( ArrayNode& node )
{
    //vector<SGNode*> tmpChildren;
    //copy ( node.begin(), node.end(), back_inserter(tmpChildren) );

    GroupNode* group = new GroupNode();
    if ( _isExport )
        NodeMgr::getInst().addNode ( group );

    _curparent->addChild ( group );
    _curparent = group;

    //_result.reserve ( node.rowCnt() * node.columnCnt() * 8);
    for ( int j=0; j<node.rowCnt(); j++ ) {
        float y = node.getVOffset ( j );
        for ( int k=0; k<node.columnCnt(); k++ ) {
            float x = node.getHOffset ( k );
            // generate new matrix & use this matrix
            TransformNode* trans = new TransformNode();
            if ( _isExport )
                NodeMgr::getInst().addNode ( trans );

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
    LayerNode* layer = new LayerNode ( node );
    if ( _isExport )
        NodeMgr::getInst().addNode ( layer );

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
    RectangleNodef* rc = new RectangleNodef ( node );
    if ( _isExport )
        NodeMgr::getInst().addNode ( rc );

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
    TransformNode* trans = new TransformNode ( node );
    if ( _isExport )
        NodeMgr::getInst().addNode ( trans );

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
    LODNode* lod = new LODNode ( node );
    if ( _isExport )
        NodeMgr::getInst().addNode ( lod );

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
    PickableGroup* pick = new PickableGroup ( node );
    if ( _isExport )
        NodeMgr::getInst().addNode ( pick );

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
    KdTreeNode* kdtreenode = new KdTreeNode ( node );
    if ( _isExport )
        NodeMgr::getInst().addNode ( kdtreenode );

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
    TextNode* textnode = new TextNode ( node );
    if ( _isExport )
        NodeMgr::getInst().addNode ( textnode );

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
    LineNodef* linenode = new LineNodef ( node );
    if ( _isExport )
        NodeMgr::getInst().addNode ( linenode );

    linenode->clear();
    _curparent->addChild ( linenode );

    SGNode* oldparent = _curparent;
    _curparent = linenode;
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    _curparent = oldparent;
}

