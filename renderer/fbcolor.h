#ifndef _FB_COLOR_H_
#define _FB_COLOR_H_

#include "gcolor.h"

class AGEF_EXPORT FgBgColor
{
public:
    FgBgColor () {}
    FgBgColor ( const FgBgColor& rhs ) : _fg(rhs._fg), _bg(rhs._bg) {}
    void setFgColor ( const GColor& color ) { _fg = color; }
    const GColor& getFgColor () const { return _fg; }
    void setBgColor ( const GColor& color ) { _bg = color; }
    const GColor& getBgColor () const { return _bg; }

    void setFgColorPtr ( GColor* color ) { _fg.setColor ( (unsigned int)color); }
    GColor* getFgColorPtr () const { return (GColor*)_fg.getColor(); }
    void setBgColorPtr ( GColor* color ) { _bg.setColor( (unsigned int)color); }
    GColor* getBgColorPtr () const { return (GColor*)_bg.getColor(); }
private:
    GColor _fg, _bg;
};

class LayerNode;
class AGEF_EXPORT FgBgColorComponent : private FgBgColor
{
private:
    bool _isUseLayerFgColor;
    bool _isUseLayerBgColor;
public:
    FgBgColorComponent() : _isUseLayerFgColor(false), _isUseLayerBgColor(false) {}
    FgBgColorComponent ( const FgBgColorComponent& rhs ) : FgBgColor ( rhs )
    {
        _isUseLayerFgColor = rhs._isUseLayerFgColor;
        _isUseLayerBgColor = rhs._isUseLayerBgColor;
    }
    void setUseLayerFgColor ( bool b ) { _isUseLayerFgColor = b; };
    void setUseLayerBgColor ( bool b ) { _isUseLayerBgColor = b; }
    bool isUseLayerFgColor () const { return _isUseLayerFgColor; }
    bool isUseLayerBgColor () const { return _isUseLayerBgColor; }

    void useLayerBgColor ( LayerNode* player );
    void setBgColor ( GColor color );
    GColor getBgColor () const;
    void useLayerFgColor ( LayerNode* player );
    void setFgColor ( GColor color );
    GColor getFgColor ();
};

#endif// _FB_COLOR_H_
