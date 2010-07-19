#ifndef _DRAWABLE_NODE_H_
#define _DRAWABLE_NODE_H_

#include "sgnode.h"
#include "bbox.h"
#include "attrset.h"

class AttrSet;
class AGEF_EXPORT DrawableNode : public SGNode
{
    //public:
    //  virtual bool acceptTraversal ( NodeVisitor* pNodeVisitor );
    //  virtual void updateRenderList ( RenderItemContainer& output );
    //  virtual void render (QPainter& painter) = 0;
public:
    DrawableNode();
    DrawableNode( const DrawableNode& rhs );
    virtual ~DrawableNode();

    virtual void accept ( NodeVisitor& pvisitor ) const { pvisitor.apply ( *this ); }
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

#endif
