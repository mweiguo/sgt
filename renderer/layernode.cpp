#include "layernode.h"

LayerNode::LayerNode ( const string& name) : GroupNode(name), _color(0)
{
    LayerMgr::getInst().append ( this );
}

LayerNode::LayerNode ( const LayerNode& rhs ) : GroupNode ( rhs ), _color(rhs._color)
{
    LayerMgr::getInst().append ( this );
}


LayerNode::~LayerNode ()
{
    LayerMgr::getInst().remove ( this );
}

//////////////////////////////////////////////////////////////////////

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

