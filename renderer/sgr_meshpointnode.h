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

    
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~MeshPointNode () {}


    virtual void updateBBox( const mat4f& mat=mat4f() );

    int coordIdx () { return _idx; }
    void coordIdx ( int idx ) { _idx = idx; }

    float pointSize() { setBBoxDirty(true); return _pointSize; }
    void pointSize(float size) { _pointSize=size; }

    void setCoord ( const vec3f& coord );
    vec3f& getCoord ();
    vec3f getCoord () const;

private:
    int _idx;
    float _pointSize;
};
}
//typedef NodeMgr<SceneNode>      SceneNodeMgr;
#endif
