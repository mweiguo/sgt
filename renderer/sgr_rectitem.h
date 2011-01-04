#ifndef __RECTANGLENODE_H__
#define __RECTANGLENODE_H__

#include "sgr_drawablenode.h"
#include "sgr_nodemgr.h"
#include <sgr_vec2.h>
#include <sgr_vec4.h>
#include <sgr_mat4.h>
#include <sgr_rectangle.h>
//#include "fbcolor.h"
//#include "sgr_parentfinder.h"
namespace SGR
{

template < class T >
class SGR_DLL RectangleNode : public DrawableNode//, public Rectangle<T>
{
public:
    RectangleNode ( T x=0, T y=0, T w=0, T h=0 )
    {
        _rect.setRect(x,y,w,h); 
    }

    RectangleNode ( const RectangleNode& rhs ) : DrawableNode ( rhs ), _rect(rhs._rect) {} 

    virtual SGNode* clone ()
    {
        return new RectangleNode(*this);
    }

    virtual void updateBBox( const mat4f& mat=mat4f(), bool force=false )
    {
        if ( force || isBBoxDirty()  )
        {
            _bb = _rect.boundingbox();

            vec4f min = mat * vec4f ( _bb.minvec() );
            vec4f max = mat * vec4f ( _bb.maxvec() );
            _bb.setminmax ( min.xyz(), max.xyz() );

            for ( iterator pp=begin(); pp!=end(); ++pp )
            {
                if ( force || (*pp)->isBBoxDirty () )
                    (*pp)->updateBBox(mat, force);
                _bb = _bb.unionbox ( (*pp)->getBBox() );
            }

            _isBBoxDirty = false;
        }
    }
    
    virtual void computeBBox( const mat4f* mat=0 ) const
    {
        if ( false == _isBBoxDirty )
            return;

        mat4f tmat;
        if ( 0 == mat )
        {
            mat = &tmat;
            tmat = getParentTranMatrix ();
        }
        DrawableNode::computeBBox ( mat );

        BBox bb;
        bb = _rect.boundingbox();

        vec4f min = (*mat) * vec4f ( bb.minvec() );
        vec4f max = (*mat) * vec4f ( bb.maxvec() );
        bb.setminmax ( min.xyz(), max.xyz() );

        _bb = _bb.unionbox ( bb );
    }

    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }

    inline T x() const { return _rect.x(); }
    inline T y() const { return _rect.y(); }
    inline T w() const { return _rect.w(); }
    inline T h() const { return _rect.h(); }
    inline void w( T v )
    {
        _rect.w ( v );
        setBBoxDirty (); 
        //setParentBBoxDirty ( true );
    }
    inline void h( T v ) 
    {
        _rect.h ( v );
        setBBoxDirty (); 
        //setParentBBoxDirty ( true );
    }

    void setRect ( T x, T y, T w, T h )
    {
        _rect.setRect ( x, y, w, h );
        setBBoxDirty (); 
        //setParentBBoxDirty ( true );
    }

    inline vec2<T> lb() const { return _rect.lb(); }
    inline vec2<T> rt() const { return _rect.rt(); }

    inline void dimention( T w, T h )
    {
        _rect.dimention(w, h);
        setBBoxDirty (); 
        //setParentBBoxDirty ( true );
    }
    inline vec2<T> dimention() const { return _rect.dimention(); }

    ~RectangleNode () {}
private:
    Rectangle<T> _rect;
};

typedef RectangleNode<double>  RectangleNoded;
typedef RectangleNode<float>   RectangleNodef;
typedef RectangleNode<int>     RectangleNodei;
typedef RectangleNode<short>   RectangleNodes;

}
#endif
