#include "sgr_render2d.h"
#include "sgr_lc.h"
#include "sgr_vfculler.h"
#include "sgr_statesetbuilder.h"
#include "sgr_renderer.h"
#include "sgr_nodetypes.h"
#include "sgr_bboxupdater.h"
#include <vector>
#include <fstream>

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "mat4.h"
#include <ctime>

#include <stdlib.h>

extern LoadTex_Proc loadtex_proc;

using namespace std;

vector<LC*> globalLC;
mat4f mvmat;
float currentScale = 1;
//int gViewportMinMax[4];

void r2d_init ()
{
    glClearColor ( 0.0, 0.0, 0.0, 1.0 );
    glEnable ( GL_DEPTH_TEST );
    glEnable ( GL_TEXTURE_2D );
    glEnable ( GL_LINE_SMOOTH );

    glEnable ( GL_BLEND );
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); 

//     glEnable ( GL_ALPHA_TEST );
//     glAlphaFunc ( GL_GREATER, 0.01 );
    glEnableClientState ( GL_VERTEX_ARRAY );
    glEnableClientState ( GL_TEXTURE_COORD_ARRAY );
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

void r2d_get_scene_minmax ( int sceneID, float* minxy, float* maxxy )
{
    if ( sceneID >=0 && sceneID<globalLC.size() )
    {
	LC* lc = globalLC[sceneID];
	GlobalLCRecord& grcd = lc->globalLCEntry->LCRecords[0];
	minxy[0] = grcd.minmax[0];
	minxy[1] = grcd.minmax[1];
	maxxy[0] = grcd.minmax[2];
	maxxy[1] = grcd.minmax[3];
    }
}

// ================================================================================

void draw ( int* ids, int length, float* viewfrustum_minmax )
{
    for ( int i=0; i<length; i++ )
    {
	if ( ids[i]<0 || ids[i]>=globalLC.size() || globalLC[ids[i]] == 0 )
	    continue;
	
	clock_t t = clock();
	vfculler::clear ();
	StateSetBuilder::clear ();
	cout << "clean finished, elapse " << clock() - t << "(ms)" << endl;

	t = clock();
	LC* lc = globalLC[ids[i]];
	lc->toElement ( ROOT );
	vfculler::cull ( *lc, viewfrustum_minmax, currentScale );
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
	cout << endl;
    }
}

void draw2 ( int* ids, int length, float* viewfrustum_minmax )
{
    vector<StateSet*> opaques, transparents;
    int cleantime = 0;
    int culltime = 0;
    int cullobjcnt = 0;
    int ssbuildtime = 0;

    for ( int i=0; i<length; i++ )
    {
	if ( ids[i]<0 || ids[i]>=globalLC.size() || globalLC[ids[i]] == 0 )
	    continue;
	
	clock_t t = clock();
	vfculler::clear ();
	StateSetBuilder2::clear();
	cleantime += clock() - t;

	t = clock();
	LC* lc = globalLC[ids[i]];
	lc->toElement ( ROOT );
	vfculler::cull ( *lc, viewfrustum_minmax, currentScale );
	culltime += clock() - t;
	cullobjcnt += vfculler::renderObjects.size();

	t = clock();
	// collect opxaque objects & transparent objects
	StateSetBuilder2::build ( lc, vfculler::renderObjects, opaques, transparents );
	ssbuildtime += clock() - t;
    }

    cout << "cull finished, elapse " << culltime << "(ms), object count = " << cullobjcnt << endl;
    cout << "stateset build finished, elapse " << ssbuildtime << "(ms)" << endl;
    
    // render opaques
    clock_t t = clock();
    if ( opaques.empty() == false )
    {
	glEnable ( GL_DEPTH_TEST );
	for ( vector<StateSet*>::iterator pp=opaques.begin(); pp!=opaques.end(); ++pp )
	{
	    (*pp)->render();
	}
    }
    // render transparents
    if ( transparents.empty() == false )
    {
	glDisable ( GL_DEPTH_TEST );
	for ( vector<StateSet*>::iterator pp=transparents.begin(); pp!=transparents.end(); ++pp )
	{
	    (*pp)->render();
	}
    }
    cout << "render finished, elapse " << clock() - t << "(ms)" << endl;

    t = clock();
    for ( vector<StateSet*>::iterator pp=transparents.begin(); pp!=transparents.end(); ++pp )
	delete *pp;
    for ( vector<StateSet*>::iterator pp=opaques.begin(); pp!=opaques.end(); ++pp )
	delete *pp;
    cout << "clean finished, elapse " << cleantime + clock() - t<< "(ms)" << endl;
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
    float viewfrustum_minmax[4];
    r2d_get_viewport_rect ( viewfrustum_minmax );
    viewfrustum_minmax[2] += viewfrustum_minmax[0];
    viewfrustum_minmax[3] += viewfrustum_minmax[1];
    
    draw2 ( ids, length, viewfrustum_minmax );
}

