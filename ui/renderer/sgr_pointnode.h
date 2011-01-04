#ifndef __SGR_POINTNODE_H__
#define __SGR_POINTNODE_H__

#include "sgr_drawablenode.h"
//#include "sgr_nodemgr.h"
#include <sgr_vec2.h>
#include <sgr_vec4.h>
#include <sgr_mat4.h>
#include <sgr_line.h>
#include <string>

//#include "fbcolor.h"
namespace SGR
{

class SGR_DLL PointNode : public DrawableNode//, public vec3f
{
public:
    PointNode ( float x=0.f, float y=0.f, float z=0.f ) : _pnt(x,y,z), _pointSize(0), _name("") {}
    PointNode ( const PointNode& rhs ) : DrawableNode ( rhs ), _pnt(rhs._pnt), _pointSize(rhs._pointSize), _name(rhs._name) {} 
    virtual SGNode* clone ()
    {
        return new PointNode(*this);
    }

    virtual void updateBBox( const mat4f& mat=mat4f(), bool force=false )
    {
        if ( force || isBBoxDirty()  )
        {
            float delta = _pointSize/2.f;
            vec4f min = vec4f ( _pnt.xyz() - delta );
            vec4f max = vec4f ( _pnt.xyz() + delta );
            min = mat * vec4f ( min );
            max = mat * vec4f ( max );

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
        float delta = _pointSize/2.f;
        vec4f min = vec4f ( _pnt.xyz() - delta );
        vec4f max = vec4f ( _pnt.xyz() + delta );
        min = (*mat) * vec4f ( min );
        max = (*mat) * vec4f ( max );

        bb.setminmax ( min.xyz(), max.xyz() );

        _bb = _bb.unionbox ( bb );
    }
    
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    ~PointNode () {}

    float pointSize() { return _pointSize; }
    void pointSize(float size) 
    {
        _pointSize=size; 
        setBBoxDirty ();
        //setParentBBoxDirty ();
    }

    vec3f xyz() const { return _pnt; }
    void xyz ( float v0, float v1, float v2 )
    {
        _pnt.xyz ( v0, v1, v2 );
        setBBoxDirty ();
        //setParentBBoxDirty ( true );
    }
    void xy ( float v0, float v1 )
    {
        _pnt.xy ( v0, v1 );
        setBBoxDirty ();
        //setParentBBoxDirty ( true );
    }

    float x() const { return _pnt.x(); }
    float y() const { return _pnt.y(); }
    float z() const { return _pnt.z(); }

    const std::string& name() const { return _name; }
    std::string name() { return _name; }
    void name ( const std::string& nm ) { _name = nm; }

private:
    vec3f _pnt;
    float _pointSize;
    std::string _name;
};

}
#endif // __LINENODE_H__
