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
    static FontMetric& getInst()
    {
        static FontMetric inst;
        return inst;
    }
    BBox getBBox ( const FontNode& fontnode, const string& content )
    {
        QFont font ( fontnode.family().c_str(), fontnode.size(), QFont::Normal, fontnode.italic() );
        QFontMetricsF m ( font );
        QRectF rc = m.boundingRect ( QRectF(0,0,100,100), Qt::AlignCenter, content.c_str() );
        //QSizeF sz = m.size ( Qt::TextWordWrap, content.c_str() );
        BBox b;
        b.init ( vec3f(0.f, 0.f, 0.f) );
        b.expandby ( vec3f(rc.width(), rc.height(), 0.f) );
        return b;
    }
};
}

#endif // _FONT_METRIC_H
