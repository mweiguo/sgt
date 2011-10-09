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

/** scene interface
 */
int r2d_load_scene ( const char* filename );
void r2d_unload_scene ( int sceneID );
void r2d_get_scene_minmax ( int sceneid, float* minxy, float* maxxy );
void r2d_update_scenes ( int* ids, int length );
void r2d_get_scene_position ( int x, int y, float& scenex, float& sceney );

/** navigation interface
 */
void r2d_translate ( float x, float y );
void r2d_scale ( float s );
void r2d_loadidentity ();
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
/** get current view frustum coordinates (world coordinate)
 */
void r2d_get_viewport_rect ( float* x_y_width_height );

/** element traverse
 */
#define R2D_ROOT          0
#define R2D_PARENT        1
#define R2D_FIRST_CHILD   2
#define R2D_LAST_CHILD    3
#define R2D_NEXT_SIBLING  4
/** user can use this function move current cursor
 */
int r2d_to_element ( int sceneID, int elementType );
int r2d_get_node_type ( int sceneID );
void r2d_rect_points ( int sceneID, int rectid, float* xyz );
#endif //_SGR_RENDERER2D_H_
