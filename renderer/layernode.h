#ifndef _LAYER_NODE_H_
#define _LAYER_NODE_H_

#include "groupnode.h"

//#include "lodnode.h"
//#include "colortable.h"
#include <colorconv.h>
#include <gcolor.h>

class AGEF_EXPORT LayerNode : public GroupNode
{
public:
    LayerNode ( const string& name="default" );
    LayerNode ( const LayerNode& rhs );
    virtual ~LayerNode ();

    void setFgColor ( GColor color ) { _fgcolor = color; }
    GColor getFgColor () { return _fgcolor; }
    GColor* getFgColorPtr () { return &_fgcolor; }

    void setBgColor ( GColor color ) { _bgcolor = color; }
    GColor getBgColor () { return _bgcolor; }
    GColor* getBgColorPtr () { return &_bgcolor; }
    //LODNode* lod() { return dynamic_cast<LODNode*>(*begin()); }
    virtual void accept ( NodeVisitor& pvisitor ) const { pvisitor.apply ( *this ); }
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
private:
    GColor _fgcolor, _bgcolor;
};

class LayerMgr
{
public:
    static LayerMgr& getInst () 
    {
        static LayerMgr inst;
        return inst;
    }
    void append ( LayerNode* layer ) { _layers.push_back ( layer ); }
    void remove ( LayerNode* layer ) { _layers.remove ( layer ); }
    void incr ( LayerNode* layer, int step=1 );
    int indexof ( LayerNode* layer );
    void clear() { _layers.clear(); }
private:
    list<LayerNode*> _layers;
    LayerMgr () {}
};


#endif