// ================================================================================

void r2d_get_scene_position ( int x, int y, float& scenex, float& sceney )
{
    double mv[16], pj[16], obj0[3];
    int vp[4];
    glGetDoublev ( GL_MODELVIEW_MATRIX, mv );
    glGetDoublev ( GL_PROJECTION_MATRIX, pj );
    glGetIntegerv ( GL_VIEWPORT, vp );
    gluUnProject ( x, vp[3]-y, 0, mv, pj, vp, &obj0[0], &obj0[1], &obj0[2] );
    
    scenex = obj0[0];
    sceney = obj0[1];
}
// void r2d_home ()
// {
//     mvmat.loadIdentity();
// }

// ================================================================================

void r2d_translate ( float x, float y )
{
    mvmat *= mat4f::translate_matrix ( x, y, 0 );
}

// ================================================================================

void r2d_scale ( float s )
{
    mvmat *= mat4f::scale_matrix ( s, s, s );
    currentScale = s;
}

// ================================================================================

void r2d_loadidentity ()
{
    mvmat.loadIdentity();
    currentScale = 1;
}

// ================================================================================

void r2d_resize ( int width, int height )
{
//     float ratio = 4.0f / 3.0f;
//     if ( (height * ratio) > width ) {
//  	glViewport ( 0.0, 0.0, height * ratio, height );
// //	glViewport ( -(height*ratio-width)/2.0, 0, height * ratio, height );
// // 	gViewportMinMax[0] = (height*ratio-width)/2.0;
// // 	gViewportMinMax[1] = 0;
// // 	gViewportMinMax[2] = (height*ratio+width)/2.0;
// // 	gViewportMinMax[3] = height;
//     } else {
//  	glViewport ( 0, 0.0, width, width / ratio );
// //  	glViewport ( 0, -(width/ratio-height)/2.0, width, width / ratio );
// // 	gViewportMinMax[0] = 0;
// // 	gViewportMinMax[1] = (width/ratio-height)/2.0;
// // 	gViewportMinMax[2] = width;
// // 	gViewportMinMax[3] = (width/ratio+height)/2.0;
//     }
    float ratio = 1.0f * height / width;
    glViewport ( 0, 0, width, height );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity ();
    glOrtho ( -1, 1, -ratio, ratio, 1, 100 );
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
//     gluUnProject ( gViewportMinMax[0], gViewportMinMax[1], 0, mv, pj, vp, &obj0[0], &obj0[1], &obj0[2] );
//     gluUnProject ( gViewportMinMax[2], gViewportMinMax[3], 0, mv, pj, vp, &obj1[0], &obj1[1], &obj1[2] );
    
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

void r2d_highlight_primitive ( int sceneID, int primitiveID, bool highlight )
{
    if ( sceneID >=0 && sceneID<globalLC.size() )
    {
	LC* lc = globalLC[sceneID];
	GlobalLCRecord& grecord = lc->globalLCEntry->LCRecords[primitiveID];
        DrawableRecord* drawable;
        switch ( grecord.type )
        {
        case SLC_SMARTILES:
            drawable = &(lc->smartTilesEntry->LCRecords[grecord.value]);
             break;
        case SLC_PLINE:
            drawable = &(lc->plineEntry->LCRecords[grecord.value]);
            break;
        case SLC_POLY:
            drawable = &(lc->polyEntry->LCRecords[grecord.value]);
            break;
        case SLC_RECT:
            drawable = &(lc->rectEntry->LCRecords[grecord.value]);
            break;
        case SLC_TEXT:
            drawable = &(lc->textEntry->LCRecords[grecord.value]);
            break;
        default:
            drawable = 0;
            break;
        }
        if ( 0 == drawable )
            return;

        MaterialRecord& mat = lc->materialEntry->LCRecords[drawable->materialIdx];
        int halflen = lc->materialEntry->LCLen / 2;
        bool isAlreadyHighlight = (drawable->materialIdx >= halflen);
        if ( true == highlight ) {
            // check if already highlighted
            // if so: do nothing
            // else: insert new record to hightlightRecoreds
            if ( false == isAlreadyHighlight )
            {
                MaterialRecord& highlightmat = lc->materialEntry->LCRecords[drawable->materialIdx+halflen];
                highlightmat.background_color = vec4i ( 0, 0, 255, 255 );
                drawable->materialIdx += halflen;
            }
        } else {
            // check if already non-highlighted
            // if so: do nothing
            // else: remove correspond record in hightlightRecoreds & restore old materialIdx
            if ( true == isAlreadyHighlight )
            {
                drawable->materialIdx -= halflen;
            }
        }
    }    
}

// ================================================================================

int r2d_to_element ( int sceneID, int elementType )
{
    LC* lc = globalLC[sceneID];
    lc->toElement ( elementType );
    return lc->getGIndex ();
}

int r2d_get_node_type ( int sceneID )
{
    LC* lc = globalLC[sceneID];
    return lc->getType();
}

void r2d_rect_points ( int sceneID, int rectid, float* xyz )
{
    LC* lc = globalLC[sceneID];

    GlobalLCRecord& grcd = lc->globalLCEntry->LCRecords[rectid];
    RectRecord& rrcd = lc->rectEntry->LCRecords[ grcd.value ];
    rrcd.data[0].xyz ( xyz[0], xyz[1], xyz[2] );
    rrcd.data[1].xyz ( xyz[3], xyz[1], xyz[2] );
    rrcd.data[2].xyz ( xyz[3], xyz[4], xyz[5] );
    rrcd.data[3].xyz ( xyz[0], xyz[4], xyz[5] );

    // should be lazy update, but need LC support arbitery cursor
    BBox2dUpdater::forward_update ( *lc );
}

void r2d_loadtexture_callback ( LoadTex_Proc proc )
{
    loadtex_proc = proc;
//    cout << "loadtex_proc = " << loadtex_proc << endl;
}

int* pickedlist = 0;
int r2d_crosspick ( int sceneID, float* minmax, int** outIds )
{
    if ( sceneID >=0 && sceneID<globalLC.size() )
    {
        if ( pickedlist != 0 ) {
            free ( pickedlist );
            pickedlist = 0;
        }
        
	vfculler::clear ();
	LC* lc = globalLC[sceneID];
	lc->toElement ( ROOT );
	vfculler::cull ( *lc, minmax, currentScale, true );
        if ( vfculler::renderObjects.empty() == true )
            return 0;

        pickedlist = (int*)calloc ( vfculler::renderObjects.size(), sizeof(int) );
        memcpy ( pickedlist, &(vfculler::renderObjects[0]), vfculler::renderObjects.size()*sizeof(int) );
        *outIds = pickedlist;
        return vfculler::renderObjects.size();
    }
    *outIds = 0;
    return 0;
}

int r2d_containpick ( int sceneID, float* minmax, int** outIds )
{
    if ( sceneID >=0 && sceneID<globalLC.size() )
    {
        if ( pickedlist != 0 ) {
            free ( pickedlist );
            pickedlist = 0;
        }

	vfculler::clear ();
	LC* lc = globalLC[sceneID];
	lc->toElement ( ROOT );
	vfculler::cull2 ( *lc, minmax, currentScale, true );
        if ( vfculler::renderObjects.empty() == true )
            return 0;

        pickedlist = (int*)calloc ( vfculler::renderObjects.size(), sizeof(int) );
        memcpy ( pickedlist, &(vfculler::renderObjects[0]), vfculler::renderObjects.size()*sizeof(int) );
        *outIds = pickedlist;
        return vfculler::renderObjects.size();
    }
    *outIds = 0;
    return 0;
}

