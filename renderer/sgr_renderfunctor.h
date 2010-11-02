#ifndef _QTRENDER_FUNCTOR_H_
#define _QTRENDER_FUNCTOR_H_
 
#include "sgr_renderoption.h"
#include "sgr_renderlist.h"
#include "sgr_nodevisitor.h"
#include <QPainter>

namespace SGR
{

class QRenderOption : public RenderOption
{
public:
    QPainter* painter;
    QRenderOption();
    virtual ~QRenderOption();
};

class AttrSet;
class SGR_DLL QtStateChanger
{
public:
    QtStateChanger ( QRenderOption* opt, AttrSet* set );

    ~QtStateChanger ();
private:
    QRenderOption* _opt;

    QPen _oldpen;
    QBrush _oldbrush;
    QFont _oldfont;

    bool _isPenValid;
    bool _isBrushValid;
    bool _isFontValid;
};


class SGR_DLL QtRenderVisitor : public NodeVisitor
{
public:
    QtRenderVisitor ( RenderList& renderlist, RenderOption* opt );
    virtual void apply ( RectangleNodef& rect );
    virtual void apply ( TextNode& text );
    virtual void apply ( LineNodef& /*node*/ );
    virtual void apply ( MeshLineNode& node );
    virtual void apply ( PolylineNode2Df& node );
    virtual void apply ( PolyNode2Df& node );
    virtual void apply ( PointNode& node );
    virtual void apply ( MeshPointNode& node );
    //virtual void apply ( FontNode& font );
private:
    QRenderOption* _opt;
};


}
#endif
