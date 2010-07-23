#ifndef __RECTANGLENODE_H__
#define __RECTANGLENODE_H__

#include "drawablenode.h"
#include "nodemgr.h"
#include <vec2.h>
#include <vec4.h>
#include <mat4.h>
#include <rectangle.h>
//#include "fbcolor.h"
#include "parentfinder.h"

template < class T >
class AGEF_EXPORT RectangleNode : public DrawableNode, public Rectangle<T>
{
public:
    RectangleNode ( T x=0, T y=0, T w=0, T h=0 )
    {
        setRect(x,y,w,h); 
    }

    RectangleNode ( const RectangleNode& rhs ) : DrawableNode ( rhs ), Rectangle<T>(rhs) {} 

    virtual void updateBBox( const mat4f& mat=mat4f() )
    {
        _bb = Rectangle<T>::boundingbox();
        
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

//    void setUseLayerFgColor ( bool b ) { _color.setUseLayerFgColor(b); };
//    void setUseLayerBgColor ( bool b ) { _color.setUseLayerBgColor(b); }
//    bool isUseLayerFgColor () const { return _color.isUseLayerFgColor(); }
//    bool isUseLayerBgColor () const { return _color.isUseLayerBgColor(); }
//
//    void useLayerFillColor ()
//    {
//        ParentFinder<LayerNode> finder ( this );
//        if ( finder.target() )
//            _color.useLayerBgColor ( finder.target() );
//    }
//    void setFillColor ( GColor color ) {_color.setBgColor(color); }
//    GColor getFillColor () const { return _color.getBgColor(); }
//    void useLayerBorderColor ()
//    {
//        ParentFinder<LayerNode> finder ( this );
//        if ( finder.target() )
//            _color.useLayerFgColor(finder.target()); 
//    }
//    void setBorderColor ( GColor color )
//    {
//        _color.setFgColor(color);
//    }
//    GColor getBorderColor () { return _color.getFgColor(); }
////////////////////
//    void setFgColor (GColor color);
//    void setBgColor (GColor color);
//    GColor getFgColor ();
//    GColor getBgColor ();
////////////////////



    ~RectangleNode () {}
//private:
//    FgBgColorComponent _color;
};

typedef RectangleNode<double>  RectangleNoded;
typedef RectangleNode<float>   RectangleNodef;
typedef RectangleNode<int>     RectangleNodei;
typedef RectangleNode<short>   RectangleNodes;


#endif
