#ifndef _QTRENDER_FUNCTOR_H_
#define _QTRENDER_FUNCTOR_H_
 
#include <QPainter>

#include "sgr_rectitem.h"
#include "sgr_fontnode.h"
#include "sgr_textnode.h"
#include "sgr_mat4.h"
#include "sgr_vec2.h"
#include "sgr_vec4.h"
#include "sgr_gcolor.h"
#include "sgr_linenode.h"
#include "sgr_nodes.h"
#include "sgr_global.h"
#include "sgr_parentfinder.h"
#include "sgr_fontmetric.h"
#include "sgr_circlenode.h"
#include "sgr_imagenode.h"
#include <utl_logger.h>
#include "sgr_rendercontext.h"

namespace SGR
{

// struct SGR_DLL RenderOption
// {
//     BBox bb;
//     QPainter* painter;
//     //mat4f* mvmatrix;
//     // matrix = viewpmapping * model-view
//     mat4f matrix, reverse_mvpw;
//     float scale;
//     RenderOption() : painter(0) {}
// };

#include "sgr_attrset.h"
class SGR_DLL QtStateChanger
{
public:
    QtStateChanger ( RenderOption* opt, const AttrSet* set ) : _opt(opt), _isPenValid(false), _isBrushValid(false), _isFontValid(false)
    {
        if ( NULL == set )
            return;
        
        // set foreground color ==>  QPen
        const GColor* color = set->getFgColor();
        if ( color )
        {
            _isPenValid = true;
            QPen pen ( QColor(color->r(),color->g(),color->b(),color->a()) );
            pen.setWidth ( set->getLineWidth() );
            _oldpen = _opt->painter->pen();
            _opt->painter->setPen ( pen );
        }
        
        // set background color ==>  QBrush
        color = set->getBgColor();
        if ( color )
        {
            _isBrushValid = true;
            QBrush brush ( QColor(color->r(),color->g(),color->b(),color->a()) );
            _oldbrush = _opt->painter->brush();
            _opt->painter->setBrush ( brush );
        }

        // set background color ==>  QBrush
        const FontNode* fontnode = set->getFont();
        if ( fontnode )
        {
            _isFontValid = true;
            QFont font ( fontnode->family().c_str(), fontnode->pointSize() );
            _oldfont = _opt->painter->font();
            _opt->painter->setFont ( font );
        }
    }

    ~QtStateChanger ()
    {
        //if ( _isPenValid )
        //    _opt->painter->setPen ( _oldpen );
        //if ( _isBrushValid )
        //    _opt->painter->setBrush ( _oldbrush );
        //if ( _isFontValid )
        //    _opt->painter->setFont ( _oldfont );
    }
private:
    RenderOption* _opt;

    QPen _oldpen;
    QBrush _oldbrush;
    QFont _oldfont;

    bool _isPenValid;
    bool _isBrushValid;
    bool _isFontValid;
};



class SGR_DLL QtRenderVisitor : public ChildVisitor
{
public:
    QtRenderVisitor ( RenderOption* opt );
    virtual ~QtRenderVisitor();

    virtual void apply ( RectangleNodef& rect );
    virtual void apply ( TextNode& text );
    virtual void apply ( LineNodef& /*node*/ );
    virtual void apply ( MeshLineNode& node );
    virtual void apply ( PolylineNode2Df& node );
    virtual void apply ( PolyNode2Df& node );
    virtual void apply ( PointNode& node );
    virtual void apply ( MeshPointNode& node );
    virtual void apply ( CircleNode& node );
    virtual void apply ( ImageNode& node );
private:
    RenderOption* _opt;
};

}
#endif
