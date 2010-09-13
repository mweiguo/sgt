#ifndef _SWITCH_NODE_H_
#define _SWITCH_NODE_H_

#include "sgr_sgnode.h"
#include "sgr_nodevisitor.h"
namespace SGR
{

//class NodeVisitor;
class SGR_DLL SwitchNode : public virtual SGNode
{
public:
    SwitchNode () :_selection(-1) {}
    SwitchNode ( const SwitchNode& rhs ) : _selection(rhs._selection) {}
    virtual void accept ( NodeVisitor& pvisitor ) const { pvisitor.apply ( *this ); }
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~SwitchNode () {}
    void selection ( int idx ) { _selection=idx; }
    int selection () { return _selection; }
private:
    int _selection;
};

//typedef NodeMgr<SwitchNode>     SwitchNodeMgr;
}
#endif
