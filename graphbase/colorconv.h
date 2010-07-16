#ifndef _COLORCONV_H
#define _COLORCONV_H

#include <sstream>
#include <string>
#include <stdexcept>
using namespace std;
#include "agef_global.h"

class AGEF_EXPORT ColorConv
{
public:
    // #RRGGBBAA
    static string hex2string ( unsigned int color )
    {
        unsigned short rgba[4];
        rgba[0] = (color & 0xff000000) >> 24;
        rgba[1] = (color & 0x00ff0000) >> 16;
        rgba[2] = (color & 0x0000ff00) >> 8;
        rgba[3] = (color & 0x000000ff);

        string rgbas[4];
        for ( int i=0; i<4; i++ )
        {
            stringstream ss;
            ss.fill ( '0' );
            ss.width ( 2 );
            ss << std::hex << rgba[i];
            rgbas[i] = ss.str();
        }
        
        stringstream ss;
        ss << "#" << rgbas[0] << rgbas[1] << rgbas[2] << rgbas[3];
        return ss.str();
    }

    static unsigned int string2hex ( const string& color )
    {
        unsigned int c;
        stringstream s;
        unsigned short r, g, b, a;
        if ( color.size () == 7 )  // #RRGGBB
        {
            s << std::hex << color.substr ( 1, 2 );
            s >> r;

            s.clear();
            s << std::hex << color.substr ( 3, 2 );
            s >> g;

            s.clear();
            s << std::hex << color.substr ( 5, 2 );
            s >> b;

            c = (0x00000000 | r) << 24;
            c |= (0x00000000 | g) << 16;
            c |= (0x00000000 | b) << 8;
            c |= 0x000000ff;
        }
        else if ( color.size () == 9 ) // #RRGGBBAA
        {
            s << std::hex << color.substr ( 1, 2 );
            s >> r;

            s.clear();
            s << std::hex << color.substr ( 3, 2 );
            s >> g;

            s.clear();
            s << std::hex << color.substr ( 5, 2 );
            s >> b;

            s.clear();
            s << std::hex << color.substr ( 7, 2 );
            s >> a;

            c = (0x00000000 | r) << 24;
            c |= (0x00000000 | g) << 16;
            c |= (0x00000000 | b) << 8;
            c |= (0x00000000 | a);
        }
        else
        {
            stringstream ss;
            ss << "ColorConv::string2hex: argument should in form #RRGGBB or #RRGGBBAA, argument=" << color;
            throw std::invalid_argument ( ss.str() );
        }
        return c;
    }
};

#endif // _COLORCONV_H
