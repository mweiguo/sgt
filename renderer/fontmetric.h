#ifndef _FONT_METRIC_H
#define _FONT_METRIC_H


#include <QFont>
#include <QFontMetricsF>
#include <QSizeF>

#include "bbox.h"
#include "vec3.h"
#include "fontnode.h"

class AGEF_EXPORT FontMetric
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
        QSizeF sz = m.size ( Qt::TextWordWrap, content.c_str() );
        BBox b;
        b.init ( vec3f(0.f, 0.f, 0.f) );
        b.expandby ( vec3f(sz.width(), sz.height(), 0.f) );
        return b;
    }
};


#endif // _FONT_METRIC_H
