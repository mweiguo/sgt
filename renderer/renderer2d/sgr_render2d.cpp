#include "sgr_render2d.h"
#include "sgr_lc.h"
#include "sgr_vfculler.h"
#include "sgr_statesetbuilder.h"
#include "sgr_renderer.h"
#include "sgr_nodetypes.h"
#include <vector>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "mat4.h"
#include <ctime>

using namespace std;

vector<LC*> globalLC;
mat4f mvmat;

void r2d_init ()
{
    glClearColor ( 1.0, 1.0, 1.0, 1.0 );
    glEnable ( GL_DEPTH_TEST );
    glEnable ( GL_TEXTURE_2D );
    glEnable ( GL_BLEND );
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); 
    glEnable ( GL_ALPHA_TEST );
    glAlphaFunc ( GL_GREATER, 0.01 );

    glEnableClientState ( GL_VERTEX_ARRAY );
    mvmat.loadIdentity ();

    initLC();
}

// ================================================================================

int r2d_load_scene ( const char* filename )
{
    LC* lc = new LC();
    if (lc->load ( filename ) )
    {
	globalLC.push_back ( lc );
	return ( globalLC.size() - 1 );
    }
    return -1;
}

// ================================================================================

void r2d_unload_scene ( int sceneid )
{
    if ( sceneid >= globalLC.size() )
	return;
    globalLC[sceneid]->free ();
    delete globalLC[sceneid];
    globalLC[sceneid] = NULL;
}

// ================================================================================

void r2d_update_scenes ( int* ids, int length )
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    if ( length == 0 )
	return;

    // set modelview matrix
    glLoadIdentity ();
    gluLookAt ( 0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
    glMultMatrixf ( mvmat.data ); 
    
    // calculate view-frustum
    double mv[16], pj[16], obj0[3], obj1[3];
    int vp[4];
    glGetDoublev ( GL_MODELVIEW_MATRIX, mv );
    glGetDoublev ( GL_PROJECTION_MATRIX, pj );
    glGetIntegerv ( GL_VIEWPORT, vp );
    gluUnProject ( 0, 0, 0, mv, pj, vp, &obj0[0], &obj0[1], &obj0[2] );
    gluUnProject ( vp[2], vp[3], 0, mv, pj, vp, &obj1[0], &obj1[1], &obj1[2] );
//    cout << "vp[0] = " <<  vp[0] << ", vp[1] = " << vp[1] << ", vp[2] = " <<  vp[2] << ", vp[3] = " << vp[3] << endl;
    float viewfrustum_minmax[4];
    viewfrustum_minmax[0] = obj0[0];
    viewfrustum_minmax[1] = obj0[1];
    viewfrustum_minmax[2] = obj1[0];
    viewfrustum_minmax[3] = obj1[1];

//     glBegin( GL_LINES );
//     glVertex2f ( obj0[0], obj0[1] );
//     glVertex2f ( obj1[0], obj1[1] );
//     glEnd ();

    for ( int i=0; i<length; i++ )
    {
	if ( ids[i]<0 || 
	     ids[i]>=globalLC.size() ||
	     globalLC[ids[i]] == 0 )
	    continue;
	
	clock_t t = clock();
	vfculler::clear ();
	StateSetBuilder::clear ();
	cout << "clean finished, elapse " << clock() - t << "(ms)" << endl;

	t = clock();
	LC* lc = globalLC[ids[i]];
	vfculler::cull ( *lc, viewfrustum_minmax );
	cout << "cull finished, elapse " << clock() - t << "(ms)" << endl;
	cout << "vfculler::renderObjects.size() = " << vfculler::renderObjects.size() << endl;

	t = clock();
	StateSetBuilder::build ( lc, vfculler::renderObjects );
	cout << "stateset build finished, elapse " << clock() - t << "(ms)" << endl;

	if ( vfculler::renderObjects.empty() == false ) {
	    t = clock();
	    Renderer::render ( StateSetBuilder::root, lc );
	    cout << "render finished, elapse " << clock() - t << "(ms)" << endl;
	}
	cout << "======================================== " << endl;

    }
}

// ================================================================================

void r2d_home ()
{
    mvmat.loadIdentity();
}

// ================================================================================

void r2d_translate ( float x, float y )
{
    mvmat *= mat4f::translate_matrix ( x, y, 0 );
}

// ================================================================================

void r2d_scale ( float s )
{
    mvmat *= mat4f::scale_matrix ( s, s, s );
}

// ================================================================================

void r2d_resize ( int width, int height )
{
    glViewport ( 0, 0, width, height );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity ();
    glOrtho ( -1, 1, -1, 1, 1, 100 );
//    gluPerspective ( 30.0, 1.0 * width / height, 0.1, 100.0 );
    glMatrixMode ( GL_MODELVIEW );
}

// ================================================================================

void r2d_get_viewport_rect ( float* x_y_width_height )
{
    double mv[16], pj[16], obj0[3], obj1[3];
    int vp[4];
    glGetDoublev ( GL_MODELVIEW_MATRIX, mv );
    glGetDoublev ( GL_PROJECTION_MATRIX, pj );
    glGetIntegerv ( GL_VIEWPORT, vp );
    gluUnProject ( 0, 0, 0, mv, pj, vp, &obj0[0], &obj0[1], &obj0[2] );
    gluUnProject ( vp[2], vp[3], 0, mv, pj, vp, &obj1[0], &obj1[1], &obj1[2] );
    
    x_y_width_height[0] = obj0[0];
    x_y_width_height[1] = obj0[1];
    x_y_width_height[2] = obj1[0] - obj0[0];
    x_y_width_height[3] = obj1[1] - obj0[1];
}

