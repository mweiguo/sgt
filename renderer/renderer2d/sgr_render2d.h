#ifndef _SGR_RENDERER2D_H_
#define _SGR_RENDERER2D_H_

/**
 * render2d features:
 * 1. scene management
 * 2. load scene
 * 3. unload scene
 * 4. layer access
 */
struct SGRID
{
    int lo;
    int hi;
};

void r2d_init ();

/** scene access
 */
int r2d_load_scene ( const char* filename );
void r2d_unload_scene ( int sceneID );

void r2d_update_scenes ( int* ids, int length );

/** navigation interface
 */
void r2d_home ();
void r2d_translate ( float x, float y );
void r2d_scale ( float s );
void r2d_resize ( int width, int height );

/** layer access
 */
int r2d_get_layers ( int sceneID, int* layers );
void r2d_layer_visible ( int sceneID, int layerid, bool isVisible );
void r2d_layer_foreground_color ( int sceneID, int layerid, unsigned short* rgb );
void r2d_layer_background_color ( int sceneID, int layerid, unsigned short* rgb );
const char* r2d_get_layer_name ( int sceneID, int layerid );
bool r2d_get_layer_visible ( int sceneID, int layerid );
void r2d_get_layer_foreground_color ( int sceneID, int layerid, unsigned short* rgb );
void r2d_get_layer_background_color ( int sceneID, int layerid, unsigned short* rgb );

void r2d_get_viewport_rect ( float* x_y_width_height );

#endif //_SGR_RENDERER2D_H_
