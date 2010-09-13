#ifndef __COLORNODE_H__
#define __COLORNODE_H__

#include "sgr_gcolor.h"
namespace SGR
{

class SGR_DLL ColorNode : public GColor, public SGNode
{
public:
    ColorNode ( unsigned int color=0x000000ff ) : GColor (color) {}
    ColorNode ( const string& color ) : GColor (color) {}
    ColorNode ( const ColorNode& rhs ) : GColor ( rhs ) {} 

    virtual void accept ( NodeVisitor& pvisitor ) const { pvisitor.apply ( *this ); }
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~ColorNode () {}
};
}
#endif // __COLORNODE_H__