// ================================================================================

int r2d_get_layers ( int sceneID, int* layers )
{
    int layercnt = 0;
    if ( sceneID >=0 && sceneID<globalLC.size() )
    {
	LC* lc = globalLC[sceneID];
	if ( -1 != lc->toElement ( ROOT ) )
	{
	    if ( -1 != lc->toElement ( FIRST_CHILD ) )
	    {
		do
		{
		    if ( lc->getType () == SLC_LAYER )
		    {
			layers[layercnt] = lc->getGIndex();
			layercnt ++;
		    }
		} 
		while ( -1 != lc->toElement ( NEXT_SIBLING ) );
	    }
	}
    }
    return layercnt;
}

// ================================================================================

void r2d_layer_visible ( int sceneID, int layerid, bool isVisible )
{
    if ( sceneID >=0 && sceneID<globalLC.size() )
    {
	LC* lc = globalLC[sceneID];
	GlobalLCRecord& grcd = lc->globalLCEntry->LCRecords[layerid];
	if ( grcd.type == SLC_LAYER )
	{
	    LayerRecord& layer = lc->layerEntry->LCRecords[grcd.value];
	    layer.visible = isVisible;
	}
    }
}

// ================================================================================

void r2d_layer_foreground_color ( int sceneID, int layerid, unsigned short* rgb )
{
    if ( sceneID >=0 && sceneID<globalLC.size() )
    {
	LC* lc = globalLC[sceneID];
	GlobalLCRecord& grcd = lc->globalLCEntry->LCRecords[layerid];
	if ( grcd.type == SLC_LAYER )
	{
	    LayerRecord& layer = lc->layerEntry->LCRecords[grcd.value];
	    MaterialRecord& mat = lc->materialEntry->LCRecords[layer.materialIdx];
	    mat.foreground_color.xyz ( *rgb, *(rgb+1), *(rgb+2) );
	}
    }
}

// ================================================================================

void r2d_layer_background_color ( int sceneID, int layerid, unsigned short* rgb )
{
    if ( sceneID >=0 && sceneID<globalLC.size() )
    {
	LC* lc = globalLC[sceneID];
	GlobalLCRecord& grcd = lc->globalLCEntry->LCRecords[layerid];
	if ( grcd.type == SLC_LAYER )
	{
	    LayerRecord& layer = lc->layerEntry->LCRecords[grcd.value];
	    MaterialRecord& mat = lc->materialEntry->LCRecords[layer.materialIdx];
	    mat.background_color.xyz ( *rgb, *(rgb+1), *(rgb+2) );
	}
    }
}

// ================================================================================

const char* r2d_get_layer_name ( int sceneID, int layerid )
{
    if ( sceneID >=0 && sceneID<globalLC.size() )
    {
	LC* lc = globalLC[sceneID];
	GlobalLCRecord& grcd = lc->globalLCEntry->LCRecords[layerid];
	if ( grcd.type == SLC_LAYER )
	{
	    LayerRecord& layer = lc->layerEntry->LCRecords[grcd.value];
	    return layer.name;
	}
    }
    return NULL;
}

// ================================================================================

bool r2d_get_layer_visible ( int sceneID, int layerid )
{
    if ( sceneID >=0 && sceneID<globalLC.size() )
    {
	LC* lc = globalLC[sceneID];
	GlobalLCRecord& grcd = lc->globalLCEntry->LCRecords[layerid];
	if ( grcd.type == SLC_LAYER )
	{
	    LayerRecord& layer = lc->layerEntry->LCRecords[grcd.value];
	    return layer.visible;
	}
    }
    return false;
}

// ================================================================================

void r2d_get_layer_foreground_color ( int sceneID, int layerid, unsigned short* rgb )
{
    if ( sceneID >=0 && sceneID<globalLC.size() )
    {
	LC* lc = globalLC[sceneID];
	GlobalLCRecord& grcd = lc->globalLCEntry->LCRecords[layerid];
	if ( grcd.type == SLC_LAYER )
	{
	    LayerRecord& layer = lc->layerEntry->LCRecords[grcd.value];
	    MaterialRecord& mat = lc->materialEntry->LCRecords[layer.materialIdx];
	    rgb[0] = mat.foreground_color.x();
	    rgb[1] = mat.foreground_color.y();
	    rgb[2] = mat.foreground_color.z();
	}
    }
}

// ================================================================================

void r2d_get_layer_background_color ( int sceneID, int layerid, unsigned short* rgb )
{
    if ( sceneID >=0 && sceneID<globalLC.size() )
    {
	LC* lc = globalLC[sceneID];
	GlobalLCRecord& grcd = lc->globalLCEntry->LCRecords[layerid];
	if ( grcd.type == SLC_LAYER )
	{
	    LayerRecord& layer = lc->layerEntry->LCRecords[grcd.value];
	    MaterialRecord& mat = lc->materialEntry->LCRecords[layer.materialIdx];
	    rgb[0] = mat.background_color.x();
	    rgb[1] = mat.background_color.y();
	    rgb[2] = mat.background_color.z();
	}
    }
}

// ================================================================================

