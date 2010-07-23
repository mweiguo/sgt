#ifndef __LINENODE_H__
#define __LINENODE_H__

#include "drawablenode.h"
//#include "nodemgr.h"
#include <vec2.h>
#include <vec4.h>
#include <mat4.h>
#include <line.h>
//#include "fbcolor.h"

template < class T >
class AGEF_EXPORT LineNode : public DrawableNode, public Line<T>
{
public:
    LineNode ( T x1=0, T y1=0, T x2=0, T y2=0 ) { setPoints(x1,y1,x2,y2); }
    LineNode ( const LineNode<T>& rhs ) : DrawableNode ( rhs ), Line<T>(rhs) {} 

    virtual void updateBBox( const mat4f& mat=mat4f() )
    {
        _bb = Line<T>::boundingbox();
        
        vec4f min = mat * vec4f ( _bb.min() );
        vec4f max = mat * vec4f ( _bb.max() );
        _bb.setminmax ( min.xyz(), max.xyz() );

        for ( iterator pp=begin(); pp!=end(); ++pp )
        {
            (*pp)->updateBBox();
            _bb = _bb.unionbox ( (*pp)->getBBox() );
        }
    }
    virtual void accept ( NodeVisitor& pvisitor ) const { pvisitor.apply ( *this ); }
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    ~LineNode () {}

//    void setUseLayerColor ( bool b ) { _color.setUseLayerFgColor(b); };
//    bool isUseLayerColor () const { return _color.isUseLayerFgColor(); }
//    void useLayerColor ()
//    {
//        ParentFinder<LayerNode> finder ( this );
//        if ( finder.target() )
//            _color.useLayerFgColor(finder.target()); 
//    }
//    void setLineColor ( GColor color )
//    {
//        _color.setFgColor(color);
//    }
//    GColor getLineColor () { return _color.getFgColor(); }
//private:
//    FgBgColorComponent _color;
};

typedef LineNode<double>  LineNoded;
typedef LineNode<float>   LineNodef;
typedef LineNode<int>     LineNodei;
typedef LineNode<short>   LineNodes;


#endif // __LINENODE_H__
