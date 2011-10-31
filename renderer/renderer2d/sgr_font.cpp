#include "sgr_font.h"
#include "sgr_texture.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <windows.h>
#include <GL/gl.h>
#include <vector>
#include <utf8.h>
#include <fstream>

using namespace std;

inline int next_p2 (int a )
{
    int rval=1;
    // rval<<=1 Is A Prettier Way Of Writing rval*=2;
    while(rval<a) rval<<=1;
    return rval;
}

Font::Font ( const char* filename )
{
    fontfilename = filename;

    ifstream in;
    in.open ( fontfilename.c_str() );
    if ( false == in.is_open () ) {
	fontfilename = "simhei.ttf";
    }
    in.close();
    
    cout << "load font '" << fontfilename << "'" << endl;
    FT_Library  library;
    int error = FT_Init_FreeType( &library );
    if ( error )
	cout << "FT_Init_FreeType failed" << endl;

    FT_Face face;
    error = FT_New_Face( library, fontfilename.c_str(), 0, &face );
    if ( error == FT_Err_Unknown_File_Format )
	cout << "unsupported format" << endl;
    else if ( error )
	cout << "openf font file failed" << endl;
    
//    error = FT_Set_Char_Size ( face, 0, 16*64, 300, 300 );
    double pixelsize = 128;
    error = FT_Set_Pixel_Sizes ( face, 0, pixelsize );
    if ( error )
	cout << "FT_Set_Char_Size error " << endl;

    // generate display list
    chnum  = 128;
    list_base = glGenLists ( chnum );
    glGenTextures ( chnum, texture_base );

    widths.reserve ( chnum );
    heights.reserve ( chnum );
    for ( size_t i=0; i<chnum; i++ ) {
	int glyph_index = FT_Get_Char_Index( face, i );
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
	glNewList(list_base + i,GL_COMPILE);
	glBindTexture(GL_TEXTURE_2D, list_base + i );
	float x=(float)bitmap.width / (float)width, y=(float)bitmap.rows / (float)height;
//	cout << "i : " << i << "x = " << x << ", y = " << y << endl;
	glBegin(GL_QUADS);
	float r = bitmap.rows / pixelsize;
	float r1 = bitmap.width / pixelsize;
	widths.push_back ( r1 );
	heights.push_back ( r );
	glTexCoord2d(0,0); glVertex2f(0, r);
	glTexCoord2d(0,y); glVertex2f(0, 0);
	glTexCoord2d(x,y); glVertex2f(r1, 0);
	glTexCoord2d(x,0); glVertex2f(r1, r);
	glEnd();
	glEndList();
    }
 
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

Font::~Font ()
{
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~Font::~Font ()" << endl;
    glDeleteLists ( list_base, chnum );
    glDeleteTextures ( chnum, texture_base );
    chnum = 0;
}

void Font::drawText ( const char* str )
{
    vector<int> utf32result;
    utf8::utf8to32(str, str + strlen (str), back_inserter(utf32result));

    glPushMatrix();
    for ( vector<int>::iterator pp=utf32result.begin(); pp!=utf32result.end(); ++pp ) {
	glCallList ( list_base + *pp );
// 	cout <<  *pp << ' ' << endl;
	glTranslatef ( widths[*pp], 0, 0 );
    }
//     cout << endl;
    glPopMatrix();
}

void Font::getSize ( const char* str, float& w, float& h )
{
    vector<int> utf32result;
    utf8::utf8to32(str, str + strlen (str), back_inserter(utf32result));
    if ( utf32result.empty() == false )
    {
	h = 0;
	for ( vector<int>::iterator pp=utf32result.begin(); pp!=utf32result.end(); ++pp ) {
	    if ( *pp < 0 || *pp>=widths.size() ) {
		w += widths[65];
		h = h > heights[65] ? h : heights[65];
	    } else {
		w += widths[*pp];
		h = h > heights[*pp] ? h : heights[*pp];
	    }
	}
    }
}
