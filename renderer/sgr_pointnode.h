#ifndef __SGR_POINTNODE_H__
#define __SGR_POINTNODE_H__

#include "sgr_drawablenode.h"
//#include "sgr_nodemgr.h"
#include <sgr_vec2.h>
#include <sgr_vec4.h>
#include <sgr_mat4.h>
#include <sgr_line.h>
//#include "fbcolor.h"
namespace SGR
{

class SGR_DLL PointNode : public DrawableNode, public vec3f
{
public:
    PointNode ( float x=0.f, float y=0.f, float z=0.f ) : vec3f(x,y,z), _pointSize(0) {}
    PointNode ( const PointNode& rhs ) : DrawableNode ( rhs ), vec3f(rhs), _pointSize(rhs._pointSize) {} 

    virtual void updateBBox( const mat4f& mat=mat4f() )
    {
        float delta = _pointSize/2.f;
        vec4f min = vec4f ( xyz() - delta );
        vec4f max = vec4f ( xyz() + delta );
        min = mat * vec4f ( min );
        max = mat * vec4f ( max );

        _bb.setminmax ( min.xyz(), max.xyz() );

        for ( iterator pp=begin(); pp!=end(); ++pp )
        {
            (*pp)->updateBBox();
            _bb = _bb.unionbox ( (*pp)->getBBox() );
        }
    }
    virtual void accept ( NodeVisitor& pvisitor ) const { pvisitor.apply ( *this ); }
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    ~PointNode () {}

    float pointSize() { return _pointSize; }
    void pointSize(float size) { _pointSize=size; }

private:
    float _pointSize;
};

}
#endif // __LINENODE_H__
