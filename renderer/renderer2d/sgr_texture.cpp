#include "sgr_texture.h"
#include <IL/ilut.h>

Texture::Texture ( const char* texfilename )
{
    texturefilename = texfilename;
    // load texture from file
    texture = ilutGLLoadImage ( const_cast<char*>(texfilename) );
}

Texture::~Texture()
{
    if ( texture != 0 )
        glDeleteTextures ( 1, &texture );
    texture = 0;
}

