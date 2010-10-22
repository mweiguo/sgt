#ifndef _INTERFACE_H
#define _INTERFACE_H

// for every create function, the id rules:
// 0          :          root
// -1         :          NULL
// <-1        :          internal usage
// [1, 99]    :          camera
// [100, 199] :          viewport
// 200+       :          objects
#include "sgr_global.h"
extern "C"
{
    const int NODETYPE_UNKNOWN     	  = 0 ;
    const int NODETYPE_CAMERA     	  = 1 ;
    const int NODETYPE_VIEWPORT   	  = 2 ;
    const int NODETYPE_MESH       	  = 3 ;
    const int NODETYPE_MESHLINE   	  = 4 ;
    const int NODETYPE_MESHPOINT  	  = 5 ;
    const int NODETYPE_COLOR      	  = 6 ;
    const int NODETYPE_ATTRSET    	  = 7 ;
    const int NODETYPE_SCENE      	  = 8 ;
    const int NODETYPE_LAYER      	  = 9 ;
    const int NODETYPE_LOD        	  = 10;
    const int NODETYPE_KDTREE     	  = 11;
    const int NODETYPE_ARRAY      	  = 12;
    const int NODETYPE_RECTANGLE      = 13;
    const int NODETYPE_TRANSFORM      = 14;
    const int NODETYPE_PICKABLEGROUP  = 15;
    const int NODETYPE_SWITCH         = 16;
    const int NODETYPE_GROUP          = 17;
    const int NODETYPE_TEXT           = 18;
    const int NODETYPE_FONT           = 19;
    const int NODETYPE_LINE           = 20;
    const int NODETYPE_POLYLINE       = 21;
    const int NODETYPE_POLY           = 22;
    const int NODETYPE_POINT          = 23;

    const int RENDERER_SYSTEM         = 0;
    const int CAD_SYSTEM              = 1;

    const int DRAWLINE_TOOL           = 1;
    /*********************************/
    /******** TCL COMMANDS REG *******/
    /*********************************/
    struct Tcl_Interp;
    void SGR_DLL register_tclcmds ( Tcl_Interp* interp );

    /*********************************/
    /********** WINDOW INIT **********/
    /*********************************/
    //void SGR_DLL init_windowsystem ();
    //void SGR_DLL system_mode ( int );

    /*********************************/
    /********** CAD  SYSTEM **********/
    /*********************************/
    void SGR_DLL node_translate ( int id, float tx, float ty, float tz );
    void SGR_DLL node_scale ( int id, float scale );
    

    /*********************************/
    /******* CAMERA MANAGEMENT *******/
    /*********************************/
    void SGR_DLL camera_create ( int id, const char* name );
    void SGR_DLL camera_translate ( int id, float tx, float ty, float tz );
    void SGR_DLL camera_scale ( int id, float scale );
    void SGR_DLL camera_reset ( int id );
    void SGR_DLL camera_name ( int id, const char* name );
    float SGR_DLL find_view ( float* min, float* max, float percentOfView, int camid, int vpid );

    /*********************************/
    /****** VIEWPORT MANAGEMENT ******/
    /*********************************/
    void SGR_DLL viewport_create ( int id, const char* name );
    void SGR_DLL viewport_geometry ( int id, int x, int y, int w, int h );
    void SGR_DLL viewport_attachcamera ( int id, int camid );
    void SGR_DLL viewport_name ( int id, const char* name );
    void SGR_DLL viewport_dirty ( int id );
    class QPainter;
    void SGR_DLL viewport_update ( int id, QPainter& painter );

    ///*********************************/
    ///************* WINDOW ************/
    ///*********************************/
    //void SGR_DLL viewport_add ( int id );
    //void SGR_DLL viewport_delete ( int id );
    //void SGR_DLL sgwindow_update ( int id );

    /*********************************/
    /************** IO ***************/
    /*********************************/
    // local scene load
    int SGR_DLL scene_load ( const char* file, int* data );
    int SGR_DLL node_load ( const char* file, int* data );
    void SGR_DLL node_save ( const char* file, int sceneid );
    void SGR_DLL unload_node (int id);
    void SGR_DLL scene_translate ( int id, float tx, float ty, float tz );
    void SGR_DLL scene_scale ( int id, float scale );

    /*********************************/
    /*********** diagnose ************/
    /*********************************/
    void SGR_DLL dump_node ( int id, const char* filename );

    /*********************************/
    /************ QUERY **************/
    /*********************************/
    void SGR_DLL get_bbox ( int id, float* min, float* max );
    void SGR_DLL get_scenepos ( int vpid, float* viewportCoord3f, float* sceneCoord3f, int camid=-1/*default value means use camid attached viewport */ );
    void SGR_DLL get_viewportpos ( int vpid, float* sceneCoord3f, float* viewportCoord3f, int camid=-1/*default value means use camid attached viewport */ );
    int SGR_DLL get_nodetype ( int node );
    int SGR_DLL get_nodeparent ( int node );
    void SGR_DLL get_nodechildren ( int node, int* data );
    int SGR_DLL get_nodechildrenN ( int node );
    int SGR_DLL get_specnodechildren ( int node, int nodetype, int* data );

    /*********************************/
    /************* PICK **************/
    /*********************************/
    // since the system use LOD, and in different level, objects in the scene
    // may be different, so here need parameter camid.
    // when function return. data store pick ids, returning result is the size of ids
    int SGR_DLL pick ( float x, float y, float z, int camid, int* data );
    int SGR_DLL pick_volume ( float minx, float miny, float minz, float maxx, float maxy, float maxz, int camid, int* data );
    int SGR_DLL box_pick ( float minx, float miny, float minz, float maxx, float maxy, float maxz, int camid, int* data );
    int SGR_DLL cross_pick ( float minx, float miny, float minz, float maxx, float maxy, float maxz, int camid, int* data );

    /*********************************/
    /********** build nodes **********/
    /*********************************/
    // remote scene load, C/S architecture, client should maintain node structure
    void SGR_DLL add_child ( int parent, int child );
    void SGR_DLL pushfront_child ( int parent, int child );
    void SGR_DLL remove_child ( int preant, int child );
    void SGR_DLL remove_parent ( int nodeid );
    void SGR_DLL clear_child ( int id );
    void SGR_DLL node_delete ( int id );
    void SGR_DLL node_visible ( int id, bool isVisible );
    void SGR_DLL set_userdata ( int id, void* data );
    void SGR_DLL get_userdata ( int id, void** data );
    void SGR_DLL update_bbox ( int id );

    // mesh
    void SGR_DLL mesh_create ( int id );
    void SGR_DLL mesh_coords ( int meshid, float* coords3d, int elementN );
    void SGR_DLL mesh_subcoords ( int meshid, int* indexes, int elementN, float* coords3d );
    int SGR_DLL mesh_appendcoords ( int id, float* coords3d, int elementN );
    int SGR_DLL mesh_appendcoord ( int id, float x, float y, float z );
    void SGR_DLL get_mesh_coord ( int id, int idx, float& x, float& y, float& z );

    // mesh line
    void SGR_DLL meshline_create ( int id );
    void SGR_DLL meshline_type ( int id, int type );
    void SGR_DLL meshline_data ( int id, int* data, int elementN );
    void SGR_DLL meshline_subdata ( int id, int* indexes, int elementN, int* data );
    void SGR_DLL meshline_appendpoint ( int id, int coordIdx );
    int SGR_DLL meshline_pointsN ( int id );
    int SGR_DLL get_meshline_coordindexes ( int id, int* coordIdxes );

    // mesh point
    void SGR_DLL meshpoint_create ( int id, int coordidx );
    void SGR_DLL meshpoint_coordidx ( int id, int coordidx );
    void SGR_DLL meshpoint_size ( int id, float sz );
    int SGR_DLL get_meshpoint_coordindex ( int id );

    // colors
    void SGR_DLL color_create ( int id, unsigned int color );
    void SGR_DLL color_rgbas ( int id, const char* color );                        // color format :  #rrggbbaa
    void SGR_DLL color_rgbai ( int id, unsigned int color );

    // attrset
    void SGR_DLL attrset_create ( int id, int layerid=0 );
    void SGR_DLL attrset_create_byrenderorder ( int id, int layerid );
    void SGR_DLL attrset_fgcolor ( int id, int colorid );
    void SGR_DLL attrset_bgcolor ( int id, int colorid );
    void SGR_DLL attrset_font ( int id, int fontid );
    void SGR_DLL set_attrset ( int nodeid, int attrsetid );
    void SGR_DLL unset_attrset ( int nodeid );
    int SGR_DLL attrset_refcnt ( int nodeid );
    int SGR_DLL get_attrset ( int attrsetid );
    unsigned int SGR_DLL get_attrset_fgcolor ( int attrsetid );
    unsigned int SGR_DLL get_attrset_bgcolor ( int attrsetid );
    int SGR_DLL get_attrset_font ( int attrsetid );



    //// color access
    //void SGR_DLL set_bordercolori ( int id, unsigned int color, bool isByGroup/*=true*/ );
    //void SGR_DLL set_fillcolori ( int id, unsigned int color, bool isByGroup );
    //void SGR_DLL set_fgcolori ( int id, unsigned int color );
    //void SGR_DLL set_bgcolori ( int id, unsigned int color );
    //void SGR_DLL set_bordercolors ( int id, const char* color, bool isByGroup );
    //void SGR_DLL set_fillcolors ( int id, const char* color, bool isByGroup );
    // scene
    void SGR_DLL scene_create ( int id );
    // layer
    void SGR_DLL layer_create ( int id, const char* name="" );
    void SGR_DLL layer_name ( int id, const char* name );
    void SGR_DLL layer_visible ( int id, bool isVisible );
    void SGR_DLL layer_fgcolor ( int id, int color );
    void SGR_DLL layer_bgcolor ( int id, int color );
    void SGR_DLL layer_font ( int id, const char* family, int size, int style, int weight, const char* name );
    // lod
    void SGR_DLL lod_create ( int id );
    void SGR_DLL lod_delimiters ( int id, const char* delims );
    // kdtree
    void SGR_DLL kdtreenode_create ( int id );
    void SGR_DLL kdtreenode_build ( int id );
    // array
    void SGR_DLL array_create ( int id );
    void SGR_DLL array_rowcnt ( int id, int cnt );
    void SGR_DLL array_columncnt ( int id, int cnt );
    void SGR_DLL array_hlevelcnt ( int id, int level, int cnt );
    void SGR_DLL array_vlevelcnt ( int id, int level, int cnt );
    void SGR_DLL array_marginx ( int id, int level, float space );
    void SGR_DLL array_marginy ( int id, int level, float space );
    void SGR_DLL expand_arraynode ( int id );
    // rectangle
    void SGR_DLL rectangle_create ( int id );
    void SGR_DLL rectangle_size ( int id, float w, float h );
    // transform
    void SGR_DLL transform_create ( int id );
    void SGR_DLL transform_translate3f ( int id, float tx, float ty, float tz );
    void SGR_DLL transform_translates ( int id, const char* str );
    //void SGR_DLL transform_scale ( int id, float sx, float sy, float sz );
    void SGR_DLL transform_scale ( int id, const char* str );
    // pickablegroup
    void SGR_DLL pickablegroup_create ( int id, const char* name="" );
    void SGR_DLL pickablegroup_name ( int id, const char* name );
    // switchnode
    void SGR_DLL switchnode_create ( int id );
    void SGR_DLL switchnode_visible ( int id, bool isVisible );
    void SGR_DLL switchnode_selection ( int id, int idx );
    // groupnode
    void SGR_DLL groupnode_create ( int id, const char* name="" );
    void SGR_DLL groupnode_name ( int id, const char* name );
    // text
    void SGR_DLL text_create ( int id, const char* str= "" );
    void SGR_DLL text_string ( int id, const char* str );
    //void SGR_DLL text_font ( int id, int fontid );
    //  1     2     3
    //  4     5     6
    //  7     8     9
    void SGR_DLL text_anchor ( int id, int anchor );
    //  1     2     3
    //  4     5     6
    //  7     8     9
    void SGR_DLL text_justify ( int id, int justify );
    // font

    void SGR_DLL font_create ( int id, const char* desc="" );
    void SGR_DLL font_family ( int id, const char* f );
    void SGR_DLL font_size ( int id, int size );
    /*NORMAL = 1, ITALIC = 3, UNDERSTRIKE = 4*/
    void SGR_DLL font_style ( int id, int style );
    void SGR_DLL font_weight ( int id, int weight );
    void SGR_DLL font_name ( int id, const char* name );
    void SGR_DLL get_font_desc ( int fontid, char* buffer );
    //void font_color ( int id, const char* color );

    // line
    void SGR_DLL line_create ( int id );
    void SGR_DLL line_points ( int id, float x1, float y1, float x2, float y2 );
    void SGR_DLL line_point ( int id, float x, float y, bool isFirst );
    void SGR_DLL get_line_point ( int id, float* xy, bool isFirst );

    // polyline
    void SGR_DLL polyline_create ( int id );
    void SGR_DLL polyline_points ( int id, float* xycoords, int pointcnt );

    // polygon
    void SGR_DLL poly_create ( int id );
    void SGR_DLL poly_points ( int id, float* xycoords, int pointcnt );

    // polygon
    void SGR_DLL point_create ( int id );
    void SGR_DLL point_coord ( int id, float x, float y );
    void SGR_DLL point_size ( int id, float sz );
    float SGR_DLL get_point_size ( int id );
    void SGR_DLL get_point_coord ( int id, float* xyz );
}
#endif
