#include "sgr_layernode.h"
#include "sgr_attrset.h"
#include "sgr_fontnode.h"
#include "sgr_layerchangger.h"

namespace SGR
{

LayerNode::LayerNode ( const string& name) : GroupNode(name), _fgcolor(0x0ffffffff), _bgcolor(0x000000ff), 
                                           _attrset(0)//, _renderOrder(-1)
{
    LayerMgr::getInst().append ( this );
    _fontnode = new FontNode ();

    _attrset = new AttrSet (/*order*/);
    _attrset->setFont ( _fontnode );
    _attrset->setBgColor ( &_bgcolor );
    _attrset->setFgColor ( &_fgcolor );

//     if ( _attrset )
//         _attrset->setFont ( _fontnode );

}

LayerNode::LayerNode ( const LayerNode& rhs ) : GroupNode ( rhs ), _fgcolor(rhs._fgcolor), _bgcolor(rhs._bgcolor), 
                                              _fontnode(rhs._fontnode)//, _renderOrder(rhs._renderOrder)
{
    *_attrset = *(rhs._attrset);
    LayerMgr::getInst().append ( this );
}

void LayerNode::setFgColor ( GColor color )
{
    _fgcolor = color; 
//     if ( _attrset )
    _attrset->setFgColor ( &_fgcolor );
}

void LayerNode::setBgColor ( GColor color )
{
    _bgcolor = color; 
//     if ( _attrset )
    _attrset->setBgColor ( &_bgcolor );
}

LayerNode::~LayerNode ()
{
    LayerMgr::getInst().remove ( this );
}

void LayerNode::setFont ( const string& name, const string& f, int ps )
{
    _fontnode->family ( f );
    _fontnode->pointSize ( ps );
    _fontnode->defName ( name );
}

// void LayerNode::setRenderOrder ( int order )
// {
//     _renderOrder = order; 
//     if ( NULL == _attrset )
//     {
//         _attrset = new AttrSet (order);
//         _attrset->setFont ( _fontnode );
//         _attrset->setBgColor ( &_bgcolor );
//         _attrset->setFgColor ( &_fgcolor );
//     } 
//     else
//         _attrset->setRenderOrder ( order );
// }

void LayerNode::addChild ( SGNode* pNode )
{
    GroupNode::addChild ( pNode );
    // if pNode is topnode, then 
    OnAddNodeToLayer onAddNodeToLayer;
    onAddNodeToLayer.doAction ( pNode, this );
}

void LayerNode::removeChild ( SGNode* pNode ) 
{
    GroupNode::removeChild ( pNode );
    OnRemoveNodeFromLayer onRemoveNodeFromLayer;
    onRemoveNodeFromLayer.doAction ( this, pNode );
    // same attrset

    // same part of drawable's attributes
}

//////////////////////////////////////////////////////////////////////
void LayerMgr::append ( LayerNode* layer )
{
    _layers.push_back ( layer ); 
    //layer->setRenderOrder ( _layers.size() -1 );
}

void LayerMgr::remove ( LayerNode* layer )
{
    _layers.remove ( layer ); 
    //layer->setRenderOrder ( -1 );
}


void LayerMgr::incr ( LayerNode* layer, int step ) 
{
    list<LayerNode*>::iterator pos = find ( _layers.begin(), _layers.end(), layer );
    ++pos;
    while ( step-- != 0 && pos!=_layers.end() )
        pos++;

    _layers.remove ( layer );
    _layers.insert ( pos, layer );
}

int LayerMgr::indexof ( LayerNode* layer )
{
    int idx = -1;
    list<LayerNode*>::iterator pp, end=_layers.end();
    
    for ( pp=_layers.begin(); pp!=end; ++pp )
    {
        ++idx;
        if ( (*pp) == layer )
            return idx;
    }
    return -1;
}
}
