#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include "sgr_nodemgr.h"
#include "sgr_transformnode.h"
namespace SGR
{

class SGR_DLL SceneNode : public TransformNode
{
public:
    SceneNode() {_isVisible = true;}
    SceneNode( const SceneNode& rhs ) : TransformNode(rhs) {_isVisible = rhs._isVisible;}
    
    virtual SGNode* clone ()
    {
        return new SceneNode(*this);
    }
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~SceneNode () {}
};
}
//typedef NodeMgr<SceneNode>      SceneNodeMgr;
#endif
