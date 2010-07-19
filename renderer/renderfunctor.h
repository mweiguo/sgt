#ifndef _QTRENDER_FUNCTOR_H_
#define _QTRENDER_FUNCTOR_H_

#include <QPainter>

#include "rectitem.h"
#include "fontnode.h"
#include "textnode.h"
#include "mat4.h"
#include "vec2.h"
#include "vec4.h"
#include "linenode.h"
#include "agef_global.h"

struct AGEF_EXPORT RenderOption
{
    BBox bb;
    QPainter* painter;
    //mat4f* mvmatrix;
    // matrix = viewpmapping * model-view
    mat4f matrix;
    float scale;
    RenderOption() : painter(0) {}
};

#include "attrset.h"
class AGEF_EXPORT QtStateChanger
{
public:
    QtStateChanger ( RenderOption* opt, AttrSet* set ) : _opt(opt), _isPenValid(false), _isBrushValid(false)
    {
        if ( NULL == set )
            return;
        
        // set foreground color ==>  QPen
        GColor* color = set->getFgColor();
        if ( color )
        {
            _isPenValid = true;
            QPen pen ( QColor(color->r(),color->g(),color->b(),color->a()) );
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

    }

    ~QtStateChanger ()
    {
        if ( _isPenValid )
            _opt->painter->setPen ( _oldpen );
        if ( _isBrushValid );
        _opt->painter->setBrush ( _oldbrush );
    }
private:
    RenderOption* _opt;

    QPen _oldpen;
    QBrush _oldbrush;

    bool _isPenValid;
    bool _isBrushValid;
};



class AGEF_EXPORT QtRenderVisitor : public NodeVisitor
{
public:
    QtRenderVisitor ( RenderOption* opt ) : _opt(opt) {}
    virtual void apply ( RectangleNodef& rect );
    virtual void apply ( TextNode& text );
    virtual void apply ( LineNodef& /*node*/ );
    //virtual void apply ( FontNode& font );
private:
    RenderOption* _opt;
};

inline void QtRenderVisitor::apply ( RectangleNodef& rect )
{
    //vec2f lb = (_opt->matrix * vec4f(rect.lb())).xy();
    //vec2f rt = (_opt->matrix * vec4f(rect.rt())).xy();
    vec2f lb = vec2f(rect.lb());
    vec2f rt = vec2f(rect.rt());
    //GColor color = rect.getBorderColor();  
    //QPen pen ( QColor(color.r(),color.g(),color.b(),color.a()) );

    //color = rect.getFillColor();  
    //QBrush brush ( QColor(color.r(),color.g(),color.b(),color.a()) );

    //QPen oldpen = _opt->painter->pen();
    //QBrush oldbrush = _opt->painter->brush();

    //_opt->painter->setPen ( pen );
    //_opt->painter->setBrush ( brush );

    _opt->painter->drawRect ( lb.x(), lb.y(), rt.x()-lb.x(), rt.y()-lb.y() );
    //_opt->painter->fillRect ( lb.x(), lb.y(), rt.x()-lb.x(), rt.y()-lb.y(), QColor (color>>24, (color&0x00FF0000)>>16, (color&0x0000FF00)>>8, color&0x000000FF) );

    //_opt->painter->setPen ( oldpen );
    //_opt->painter->setBrush ( oldbrush );

    for ( SGNode::iterator pp=rect.begin(); pp!=rect.end(); ++pp )
        (*pp)->accept ( *this );
}

//inline void QtRenderVisitor::apply ( FontNode& node )
//{
//    QPen pen ( node.color().c_str() );
//    QFont font ( node.family().c_str(), node.size(), QFont::Normal, node.italic() );
//    const QFont oldfont = _opt->painter->font();
//    const QPen oldpen = _opt->painter->pen();
//
//    _opt->painter->setFont ( font );
//    _opt->painter->setPen(pen);
//
//    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
//        (*pp)->accept ( *this );
//
//    _opt->painter->setPen ( oldpen );
//    _opt->painter->setFont ( oldfont );
//}

inline void QtRenderVisitor::apply ( TextNode& text )
{
    QMatrix m;
    m.reset();
    m.scale ( 1, -1 );
    QMatrix oldm = _opt->painter->matrix();
    QMatrix newm = m * oldm;
    _opt->painter->setWorldMatrix ( newm );

    FontNode* fontnode = text.fontnode();
    //QPen pen ( fontnode->color().c_str() );
    QFont font ( fontnode->family().c_str(), fontnode->size(), QFont::Normal, fontnode->italic() );
    const QFont oldfont = _opt->painter->font();
    const QPen oldpen = _opt->painter->pen();
    _opt->painter->setFont ( font );
    //_opt->painter->setPen(pen);

    const BBox& b = text.getBBox ();
    
    QRectF rc( b.min().x(), -b.min().y(), b.dimension().w(), b.dimension().h() );

    _opt->painter->drawRect ( rc.toRect() );
    _opt->painter->drawText ( rc, Qt::TextWordWrap | Qt::TextDontClip | Qt::AlignCenter, text.text().c_str() );

    _opt->painter->setWorldMatrix ( oldm );

    for ( SGNode::iterator pp=text.begin(); pp!=text.end(); ++pp )
        (*pp)->accept ( *this );

}

inline void QtRenderVisitor::apply ( LineNodef& line )
{
    ////vec2f p1 = (_opt->matrix * vec4f(line.point1())).xy();
    ////vec2f p2 = (_opt->matrix * vec4f(line.point2())).xy();
    //GColor color = line.getLineColor();
    
    //int r = color>>24;
    //int g = (color&0x00FF0000)>>16;
    //int b = (color&0x0000FF00)>>8;
    //int a = color&0x000000FF;
    //const BBox& bbox = line.getBBox ();
    //const QPen& oldpen = _opt->painter->pen();
    //_opt->painter->drawRect ( bbox.min().x(), bbox.min().y(), bbox.dimension().w(), bbox.dimension().h() ); 
    //_opt->painter->setPen ( QPen(QColor (color.r(),color.g(),color.b(),color.a())));
    _opt->painter->drawLine ( line.point1().x(), line.point1().y(), line.point2().x(), line.point2().y() );
    //_opt->painter->setPen ( oldpen );

    for ( SGNode::iterator pp=line.begin(); pp!=line.end(); ++pp )
        (*pp)->accept ( *this );
}

#endif
