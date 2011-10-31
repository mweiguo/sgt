#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>

class Texture
{
public:
    Texture ( const char* texfilename );
    ~Texture();
    std::string texturefilename;
    unsigned int texture;
};

#endif// _TEXTURE_H_
