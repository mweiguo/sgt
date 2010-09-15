#ifndef _PICKABLE_GROUP_H_
#define _PICKABLE_GROUP_H_

#include "sgr_groupnode.h"
namespace SGR
{

class SGR_DLL PickableGroup : public GroupNode
{
public:
    PickableGroup( const string& name="" );
    PickableGroup( const PickableGroup& rhs );
    
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~PickableGroup () {}
};

//typedef NodeMgr<PickableGroup>  PickableGroupMgr;
}
#endif
