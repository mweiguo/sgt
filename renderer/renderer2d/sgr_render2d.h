#ifndef _SGR_RENDERER2D_H_
#define _SGR_RENDERER2D_H_

/**
 * render2d features:
 * 1. scene management
 * 2. load scene
 */
struct SGRID
{
    int lo;
    int hi;
};

SGRID load ( const char* filename );
void update ( SGRID* ids, int length );


#endif //_SGR_RENDERER2D_H_
