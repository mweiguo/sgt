#ifndef _SGR_RENDERER2D_H_
#define _SGR_RENDERER2D_H_

/**
 * render2d features:
 * 1. scene management
 * 2. load scene
 * 3. unload scene
 */
struct SGRID
{
    int lo;
    int hi;
};

void r2d_init ();

int r2d_load_scene ( const char* filename );
void r2d_unload_scene ( int sceneID );

void r2d_update_scenes ( int* ids, int length );

void r2d_home ();
void r2d_translate ( float x, float y );
void r2d_scale ( float s );
void r2d_resize ( int width, int height );

void r2d_get_viewport_rect ( float* x_y_width_height );

#endif //_SGR_RENDERER2D_H_
