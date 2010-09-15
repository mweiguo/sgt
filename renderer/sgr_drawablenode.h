#ifndef _DRAWABLE_NODE_H_
#define _DRAWABLE_NODE_H_

#include "sgr_sgnode.h"
#include "sgr_bbox.h"
#include "sgr_attrset.h"
namespace SGR
{

class AttrSet;
class SGR_DLL DrawableNode : public SGNode
{
    //public:
    //  virtual bool acceptTraversal ( NodeVisitor* pNodeVisitor );
    //  virtual void updateRenderList ( RenderItemContainer& output );
    //  virtual void render (QPainter& painter) = 0;
public:
    DrawableNode();
    DrawableNode( const DrawableNode& rhs );
    virtual ~DrawableNode();

    
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }

    //void setColor ( unsigned int color );
    //unsigned int getColor ();
    //virtual bool getLayerColor ( unsigned int& color );
    void setVisible ( bool isVisible ) { _bRender = isVisible; }
    bool isVisible () { return _bRender; }

    void setAttrSet ( AttrSet* pAttrSet ) 
    { 
        if ( _pAttrSet == pAttrSet )
            return;

        if ( _pAttrSet )
            _pAttrSet->decRef();

        _pAttrSet = pAttrSet; 
        if ( pAttrSet )
            pAttrSet->incRef(); 
    }
    AttrSet* getAttrSet () { return _pAttrSet; }

protected:
    bool _bRender;
    AttrSet* _pAttrSet;
    //int _color;
};

}
#endif
