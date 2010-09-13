#ifndef __SGR_POLYNODE_H__
#define __SGR_POLYNODE_H__

#include "sgr_drawablenode.h"
#include <sgr_polylinenode.h>
//#include "fbcolor.h"
namespace SGR
{

template < class T >
class SGR_DLL PolyNode : public PolylineNode<T>
{
public:
    typedef typename Polyline<T>::iterator pointiterator;
    
    PolyNode () {}
    PolyNode ( const PolyNode<T>& rhs ) : PolylineNode ( rhs ) {} 
    virtual void accept ( NodeVisitor& pvisitor ) const { pvisitor.apply ( *this ); }
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }

    ~PolyNode () {}
};

typedef PolyNode<vec2d>  PolyNode2Dd;
typedef PolyNode<vec2f>  PolyNode2Df;
typedef PolyNode<vec2i>  PolyNode2Di;
typedef PolyNode<vec2s>  PolyNode2Ds;


}
#endif // __SGR_POLYNODE_H__
