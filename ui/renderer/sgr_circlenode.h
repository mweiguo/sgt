#ifndef __CIRCLENODE_H__
#define __CIRCLENODE_H__

#include "sgr_drawablenode.h"
#include "sgr_nodemgr.h"
#include <sgr_vec2.h>
#include <sgr_vec4.h>
#include <sgr_mat4.h>
#include <sgr_rectangle.h>
//#include "fbcolor.h"
//#include "sgr_parentfinder.h"
#include <sgr_circle.h>

namespace SGR
{


class SGR_DLL CircleNode : public DrawableNode
{
public:
    CircleNode ( float x=0, float y=0, float r=0 );

    CircleNode ( const CircleNode& rhs );
    virtual ~CircleNode ();

    virtual SGNode* clone ();
    virtual void updateBBox( const mat4f& mat=mat4f(), bool force=false );
    // if mat == 0, mat should be computed inside function.
    virtual void computeBBox( const mat4f* mat=0 ) const;
    
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }

    inline vec3f getCenter () const { return _circle.getCenter(); }
    inline float getRadius() const { return _circle.getRadius(); }

    void setCenter ( float x, float y, float z=0 );
    void setRadius ( float r );

private:
    Circlef _circle;
};

}
#endif // __CIRCLENODE_H__
