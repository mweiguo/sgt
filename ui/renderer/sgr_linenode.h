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
class SGR_DLL LineNode : public DrawableNode
{
public:
    LineNode ( T x1=0, T y1=0, T x2=0, T y2=0 ) { setPoints(x1,y1,x2,y2); }
    LineNode ( const LineNode<T>& rhs ) : DrawableNode ( rhs ), _line(rhs._line) {} 
    virtual SGNode* clone ()
    {
        return new LineNode(*this);
    }
    virtual void updateBBox( const mat4f& mat=mat4f(), bool force=false )
    {
        if ( force || isBBoxDirty()  )
        {
            _bb = _line.boundingbox();

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
        bb = _line.boundingbox();

        vec4f min = (*mat) * vec4f ( bb.minvec() );
        vec4f max = (*mat) * vec4f ( bb.maxvec() );
        bb.setminmax ( min.xyz(), max.xyz() );

        _bb = _bb.unionbox ( bb );
    }
    
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    ~LineNode () {}


    void setPoints ( T x1, T y1, T x2, T y2 )
    {
        _line.setPoints(x1, y1, x2, y2); 
        setBBoxDirty ();
        //setParentBBoxDirty ( true );
    }

    inline vec2<T> point1() { return _line.point1(); }
    inline vec2<T> point2() { return _line.point2(); }
    inline void point1( T x, T y )
    {
        _line.point1(x, y); 
        setBBoxDirty ();
        //setParentBBoxDirty ( true );
    }
    inline void point2( T x, T y ) 
    { 
        _line.point2(x, y); 
        setBBoxDirty ();
        //setParentBBoxDirty ( true );
    }

    inline T x1() const { return _line.x1(); }
    inline T y1() const { return _line.y1(); }
    inline T x2() const { return _line.x2(); }
    inline T y2() const { return _line.y2(); }
    inline void x1( T v ) 
    { 
        _line.x1 (v); 
        setBBoxDirty ();
        //setParentBBoxDirty ( true );
    }
    inline void y1( T v ) 
    { 
        _line.y1 (v); 
        setBBoxDirty ();
        //setParentBBoxDirty ( true );
    }
    inline void x2( T v ) 
    { 
        _line.x2 (v); 
        setBBoxDirty ();
        //setParentBBoxDirty ( true );
    }
    inline void y2( T v ) 
    { 
        _line.y2 (v); 
        setBBoxDirty ();
        //setParentBBoxDirty ( true );
    }


private:
    Line<T> _line;
};

typedef LineNode<double>  LineNoded;
typedef LineNode<float>   LineNodef;
typedef LineNode<int>     LineNodei;
typedef LineNode<short>   LineNodes;

}
#endif // __LINENODE_H__
