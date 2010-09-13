#ifndef _GCOLOR_H_
#define _GCOLOR_H_

//#include "sgr_parentfinder.h"
#include <sgr_colorconv.h>
#include "sgr_global.h"
namespace SGR
{

class /*SGR_DLL*/ GColor
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

#include <list>
using namespace std;
class SGR_DLL GColorFactory : private list<unsigned int*>
{
public:
    static GColorFactory& getInst()
    {
        static GColorFactory inst;
        return inst;
    }
    unsigned int* createColor ( unsigned int color ) 
    {
        push_back ( new unsigned int(color) );
        return back();
    }
    unsigned int* createColor ( const string& color ) 
    {
        push_back ( new unsigned int(ColorConv::string2hex(color)) );
        return back();
    }
    void clearAllColors ()
    {
        list<unsigned int*>::iterator pp, e=end();
        for ( pp=begin(); pp!=e; ++pp )
            delete *pp;
        clear();
    }

};
}
#endif // _GCOLOR_H_
