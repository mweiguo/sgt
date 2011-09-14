#ifndef _FONT_LIB_H_
#define _FONT_LIB_H_

#include <map>
#include <string>
#include <GL/gl.h>
class FontLib
{
public:
    FontLib ( const char* filename );
    void displayText ( const std::wstring& str );    
private:
    GLuint list_base;
    GLuint texture_base[256];
    int char_num;
    static const std::wstring charset;
    std::map<wchar_t, unsigned int> charmap;
};

#endif // _FONT_LIB_H_
