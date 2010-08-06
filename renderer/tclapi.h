#ifndef _TCLAPI_H_
#define _TCLAPI_H_


/*********************************/
/******* CAMERA MANAGEMENT *******/
/*********************************/
// camera_create cameraname
// camera_translate id tx ty tz
// camera_scale id scale
// camera_reset id
// camera_name id name
// find_view nodeid percentOfView camid vpid

/*********************************/
/****** VIEWPORT MANAGEMENT ******/
/*********************************/
// viewport_create name
// viewport_geometry id x y w h
// viewport_attachcamera id camid
// viewport_name id name
// viewport_dirty id
// viewport_add id
// viewport_delete id
// update id

/*********************************/
/************* WINDOW ************/
/*********************************/
// viewport_add     vpid
// viewport_delete  vpid
// sgwindow_update  vpid

/*********************************/
/************** IO ***************/
/*********************************/
// scene_load file
// scene_save file meshid
// mesh_unload meshid

/*********************************/
/*********** diagnose ************/
/*********************************/
// dump_node nodeid filename

/*********************************/
/************ QUERY **************/
/*********************************/
//{minx miny minz maxx maxy maxz} get_bbox id

/*********************************/
/************* PICK **************/
/*********************************/
//{id1 id2 id3} pick x y z camid
//{id1 id2 id3} pick_volume minx miny minz maxx maxy maxz camid

/*********************************/
/********** build nodes **********/
/*********************************/
// add_child parent child
// remove_child parent child
// clear_child parent
// node_delete id
// node_visible id isVisible

// update_bbox id
// set_fillcolori id color
// set_bordercolori id color

// color_create  color
// color_rgbas  id  color
// color_rgbai  id  color

// attrset_create   layerid
// attrset_fgcolor  id  colorid
// attrset_bgcolor  id  colorid
// set_attrset      id  attrsetid
// unset_attrset    id

// scene_create

// layer_create name
// layer_name id name
// layer_name id isvisible

// lod_create

// lod_delimiters id delims

// kdtree_create

// array_create
// array_rowcnt id cnt
// array_columncnt id cnt
// array_hlevelcnt id level cnt
// array_vlevelcnt id level cnt
// array_marginx id level space
// array_marginy id level space

// rectangle_create
// rectangle_size id w h

// transform_create
// transform_translate id tx ty tz
// transform_scale id sx sy sz

// pickablegroup_create
// pickablegroup_name id name

// switchnode_create
// switchnode_props id isvisible

// groupnode_create
// groupnode_props id name

// text_create
// text_string id name
// text_font id fontid
// text_anchor id anchor
// text_justify id justify

// font_create
// font_family id family
// font_size id size
// font_style id style

// line_create
// line_points id x1 y1 x2 y2
// line_point id x y isFirstPoint

// marshalling all the api, is a annoy work, is there any trick to avoid this?
#include <tcl.h>

int tcl_camera_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_camera_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_camera_translate ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_camera_scale ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_camera_reset ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_camera_name ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_find_view ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// viewport management
int tcl_viewport_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_viewport_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_viewport_geometry ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_viewport_attachcamera ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_viewport_name ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_viewport_dirty ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_viewport_add ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_viewport_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_update ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// local mesh load, treat mesh as a single object
int tcl_scene_load ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_scene_save ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_mesh_unload (ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// diagnose
int tcl_dump_node (ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// query
int tcl_get_bbox (ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_get_scenepos (ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// pick
int tcl_pick (ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_pick_volume (ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// remote mesh load, C/S architecture, client should maintian node structure
int tcl_add_child ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_remove_child ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_clear_child ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_node_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_node_visible ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_update_bbox ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_set_fillcolori ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_set_bordercolori ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );


int tcl_color_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_color_rgbas ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_color_rgbai ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

int tcl_attrset_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_attrset_fgcolor ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_attrset_bgcolor ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_set_attrset ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_unset_attrset ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// mesh
int tcl_scene_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_mesh_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// layer
int tcl_layer_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_layer_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_layer_name ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_layer_visible ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// lod
int tcl_lod_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_lod_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_lod_delimiters ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// kdtree
int tcl_kdtree_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_kdtree_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// array
int tcl_array_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_array_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_array_rowcnt ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_array_columncnt ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_array_hlevelcnt ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_array_vlevelcnt ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_array_marginx ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_array_marginy ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// rectangle
int tcl_rectangle_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_rectangle_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_rectangle_size ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// transform
int tcl_transform_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_transform_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_transform_translate ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_transform_translate ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_transform_scale ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_transform_scale ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// pickablegroup
int tcl_pickablegroup_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_pickablegroup_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_pickablegroup_name ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// switchnode
int tcl_switchnode_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_switchnode_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_switchnode_props ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// groupnode
int tcl_groupnode_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_groupnode_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_groupnode_props ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// text
int tcl_text_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_text_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_text_string ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_text_font ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_text_anchor ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_text_justify ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// font
int tcl_font_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_font_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_font_family ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_font_size ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
/*PLAIN = 1, BOLD = 2, ITALIC = 3, BOLDITALIC = 4*/
int tcl_font_style ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

// line
int tcl_line_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_line_points ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );
int tcl_line_point ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] );

#endif //_TCLAPI_H_
