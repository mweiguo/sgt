#ifndef __POLYLINENODE_H__
#define __POLYLINENODE_H__

#include "sgr_drawablenode.h"

#include <sgr_mat4.h>
#include <sgr_polyline.h>
//#include "fbcolor.h"
namespace SGR
{

template < class T >
class SGR_DLL PolylineNode : public DrawableNode
{
public:
    typedef typename Polyline<T>::iterator pointiterator;
    
    PolylineNode () {}
    PolylineNode ( const PolylineNode<T>& rhs ) : DrawableNode ( rhs ), _polyline(rhs._polyline) {} 

    virtual void updateBBox( const mat4f& mat=mat4f() )
    {
        _bb = _polyline.boundingbox();
        
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

    // points interface
    void addPoint ( const T& t ) { _polyline.push_back ( t ); }
    void removePoint ( const T& t ) { _polyline.remove ( t ); }
    template <class InputIterator>
    void assignpoints ( InputIterator first, InputIterator last ) { _polyline.assign ( first, last); }

    pointiterator pointbegin() { return _polyline.begin(); }
    pointiterator pointend() { return _polyline.end(); }

    T pointfront () { return _polyline.front(); }
    T pointback () { return _polyline.back(); }

    int pointssize() { return _polyline.size(); }
    bool pointsempty () { return _polyline.empty(); }

    ~PolylineNode () {}
private:
    Polyline<T> _polyline;
    
};

typedef PolylineNode<vec2d>  PolylineNode2Dd;
typedef PolylineNode<vec2f>  PolylineNode2Df;
typedef PolylineNode<vec2i>  PolylineNode2Di;
typedef PolylineNode<vec2s>  PolylineNode2Ds;

typedef PolylineNode<vec3d>  PolylineNode3Dd;
typedef PolylineNode<vec3f>  PolylineNode3Df;
typedef PolylineNode<vec3i>  PolylineNode3Di;
typedef PolylineNode<vec3s>  PolylineNode3Ds;

}
#endif // __POLYLINENODE_H__
