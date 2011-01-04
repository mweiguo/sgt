
#include "sgr_pickablegroup.h"
namespace SGR
{

PickableGroup::PickableGroup( const string& name ) : GroupNode(name)
{
}

PickableGroup::PickableGroup( const PickableGroup& rhs ) : GroupNode(rhs)
{
}

SGNode* PickableGroup::clone ()
{
    return new PickableGroup(*this);
}

}
