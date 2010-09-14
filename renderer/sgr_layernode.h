#ifndef _LAYER_NODE_H_
#define _LAYER_NODE_H_

#include "sgr_groupnode.h"

//#include "sgr_lodnode.h"
//#include "colortable.h"
#include <sgr_colorconv.h>
#include <sgr_gcolor.h>
namespace SGR
{

class SGR_DLL LayerNode : public GroupNode
{
public:
    LayerNode ( const string& name="default" );
    LayerNode ( const LayerNode& rhs );
    virtual ~LayerNode ();

    void setFgColor ( GColor color );
    GColor getFgColor () { return _fgcolor; }
    GColor* getFgColorPtr () { return &_fgcolor; }

    void setBgColor ( GColor color );
    GColor getBgColor () { return _bgcolor; }
    GColor* getBgColorPtr () { return &_bgcolor; }

    FontNode* getFontNode () { return _fontnode; }

    void setRenderOrder ( int order );
    int getRenderOrder () { return _renderOrder; }

    AttrSet* getAttrSet () { return _attrset; }

    virtual void addChild ( SGNode* pNode );
    virtual void removeChild ( SGNode* pNode );
    virtual void accept ( NodeVisitor& pvisitor ) const { pvisitor.apply ( *this ); }
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
private:
    GColor _fgcolor, _bgcolor;
    AttrSet* _attrset;
    FontNode* _fontnode;
    int _renderOrder;
};

class LayerMgr
{
public:
    static LayerMgr& getInst () 
    {
        static LayerMgr inst;
        return inst;
    }
    void append ( LayerNode* layer );
    void remove ( LayerNode* layer );
    void incr ( LayerNode* layer, int step=1 );
    int indexof ( LayerNode* layer );
    void clear() { _layers.clear(); }
private:
    list<LayerNode*> _layers;
    LayerMgr () {}
};

}
#endif
