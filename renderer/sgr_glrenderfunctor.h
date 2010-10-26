#ifndef __QT_GLRENDER_FUNCTOR_H__
#define __QT_GLRENDER_FUNCTOR_H__

#include "sgr_nodevisitor.h"
namespace SGR
{

class GLRenderVisitor : public NodeVisitor
{
public:
    GLRenderVisitor ();
    virtual void apply ( RectangleNodef& rect );
    virtual void apply ( TextNode& text );
    virtual void apply ( LineNodef& /*node*/ );
    virtual void apply ( MeshLineNode& node );
    virtual void apply ( PolylineNode2Df& node );
    virtual void apply ( PolyNode2Df& node );
    virtual void apply ( PointNode& node );
    virtual void apply ( MeshPointNode& node );
    virtual void apply ( MeshNode3f& node );
    //virtual void apply ( FontNode& font );
private:

};

}

#endif // __QT_GLRENDER_FUNCTOR_H__
