#ifndef _FONT_H_
#define _FONT_H_

#include <vector>
#include <string>

class Font
{
public:
    Font ( const char* filename );
    ~Font ();
    void drawText ( const char* str );    
    void getSize ( const char* str, float& w, float& h );
    std::string fontfilename;
    std::vector<float> widths;
    std::vector<float> heights;
private:
    unsigned int list_base; // display list
    unsigned int texture_base[128];
    unsigned int chnum;
};

#endif // _FONT_H_
