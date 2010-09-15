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

class SGR_DLL PointNode : public DrawableNode//, public vec3f
{
public:
    PointNode ( float x=0.f, float y=0.f, float z=0.f ) : _pnt(x,y,z), _pointSize(0) {}
    PointNode ( const PointNode& rhs ) : DrawableNode ( rhs ), _pnt(rhs._pnt), _pointSize(rhs._pointSize) {} 

    virtual void updateBBox( const mat4f& mat=mat4f() )
    {
        float delta = _pointSize/2.f;
        vec4f min = vec4f ( _pnt.xyz() - delta );
        vec4f max = vec4f ( _pnt.xyz() + delta );
        min = mat * vec4f ( min );
        max = mat * vec4f ( max );

        _bb.setminmax ( min.xyz(), max.xyz() );

        for ( iterator pp=begin(); pp!=end(); ++pp )
        {
            (*pp)->updateBBox();
            _bb = _bb.unionbox ( (*pp)->getBBox() );
        }

        setBBoxDirty ( false );
    }
    
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    ~PointNode () {}

    float pointSize() { return _pointSize; }
    void pointSize(float size) 
    {
        _pointSize=size; 
        setBBoxDirty ( true );
        setParentBBoxDirty ( true );
    }

    vec3f xyz() const { return _pnt; }
    void xyz ( float v0, float v1, float v2 )
    {
        _pnt.xyz ( v0, v1, v2 );
        setBBoxDirty ( true );
        setParentBBoxDirty ( true );
    }
    void xy ( float v0, float v1 )
    {
        _pnt.xy ( v0, v1 );
        setBBoxDirty ( true );
        setParentBBoxDirty ( true );
    }

    float x() const { return _pnt.x(); }
    float y() const { return _pnt.y(); }
    float z() const { return _pnt.z(); }


private:
    vec3f _pnt;
    float _pointSize;
};

}
#endif // __LINENODE_H__
