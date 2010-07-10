#ifndef _PICKABLE_GROUP_H_
#define _PICKABLE_GROUP_H_

#include "groupnode.h"

class AGEF_EXPORT PickableGroup : public GroupNode
{
public:
    PickableGroup( const string& name="" );
    PickableGroup( const PickableGroup& rhs );
    virtual void accept ( NodeVisitor& pvisitor ) const { pvisitor.apply ( *this ); }
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~PickableGroup () {}
};

//typedef NodeMgr<PickableGroup>  PickableGroupMgr;

#endif
