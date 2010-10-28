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
#include <tinylog.h>

namespace SGR
{

struct SGR_DLL RenderOption
{
    BBox bb;
    QPainter* painter;
    //mat4f* mvmatrix;
    // matrix = viewpmapping * model-view
    mat4f matrix, reverse_mvpw;
    float scale;
    RenderOption() : painter(0) {}
};

#include "sgr_attrset.h"
class SGR_DLL QtStateChanger
{
public:
    QtStateChanger ( RenderOption* opt, AttrSet* set ) : _opt(opt), _isPenValid(false), _isBrushValid(false), _isFontValid(false)
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

        // set background color ==>  QBrush
        FontNode* fontnode = set->getFont();
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



class SGR_DLL QtRenderVisitor : public NodeVisitor
{
public:
    QtRenderVisitor ( RenderOption* opt ) : _opt(opt) {}
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
    RenderOption* _opt;
};

inline void QtRenderVisitor::apply ( RectangleNodef& rect )
{
    QRectF rc ( rect.lb().x(), rect.lb().y(), rect.w(), rect.h() );

    _opt->painter->drawRect ( rc );

    for ( SGNode::iterator pp=rect.begin(); pp!=rect.end(); ++pp )
        (*pp)->accept ( *this );
}

inline void QtRenderVisitor::apply ( TextNode& text )
{
    QRectF rc, rc1;
    QMatrix oldm = _opt->painter->matrix();
    QMatrix newm;


    float scale;
    vec3f scenesize;
    if ( text.getSizeMode() == TextNode::TXTSIZEMODE_SCREEN )
    {
	vec4f p0 = _opt->reverse_mvpw * vec4f(0,0,0,1);
	vec4f p1 = _opt->reverse_mvpw * vec4f(text.getSize().x(), 0, 0, 1);
 	vec3f screen_size = FontMetric::getInst().getBBox ( *(text.getAttrSet()->getFont()), text.text().c_str() ).dimension();
	scale = (p1.x() - p0.x()) / screen_size.x();
 	scenesize = screen_size;
    }
    else if ( text.getSizeMode() == TextNode::TXTSIZEMODE_SCENE )
    {
 	scenesize = FontMetric::getInst().getBBox ( *(text.getAttrSet()->getFont()), text.text().c_str() ).dimension();
 	scale = text.renderScale();
    }

    float dx=0, dy=0;
    if ( text.isAnchorHCenter () )
	dx = -text.width()/2.f;
    else if ( text.isAnchorRight () )
	dx = -text.width();

    float calcHeight = text.width() * scenesize.y() / scenesize.x();
    if ( text.isAnchorVCenter () )
	dy = -calcHeight / 2.f;
    else if ( text.isAnchorTop () )
	dy = -calcHeight;

    // calculate rectangle for text
    rc.setRect ( text.anchorPos().x()+dx, text.anchorPos().y()+dy, scenesize.x(), scenesize.y() );
//    LOG_INFO ("rect.x=%f, rect.y=%f, rect.w=%f, rect.h=%f\n", text.anchorPos().x()+dx, text.anchorPos().y()+dy, scenesize.x(), scenesize.y() );
    // this point is rect lb point
    vec4f ttt = _opt->matrix * vec4f ( text.anchorPos().x()+dx, text.anchorPos().y()+dy, 0, 1 );
    // reverse and scale it inplace
    vec4f ttt1 = _opt->matrix * vec4f ( text.anchorPos().x()+dx+scenesize.x(), text.anchorPos().y()+dy+scenesize.y(), 0, 1 );
    QMatrix mm1, mm2, mm3, mm4, mm5, mm;
    mm.translate ( ttt.x(), ttt.y() );
    mm.scale ( scale, scale );
    mm.translate ( 0, ttt1.y()-ttt.y() );
    mm.scale ( 1, -1 );
    mm.translate ( -ttt.x(), -ttt.y() );
    newm = oldm * mm;
/*     } */
/*     else if ( text.getSizeMode() == TextNode::TXTSIZEMODE_SCENE ) */
/*     { */
/* // 	// in this mode, scene size is fixed, and current device-scene coordinate mapping rate is 1:1, so screen unit = scene unit */
/*  	vec3f scenesize = FontMetric::getInst().getBBox ( *(text.getAttrSet()->getFont()), text.text().c_str() ).dimension(); */
/*  	float scale = text.renderScale(); */

/* 	float dx=0, dy=0; */
/* 	if ( text.isAnchorHCenter () ) */
/* 	    dx = -text.width()/2.f; */
/* 	else if ( text.isAnchorRight () ) */
/* 	    dx = -text.width(); */

/* 	float calcHeight = text.width() * scenesize.y() / scenesize.x(); */
/* 	if ( text.isAnchorVCenter () ) */
/* 	    dy = -calcHeight / 2.f; */
/* 	else if ( text.isAnchorTop () ) */
/* 	    dy = -calcHeight; */

/* 	// calculate rectangle for text */
/* 	rc.setRect ( text.anchorPos().x()+dx, text.anchorPos().y()+dy, scenesize.x(), scenesize.y() ); */
/* 	// this point is rect lb point */
/*  	vec4f ttt = _opt->matrix * vec4f ( text.anchorPos().x()+dx, text.anchorPos().y()+dy, 0, 1 ); */
/* 	// reverse and scale it inplace */
/* 	vec4f ttt1 = _opt->matrix * vec4f ( text.anchorPos().x()+dx+scenesize.x(), text.anchorPos().y()+dy+scenesize.y(), 0, 1 ); */
/*  	QMatrix mm1, mm2, mm3, mm4, mm5, mm; */
/* 	mm.translate ( ttt.x(), ttt.y() ); */
/* 	mm.scale ( scale, scale ); */
/* 	mm.translate ( 0, ttt1.y()-ttt.y() ); */
/* 	mm.scale ( 1, -1 ); */
/* 	mm.translate ( -ttt.x(), -ttt.y() ); */
/* 	newm = oldm * mm; */
/*     } */

    _opt->painter->setWorldMatrix ( newm );
    _opt->painter->drawText ( rc, Qt::TextWordWrap | Qt::TextDontClip | Qt::AlignCenter, text.text().c_str() );
    _opt->painter->drawRect ( rc );
    _opt->painter->drawRect ( rc1 );
    _opt->painter->setWorldMatrix ( oldm );

    for ( SGNode::iterator pp=text.begin(); pp!=text.end(); ++pp )
        (*pp)->accept ( *this );
}

inline void QtRenderVisitor::apply ( LineNodef& line )
{
    QPointF p1 ( line.point1().x(), line.point1().y() );
    QPointF p2 ( line.point2().x(), line.point2().y() );
//     stringstream ss;
//     ss << "(" << line.point1().x() << ", " << line.point1().y() << ") (" << line.point2().x() << ", " << line.point2().y() << ")";
//     qDebug ( ss.str().c_str() );
    _opt->painter->drawLine ( p1, p2 );
    
    for ( SGNode::iterator pp=line.begin(); pp!=line.end(); ++pp )
        (*pp)->accept ( *this );
}

inline void QtRenderVisitor::apply ( PointNode& pnt )
{
    QPointF p ( pnt.x(), pnt.y() );
    _opt->painter->drawPoint ( p );

    for ( SGNode::iterator pp=pnt.begin(); pp!=pnt.end(); ++pp )
        (*pp)->accept ( *this );
}

inline void QtRenderVisitor::apply ( PolylineNode2Df& line )
{
    if ( !line.pointsempty() )
    {
        QPointF* tmp = new QPointF[line.pointssize()];
        QPointF* b = tmp;
	
        for ( PolylineNode2Df::pointiterator pp=line.pointbegin(); pp!=line.pointend(); ++pp, ++tmp )
        {
            tmp->setX ( (*pp).x() );
            tmp->setY ( (*pp).y() );
        }

        _opt->painter->drawPolyline ( b, line.pointssize() );
        delete[] b;

    }

    for ( SGNode::iterator pp=line.begin(); pp!=line.end(); ++pp )
        (*pp)->accept ( *this );
}

inline void QtRenderVisitor::apply ( PolyNode2Df& line )
{
    if ( !line.pointsempty() )
    {
        QPointF* tmp = new QPointF[line.pointssize()];
        QPointF* b = tmp;
        for ( PolylineNode2Df::pointiterator pp=line.pointbegin(); pp!=line.pointend(); ++pp, ++tmp )
        {
            tmp->setX ( (*pp).x() );
            tmp->setY ( (*pp).y() );
        }

        _opt->painter->drawPolygon ( b, line.pointssize() );
        delete[] b;
    }

    for ( SGNode::iterator pp=line.begin(); pp!=line.end(); ++pp )
        (*pp)->accept ( *this );
}

inline void QtRenderVisitor::apply ( MeshLineNode& line )
{
    ParentFinder<MeshNode3f> finder ( &line );
    MeshNode3f& mesh = *(finder.target());

    MeshLineNode::pntiterator pp, end=line.pntend();
    switch ( line.type() )
    {
    case MeshLineNode::LINETYPE_LINES:
        if ( (line.pntsize() % 2) == 0 )
        {
            for ( pp=line.pntbegin(); pp!=end; ++pp )
            {
                vec3f& p1 = mesh[*pp];
                vec3f& p2 = mesh[*(++pp)];
                
                QPointF tp1 ( p1.x(), p1.y() );
                QPointF tp2 ( p2.x(), p2.y() );

                _opt->painter->drawLine ( tp1, tp2 );
            }
        }
        break;
    case MeshLineNode::LINETYPE_LINE_STRIP:
        {
            QVector<QPointF> tmp;
            for ( pp=line.pntbegin(); pp!=end; ++pp )
            {
                vec3f& pnt = mesh[*pp];
                tmp.push_back ( QPointF(pnt.x(), pnt.y()) );
            }

            _opt->painter->drawPolyline ( &tmp[0], tmp.size() );
            //_opt->painter->drawLines ( tmp );
        }
        break;
    case MeshLineNode::LINETYPE_LINE_LOOP:
        QVector<QPointF> tmp;
        for ( pp=line.pntbegin(); pp!=end; ++pp )
        {
            vec3f& pnt = mesh[*pp];
            tmp.push_back ( QPointF(pnt.x(), pnt.y()) );
        }
        if ( !tmp.empty() )
            tmp.push_back (tmp[0]);

        _opt->painter->drawLines ( tmp );
        break;
    }

    for ( SGNode::iterator pp=line.begin(); pp!=line.end(); ++pp )
        (*pp)->accept ( *this );
}

inline void QtRenderVisitor::apply ( MeshPointNode& pnt )
{
    ParentFinder<MeshNode3f> finder ( &pnt );
    if ( finder.target() )
    {
        //MeshNode3f mesh = *(finder.target());
        //const vec3f& p = mesh[ pnt.coordIdx() ];
        //QPointF point ( p.x(), p.y() );
        //_opt->painter->drawPoint ( point );
        const BBox& box = pnt.getBBox();
        //float delta = 0.02;
        QRectF rc ( box.minvec().x(), box.minvec().y(), box.dimension().w(), box.dimension().h() );
        _opt->painter->drawRect ( rc );
    }

    for ( SGNode::iterator pp=pnt.begin(); pp!=pnt.end(); ++pp )
        (*pp)->accept ( *this );
}

}
#endif
