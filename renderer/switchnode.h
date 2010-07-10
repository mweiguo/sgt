#ifndef _SWITCH_NODE_H_
#define _SWITCH_NODE_H_

#include "sgnode.h"
#include "nodevisitor.h"

//class NodeVisitor;
class AGEF_EXPORT SwitchNode : public virtual SGNode
{
public:
    SwitchNode () {_isVisible = true;}
    SwitchNode ( const SwitchNode& rhs )
    {
	_isVisible = rhs._isVisible;
    }
    bool isVisible() { return _isVisible; }
    void setVisible(bool isVisible) { _isVisible = isVisible; }
    virtual void accept ( NodeVisitor& pvisitor ) const { pvisitor.apply ( *this ); }
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~SwitchNode () {}
protected:
    bool _isVisible;
};

//typedef NodeMgr<SwitchNode>     SwitchNodeMgr;

#endif
