#ifndef _GCOLOR_H_
#define _GCOLOR_H_

//#include "parentfinder.h"
#include <colorconv.h>
#include "agef_global.h"

class AGEF_EXPORT GColor
{
public:
    GColor ( unsigned int color=0x000000ff ) : _color (color) {}
    GColor ( const string& color ) { _color = ColorConv::string2hex(color); }
    GColor ( const GColor& rhs ) : _color(rhs._color) {}
    void setColor ( unsigned int color ) { _color = color; }
    void setColor ( int r, int g, int b, int a ) {}
    void setColor ( const string& color ) { _color = ColorConv::string2hex(color); }
    unsigned int getColor () const { return _color; }
    string toString() const { return ColorConv::hex2string(_color); }
    unsigned int r() { return ((_color & 0xff000000) >> 24); }
    unsigned int g() { return ((_color & 0x00ff0000) >> 16); }
    unsigned int b() { return ((_color & 0x0000ff00) >> 8); }
    unsigned int a() { return ((_color & 0x000000ff)); }
private:
    // 0xRRGGBBAA
    unsigned int _color;
};


#endif // _GCOLOR_H_
