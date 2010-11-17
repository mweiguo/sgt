#ifndef __QT_GLRENDER_FUNCTOR_H__
#define __QT_GLRENDER_FUNCTOR_H__

#include "sgr_renderoption.h"
#include "sgr_nodevisitor.h"
#include "sgr_renderlist.h"

namespace SGR
{

class GLRenderOption : public RenderOption
{
public:
    GLRenderOption() {}
    virtual ~GLRenderOption() {}

    float vpXYWH[4];
    SGR::mat4f mvmat, projmat;
};

class GLStateChanger
{
public:
    GLStateChanger ( GLRenderOption* opt, AttrSet* set );
};

class AttrSet;
class GLRenderVisitor : public NodeVisitor
{
public:
    GLRenderVisitor ( RenderOption* opt, RenderList& renderlist );
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
    GLRenderOption* _opt;
};

}

#endif // __QT_GLRENDER_FUNCTOR_H__
