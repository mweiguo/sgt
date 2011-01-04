#include "sgr_renderfunctor.h"

namespace SGR
{
    QtRenderVisitor::QtRenderVisitor ( RenderOption* opt ) : _opt(opt)
    {
    }

    QtRenderVisitor::~QtRenderVisitor()
    {
    }

    void QtRenderVisitor::apply ( RectangleNodef& rect )
    {
        QRectF rc ( rect.lb().x(), rect.lb().y(), rect.w(), rect.h() );

        _opt->painter->drawRect ( rc );

        for ( SGNode::iterator pp=rect.begin(); pp!=rect.end(); ++pp )
            (*pp)->accept ( *this );
    }

    void QtRenderVisitor::apply ( TextNode& text )
    {
        QRectF rc;
        QMatrix oldm = _opt->painter->matrix();
        QMatrix newm;

        float scale;
        vec3f scenesize;
        int textid = text.getID();
        float ww/*, hh*/;
        if ( text.getSizeMode() == TextNode::TXTSIZEMODE_SCREEN )
        {
            vec4f p0 = _opt->reverse_mvpw * vec4f(0,0,0,1);
            vec4f p1 = _opt->reverse_mvpw * vec4f(text.getSize().x(), 0, 0, 1);
            vec3f screen_size = FontMetric::getBBox ( *(text.getAttrSet()->getFont()), text.text().c_str() ).dimension();
            scale = (p1.x() - p0.x()) / screen_size.x();
            scenesize = screen_size;
            ww = p1.x() - p0.x();
        }
        else if ( text.getSizeMode() == TextNode::TXTSIZEMODE_SCENE )
        {
            scenesize = FontMetric::getBBox ( *(text.getAttrSet()->getFont()), text.text().c_str() ).dimension();
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

        if ( text.getSizeMode() == TextNode::TXTSIZEMODE_SCREEN )
        {
            if ( text.isAnchorHCenter () )
                dx = -ww/2.f;
            else if ( text.isAnchorRight () )
                dx = -ww;

            float calcHeight = ww * scenesize.y() / scenesize.x();
            if ( text.isAnchorVCenter () )
                dy = -calcHeight / 2.f;
            else if ( text.isAnchorTop () )
                dy = -calcHeight;
        }

        // calculate rectangle for text
        rc.setRect ( text.anchorPos().x()+dx, text.anchorPos().y()+dy, scenesize.x(), scenesize.y() );
        // this point is rect lb point
        vec4f ttt = _opt->matrix * vec4f ( text.anchorPos().x()+dx, text.anchorPos().y()+dy, 0, 1 );
        //vec4f ttt = _opt->matrix * vec4f ( text.anchorPos().x(), text.anchorPos().y(), 0, 1 );
        // reverse and scale it inplace
        vec4f ttt1 = _opt->matrix * vec4f ( text.anchorPos().x()+dx+scenesize.x(), text.anchorPos().y()+dy+scenesize.y(), 0, 1 );
        QMatrix mm;
        mm.translate ( ttt.x(), ttt.y() );
        mm.scale ( scale, scale );
        mm.translate ( 0, ttt1.y()-ttt.y() );
        mm.scale ( 1, -1 );
        mm.translate ( -ttt.x(), -ttt.y() );
        newm = oldm * mm;

        _opt->painter->setWorldMatrix ( newm );
        if ( text.isDrawBackground() )
            _opt->painter->drawRect ( rc );

        _opt->painter->drawText ( rc, Qt::TextWordWrap | Qt::TextDontClip | Qt::AlignCenter, text.text().c_str() );
        _opt->painter->setWorldMatrix ( oldm );

        for ( SGNode::iterator pp=text.begin(); pp!=text.end(); ++pp )
            (*pp)->accept ( *this );
    }

    void QtRenderVisitor::apply ( LineNodef& line )
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

    void QtRenderVisitor::apply ( PointNode& pnt )
    {
        QPointF p ( pnt.x(), pnt.y() );
        _opt->painter->drawPoint ( p );

        for ( SGNode::iterator pp=pnt.begin(); pp!=pnt.end(); ++pp )
            (*pp)->accept ( *this );
    }

    void QtRenderVisitor::apply ( PolylineNode2Df& line )
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

    void QtRenderVisitor::apply ( PolyNode2Df& line )
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

    void QtRenderVisitor::apply ( MeshLineNode& line )
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

    void QtRenderVisitor::apply ( MeshPointNode& pnt )
    {
        ParentFinder<MeshNode3f> finder ( &pnt );
        if ( finder.target() )
        {
            MeshNode3f mesh = *(finder.target());
            vec3f& p = mesh [ pnt.coordIdx() ];
            float delta = pnt.pointSize()/2.f;

            //QPointF point ( p.x(), p.y() );
//            const BBox& box = pnt.getBBox();
            //float delta = 0.02;
            QRectF rc ( p.x()-delta, p.y()-delta, pnt.pointSize(), pnt.pointSize() );
            _opt->painter->drawRect ( rc );
        }

        for ( SGNode::iterator pp=pnt.begin(); pp!=pnt.end(); ++pp )
            (*pp)->accept ( *this );
    }

    void QtRenderVisitor::apply ( CircleNode& circle )
    {
        SGR::vec3f cen = circle.getCenter();
        float r = circle.getRadius();
        _opt->painter->drawEllipse ( QRectF ( cen.x()-r, cen.y()-r, 2*r, 2*r ) );

        for ( SGNode::iterator pp=circle.begin(); pp!=circle.end(); ++pp )
            (*pp)->accept ( *this );
    }

    void QtRenderVisitor::apply ( ImageNode& image )
    {
	int renderstart = clock();
        QRectF src(0, 0, image.widthInPixel(), image.heightInPixel() );
        BBox bb = image.getBBox ();
        QRectF dst( bb.minvec().x(), bb.minvec().y(), bb.dimension().x(), bb.dimension().y() );

        _opt->painter->drawPixmap ( dst, image.qimage(), src );

        for ( SGNode::iterator pp=image.begin(); pp!=image.end(); ++pp )
            (*pp)->accept ( *this );
	int renderend = clock();

	qDebug ( "drawImageNode: %d clock", renderend-renderstart );
    }

}
