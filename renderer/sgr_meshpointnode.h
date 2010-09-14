#ifndef _MESHPOINTNODE_H_
#define _MESHPOINTNODE_H_

#include "sgr_drawablenode.h"
namespace SGR
{

class SGR_DLL MeshPointNode : public DrawableNode
{
public:
    MeshPointNode () : _idx(-1), _pointSize(0) {}
    MeshPointNode ( const MeshPointNode& rhs ) : DrawableNode(rhs) { _idx = rhs._idx; _pointSize = rhs._pointSize; }

    virtual void accept ( NodeVisitor& pvisitor ) const { pvisitor.apply ( *this ); }
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~MeshPointNode () {}


    virtual void updateBBox( const mat4f& mat=mat4f() )
    {
        ParentFinder<MeshNode3f> finder(this);
        if ( finder.target() )
        {
            MeshNode3f& meshnode = *finder.target();
            
            float delta = _pointSize/2.f;
            vec4f min = vec4f ( meshnode[_idx].xyz() - delta );
            vec4f max = vec4f ( meshnode[_idx].xyz() + delta );
            min = mat * vec4f ( min );
            max = mat * vec4f ( max );

            _bb.setminmax ( min.xyz(), max.xyz() );
        }

        for ( iterator pp=begin(); pp!=end(); ++pp )
        {
            (*pp)->updateBBox();
            _bb = _bb.unionbox ( (*pp)->getBBox() );
        }
    }

    int coordIdx () { return _idx; }
    void coordIdx ( int idx ) { _idx = idx; }

    float pointSize() { return _pointSize; }
    void pointSize(float size) { _pointSize=size; }

private:
    int _idx;
    float _pointSize;
};
}
//typedef NodeMgr<SceneNode>      SceneNodeMgr;
#endif
