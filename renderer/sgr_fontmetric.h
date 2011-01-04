#ifndef _FONT_METRIC_H
#define _FONT_METRIC_H


#include <QFont>
#include <QFontMetricsF>
#include <QSizeF>

#include "sgr_bbox.h"
#include "sgr_vec3.h"
#include "sgr_fontnode.h"
namespace SGR
{

class SGR_DLL FontMetric
{
public:
    static BBox getBBox ( const FontNode& fontnode, const string& content )
    {
        //	    qDebug ( "QFont font ( %s, %d, QFont::Normal, %d );", fontnode.family().c_str(), fontnode.pointSize(), fontnode.italic() );
        QFont font ( fontnode.family().c_str(), fontnode.pointSize(), QFont::Normal, fontnode.italic() );
        //	    qDebug ( "QFont font2 ( %s, %d, QFont::Normal, fontnode.italic() );", fontnode.family().c_str(), fontnode.pointSize() );
        QFontMetricsF m ( font );
        //	    qDebug ( "QFont font3 ( %s, %d, QFont::Normal, fontnode.italic() );", fontnode.family().c_str(), fontnode.pointSize() );
        QRectF rc = m.boundingRect ( QRectF(0,0,1000,1000), Qt::AlignCenter, content.c_str() );
        //	    QRectF rc = m.boundingRect ( content.c_str() );
        //	    qDebug ( "QFont font4 ( %s, %d, QFont::Normal, fontnode.italic() );", fontnode.family().c_str(), fontnode.pointSize() );

        //QSizeF sz = m.size ( Qt::TextWordWrap, content.c_str() );
        BBox b;
        b.init ( vec3f(0.f, 0.f, 0.f) );
        b.expandby ( vec3f(rc.width(), rc.height(), 0.f) );
        return b;
    }
};
}

#endif // _FONT_METRIC_H
