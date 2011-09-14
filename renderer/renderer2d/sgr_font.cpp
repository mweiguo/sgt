#include "fontlib.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>

using namespace std;

const wstring FontLib::charset = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789中国的人";

inline int next_p2 (int a )
{
    int rval=1;
    // rval<<=1 Is A Prettier Way Of Writing rval*=2;
    while(rval<a) rval<<=1;
    return rval;
}

FontLib::FontLib ( const char* filename )
{
    FT_Library  library;
    int error = FT_Init_FreeType( &library );
    if ( error )
	cout << "FT_Init_FreeType failed" << endl;

    FT_Face face;
    error = FT_New_Face( library, filename, 0, &face );
    if ( error == FT_Err_Unknown_File_Format )
	cout << "unsupported format" << endl;
    else if ( error )
	cout << "openf font file failed" << endl;
    
    error = FT_Set_Char_Size ( face, 0, 16*64, 300, 300 );
    if ( error )
	cout << "FT_Set_Char_Size error " << endl;

    char_num = charset.size();
    list_base = glGenLists ( charset.size() );
    glGenTextures ( charset.size(), texture_base );

    for ( size_t i=0; i<charset.size(); i++ ) {
	int glyph_index = FT_Get_Char_Index( face, charset[i] );
	error = FT_Load_Glyph ( face, glyph_index, 0 );
	if ( error )
	    cout << "FT_Load_Glyph failed " << endl;
	error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
	if ( error )
	    cout << "FT_Render_Glyph error" << endl;
	// create bitmap
	FT_Bitmap& bitmap = face->glyph->bitmap;
	int width = next_p2( bitmap.width );
	int height = next_p2( bitmap.rows );
	GLubyte* expanded_data = new GLubyte[ 2 * width * height];
	for(int j=0; j <height;j++) {
	    for(int i=0; i < width; i++){
		expanded_data[2*(i+j*width)]= expanded_data[2*(i+j*width)+1] =
		    (i>=bitmap.width || j>=bitmap.rows) ?
		    0 : bitmap.buffer[i + bitmap.width*j];
	    }
	}
	// create texture
	glBindTexture( GL_TEXTURE_2D, texture_base[i] );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );
	delete [] expanded_data;
	// make display list
	charmap.insert ( pair<wchar_t,unsigned int>(charset[i], list_base + i) );
	glNewList(list_base + i,GL_COMPILE);
	glBindTexture(GL_TEXTURE_2D, list_base + i );
	float x=(float)bitmap.width / (float)width, y=(float)bitmap.rows / (float)height;
	glBegin(GL_QUADS);
	glTexCoord2d(0,0); glVertex2f(0,1);
	glTexCoord2d(0,y); glVertex2f(0,0);
	glTexCoord2d(x,y); glVertex2f(1,0);
	glTexCoord2d(x,0); glVertex2f(1,1);
	glEnd();
	glEndList();
    }
 
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}


void FontLib::displayText ( const wstring& str )
{
    glPushMatrix();
    for ( size_t i=0; i<str.size(); i++ ) {
	map<wchar_t, unsigned int>::iterator pp = charmap.find ( str[i] );
	if ( pp != charmap.end() ) {
	    glCallList ( pp->second );
	    glTranslatef ( 1, 0, 0 );
	}
    }
    glPopMatrix();
}
