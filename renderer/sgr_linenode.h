#ifndef __LINENODE_H__
#define __LINENODE_H__

#include "sgr_drawablenode.h"
//#include "sgr_nodemgr.h"
#include <sgr_vec2.h>
#include <sgr_vec4.h>
#include <sgr_mat4.h>
#include <sgr_line.h>
//#include "fbcolor.h"
namespace SGR
{

template < class T >
class SGR_DLL LineNode : public DrawableNode, public Line<T>
{
public:
    LineNode ( T x1=0, T y1=0, T x2=0, T y2=0 ) { setPoints(x1,y1,x2,y2); }
    LineNode ( const LineNode<T>& rhs ) : DrawableNode ( rhs ), Line<T>(rhs) {} 

    virtual void updateBBox( const mat4f& mat=mat4f() )
    {
        _bb = Line<T>::boundingbox();
        
        vec4f min = mat * vec4f ( _bb.minvec() );
        vec4f max = mat * vec4f ( _bb.maxvec() );
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

}
#endif // __LINENODE_H__
