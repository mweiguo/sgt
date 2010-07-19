#include "tclapi.h"
#include "interface.h"
#include <stdexcept>
#include <sstream>
using namespace std;

int tcl_camera_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try 
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "camera_create cameraname" );
            return TCL_ERROR;
        }

        int len;
        const char* camname = Tcl_GetStringFromObj ( objv[1], &len);
        int id = camera_create ( camname );
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_camera_translate ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try 
    {
        if ( objc != 5 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "camera_translate id tx ty tz" );
            return TCL_ERROR;
        }

        int id;
        double tx, ty, tz;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetDoubleFromObj ( interp, objv[2], &tx );
        Tcl_GetDoubleFromObj ( interp, objv[3], &ty );
        Tcl_GetDoubleFromObj ( interp, objv[4], &tz );

        camera_translate ( id, tx, ty, tz );
        return TCL_OK;
    } catch ( std::exception& e ) {
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_camera_scale ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "camera_scale id scale" );
            return TCL_ERROR;
        }

        int id;
        double scale;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetDoubleFromObj ( interp, objv[2], &scale );

        camera_scale ( id, scale );
        return TCL_OK;
    } catch ( std::exception& e ) {
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_camera_reset ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "camera_reset id" );
            return TCL_ERROR;
        }

        int id;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 

        camera_reset ( id );
        return TCL_OK;
    } catch ( std::exception& e ) {
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_camera_name ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "camera_name id name" );
            return TCL_ERROR;
        }

        int id, len;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        const char* camname = Tcl_GetStringFromObj ( objv[2], &len );

        camera_name ( id, camname );
        return TCL_OK;
    } catch ( std::exception& e ) {
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_find_view ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 10 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "find_view minx miny minz maxx maxy maxz percentOfView camid vpid" );
            return TCL_ERROR;
        }

        int camid, vpid;
        double percentOfView, tmp;
        float min[3], max[3];
        Tcl_GetDoubleFromObj ( interp, objv[1], &tmp );
        min[0] = tmp;
        Tcl_GetDoubleFromObj ( interp, objv[2], &tmp );
        min[1] = tmp;
        Tcl_GetDoubleFromObj ( interp, objv[3], &tmp );
        min[2] = tmp;
        Tcl_GetDoubleFromObj ( interp, objv[4], &tmp );
        max[0] = tmp;
        Tcl_GetDoubleFromObj ( interp, objv[5], &tmp );
        max[1] = tmp;
        Tcl_GetDoubleFromObj ( interp, objv[6], &tmp );
        max[2] = tmp;
        Tcl_GetDoubleFromObj ( interp, objv[7], &percentOfView );
        Tcl_GetIntFromObj ( interp, objv[8], &camid ); 
        Tcl_GetIntFromObj ( interp, objv[9], &vpid ); 

        float scale = find_view ( min, max, percentOfView, camid, vpid );
        Tcl_Obj* rstobj = Tcl_NewDoubleObj(scale);
        Tcl_SetObjResult ( interp, rstobj );

        return TCL_OK;
    } catch ( std::exception& e ) {
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}


// viewport management
int tcl_viewport_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "viewport_create name" );
            return TCL_ERROR;
        }

        int len;
        const char* vpname = Tcl_GetStringFromObj ( objv[1], &len );

        int id = viewport_create ( vpname );
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_viewport_geometry ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 6 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "viewport_geometry id x y w h" );
            return TCL_ERROR;
        }

        int id;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        double x, y, w, h;
        Tcl_GetDoubleFromObj ( interp, objv[2], &x );
        Tcl_GetDoubleFromObj ( interp, objv[3], &y );
        Tcl_GetDoubleFromObj ( interp, objv[4], &w );
        Tcl_GetDoubleFromObj ( interp, objv[5], &h );

        viewport_geometry ( id, x, y, w, h );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_viewport_attachcamera ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "viewport_attachcamera id camid" );
            return TCL_ERROR;
        }

        int id, camid;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetIntFromObj ( interp, objv[2], &camid ); 

        viewport_attachcamera ( id, camid );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_viewport_name ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "viewport_name id name" );
            return TCL_ERROR;
        }

        int id, len;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        const char* name = Tcl_GetStringFromObj ( objv[2], &len);
        viewport_name ( id, name );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_viewport_dirty ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "viewport_dirty id" );
            return TCL_ERROR;
        }

        int id;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        viewport_dirty ( id );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}


int tcl_viewport_add ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "viewport_add id" );
            return TCL_ERROR;
        }

        int id;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        viewport_add ( id );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_viewport_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "viewport_delete id" );
            return TCL_ERROR;
        }

        int id;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        viewport_delete ( id );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_update ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "update id" );
            return TCL_ERROR;
        }

        int id;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        sgwindow_update ( id );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}


// local mesh load, treat mesh as a single object
int tcl_mesh_load ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "mesh_load file" );
            return TCL_ERROR;
        }

        int len;
        const char* file = Tcl_GetStringFromObj ( objv[1], &len );
        int id = mesh_load ( file );
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_mesh_save ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "mesh_save file meshid" );
            return TCL_ERROR;
        }

        int len, id;
        const char* file = Tcl_GetStringFromObj ( objv[1], &len );
        Tcl_GetIntFromObj ( interp, objv[2], &id );
        mesh_save ( file, id );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_mesh_unload (ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "mesh_unload meshid" );
            return TCL_ERROR;
        }

        int id;
        Tcl_GetIntFromObj ( interp, objv[1], &id );
        unload_node ( id );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_dump_node (ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "dump_node nodeid filename" );
            return TCL_ERROR;
        }

        int id, len;
        Tcl_GetIntFromObj ( interp, objv[1], &id );
        const char* filename = Tcl_GetStringFromObj ( objv[2], &len);
        dump_node ( id, filename );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_get_bbox (ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "get_bbox nodeid" );
            return TCL_ERROR;
        }

        int id, len;
        Tcl_GetIntFromObj ( interp, objv[1], &id );
        float min[3], max[3];
        get_bbox ( id, min, max );
        
        Tcl_Obj *list[6];
        list[0] = Tcl_NewDoubleObj (min[0]);
        list[1] = Tcl_NewDoubleObj (min[1]);
        list[2] = Tcl_NewDoubleObj (min[2]);
        list[3] = Tcl_NewDoubleObj (max[0]);
        list[4] = Tcl_NewDoubleObj (max[1]);
        list[5] = Tcl_NewDoubleObj (max[2]);

        Tcl_SetObjResult ( interp, Tcl_NewListObj(6, list) );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_get_scenepos (ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 5 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "get_scenepos viewportid vx vy vz" );
            return TCL_ERROR;
        }

        int vpid;
        Tcl_GetIntFromObj ( interp, objv[1], &vpid );
        double viewcoord[3];
        Tcl_GetDoubleFromObj ( interp, objv[2], &viewcoord[0] );
        Tcl_GetDoubleFromObj ( interp, objv[3], &viewcoord[1] );
        Tcl_GetDoubleFromObj ( interp, objv[4], &viewcoord[2] );
        
        float vcoord[3], scenecoord[3];
        vcoord[0] = viewcoord[0];
        vcoord[1] = viewcoord[1];
        vcoord[2] = viewcoord[2];
        get_scenepos ( vpid, vcoord, scenecoord );
        
        Tcl_Obj *list[3];
        list[0] = Tcl_NewDoubleObj (scenecoord[0]);
        list[1] = Tcl_NewDoubleObj (scenecoord[1]);
        list[2] = Tcl_NewDoubleObj (scenecoord[2]);

        Tcl_SetObjResult ( interp, Tcl_NewListObj(3, list) );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_pick (ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 5 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "pick x y z camid" );
            return TCL_ERROR;
        }

        double scenecoord[3];
        Tcl_GetDoubleFromObj ( interp, objv[1], &scenecoord[0] );
        Tcl_GetDoubleFromObj ( interp, objv[2], &scenecoord[1] );
        Tcl_GetDoubleFromObj ( interp, objv[3], &scenecoord[2] );
        int camid;
        Tcl_GetIntFromObj ( interp, objv[4], &camid );

        int data[256];
        float x = scenecoord[0];
        float y = scenecoord[1];
        float z = scenecoord[2];
        
        int size = pick ( scenecoord[0], scenecoord[1], scenecoord[2], camid, data );
        
        Tcl_Obj *list[256];
        for ( int i=0; i<size; i++ )
            list[i] = Tcl_NewIntObj ( data[i] );
        Tcl_SetObjResult ( interp, Tcl_NewListObj(size, list) );

        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_pick_volume (ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 8 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "pick_volume minx miny minz minx miny minz camid" );
            return TCL_ERROR;
        }

        double min[3], max[3];
        Tcl_GetDoubleFromObj ( interp, objv[1], &min[0] );
        Tcl_GetDoubleFromObj ( interp, objv[2], &min[1] );
        Tcl_GetDoubleFromObj ( interp, objv[3], &min[2] );

        Tcl_GetDoubleFromObj ( interp, objv[4], &max[0] );
        Tcl_GetDoubleFromObj ( interp, objv[5], &max[1] );
        Tcl_GetDoubleFromObj ( interp, objv[6], &max[2] );

        int camid;
        Tcl_GetIntFromObj ( interp, objv[7], &camid );

        int data[256];
        int size = pick_volume ( min[0], min[1], min[2], max[0], max[1], max[2], camid, data ); 

        Tcl_Obj *list[256];
        for ( int i=0; i<size; i++ )
            list[i] = Tcl_NewIntObj ( data[i] );
        Tcl_SetObjResult ( interp, Tcl_NewListObj(size, list) );

        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

// remote mesh load, C/S architecture, client should maintian node structure
int tcl_add_child ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "add_child parent child" );
            return TCL_ERROR;
        }

        int parent, child;
        Tcl_GetIntFromObj ( interp, objv[1], &parent );
        Tcl_GetIntFromObj ( interp, objv[2], &child );
        add_child ( parent, child );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_remove_child ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "remove_child parent child" );
            return TCL_ERROR;
        }

        int parent, child;
        Tcl_GetIntFromObj ( interp, objv[1], &parent );
        Tcl_GetIntFromObj ( interp, objv[2], &child );
        remove_child ( parent, child );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_clear_child ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "clear_child id" );
            return TCL_ERROR;
        }

        int parent, child;
        Tcl_GetIntFromObj ( interp, objv[1], &parent );
        clear_child ( parent );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}


int tcl_node_delete ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "node_delete id" );
            return TCL_ERROR;
        }

        int id;
        Tcl_GetIntFromObj ( interp, objv[1], &id );
        node_delete ( id );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_node_visible ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "node_visible id isVisible" );
            return TCL_ERROR;
        }

        int id, isVisible;
        Tcl_GetIntFromObj ( interp, objv[1], &id );
        Tcl_GetBooleanFromObj ( interp, objv[2], &isVisible );
        node_visible ( id, isVisible );
        return TCL_OK;
    } catch ( std::exception& e ) {
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_update_bbox ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "update_bbox id" );
            return TCL_ERROR;
        }

        int id;
        Tcl_GetIntFromObj ( interp, objv[1], &id );
        update_bbox ( id );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

//int tcl_set_fillcolori ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
//{
//    try
//    {
//        if ( objc != 4 ) {
//            Tcl_WrongNumArgs ( interp, 0, objv, "set_fillcolori id color isbygroup" );
//            return TCL_ERROR;
//        }
//
//        int id, color, isByGroup;
//        Tcl_GetIntFromObj ( interp, objv[1], &id );
//        Tcl_GetIntFromObj ( interp, objv[2], &color );
//        Tcl_GetBooleanFromObj ( interp, objv[3], &isByGroup ); 
//        set_fillcolori ( id, color, isByGroup );
//        return TCL_OK;
//    } catch ( std::exception& e ) {
//        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
//        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
//        return TCL_ERROR;
//    }
//}
//
//int tcl_set_bordercolori ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
//{
//    try
//    {
//        if ( objc != 4 ) {
//            Tcl_WrongNumArgs ( interp, 0, objv, "set_bordercolori id color isByGroup" );
//            return TCL_ERROR;
//        }
//
//
//        int id, color, isByGroup;
//        Tcl_GetIntFromObj ( interp, objv[1], &id );
//        Tcl_GetIntFromObj ( interp, objv[2], &color );
//        Tcl_GetBooleanFromObj ( interp, objv[3], &isByGroup ); 
//        set_bordercolori ( id, color, isByGroup );
//        return TCL_OK;
//    } catch ( std::exception& e ) {
//        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
//        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
//        return TCL_ERROR;
//    }
//}
//
int tcl_color_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "color_create color" );
            return TCL_ERROR;
        }


        int color;
        Tcl_GetIntFromObj ( interp, objv[1], &color );
        int id = color_create ( color );
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_color_rgbas ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "color_rgbas id color" );
            return TCL_ERROR;
        }

        int id, len;
        Tcl_GetIntFromObj ( interp, objv[1], &id );
        const char* color = Tcl_GetStringFromObj ( objv[2], &len );
        color_rgbas ( id, color );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_color_rgbai ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "color_rgbai id color" );
            return TCL_ERROR;
        }

        int id, color;
        Tcl_GetIntFromObj ( interp, objv[1], &id );
        Tcl_GetIntFromObj ( interp, objv[2], &color );
        color_rgbai ( id, color );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_attrset_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "attrset_create layerid" );
            return TCL_ERROR;
        }

        int layerid;
        Tcl_GetIntFromObj ( interp, objv[1], &layerid );
        int id = attrset_create ( layerid );
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_attrset_fgcolor ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "attrset_fgcolor id  colorid" );
            return TCL_ERROR;
        }

        int id, colorid;
        Tcl_GetIntFromObj ( interp, objv[1], &id );
        Tcl_GetIntFromObj ( interp, objv[2], &colorid );
        attrset_fgcolor ( id, colorid );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_attrset_bgcolor ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "attrset_bgcolor id  colorid" );
            return TCL_ERROR;
        }

        int id, colorid;
        Tcl_GetIntFromObj ( interp, objv[1], &id );
        Tcl_GetIntFromObj ( interp, objv[2], &colorid );
        attrset_bgcolor ( id, colorid );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_set_attrset ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "set_attrset      id  attrsetid" );
            return TCL_ERROR;
        }

        int id, attrsetid;
        Tcl_GetIntFromObj ( interp, objv[1], &id );
        Tcl_GetIntFromObj ( interp, objv[2], &attrsetid );
        set_attrset ( id, attrsetid );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_unset_attrset ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "unset_attrset    id" );
            return TCL_ERROR;
        }

        int id;
        Tcl_GetIntFromObj ( interp, objv[1], &id );
        unset_attrset ( id );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

// mesh
int tcl_mesh_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 1 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "mesh_create" );
            return TCL_ERROR;
        }

        int id = mesh_create ();
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

// layer
int tcl_layer_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 2 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "layer_create name" );
            return TCL_ERROR;
        }

        int len;
        const char* name = Tcl_GetStringFromObj ( objv[1], &len );
        int id = layer_create ( name );
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_layer_name ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "layer_name id name" );
            return TCL_ERROR;
        }

        int id, len;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        const char* name = Tcl_GetStringFromObj ( objv[2], &len );
        layer_name ( id, name );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_layer_visible ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "layer_name id isvisible" );
            return TCL_ERROR;
        }

        int id, isvisible;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetBooleanFromObj ( interp, objv[2], &isvisible ); 
        layer_visible ( id, isvisible );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}


// lod
int tcl_lod_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 1 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "lod_create" ); 
            return TCL_ERROR;
        }

        int id = lod_create ();
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_lod_delimiters ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "lod_delimiters id delims" );
            return TCL_ERROR;
        }

        int id, len;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        const char* delims = Tcl_GetStringFromObj ( objv[2], &len );
        lod_delimiters ( id, delims );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}


// kdtree
int tcl_kdtree_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 1 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "kdtree_create" );
            return TCL_ERROR;
        }

        int id = kdtree_create ();
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

// array
int tcl_array_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 1 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "array_create" ); 
            return TCL_ERROR;
        }

        int id = array_create ();
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_array_rowcnt ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "array_rowcnt id cnt" ); 
            return TCL_ERROR;
        }

        int id, cnt;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetIntFromObj ( interp, objv[2], &cnt ); 
        array_rowcnt ( id, cnt );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_array_columncnt ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "array_columncnt id cnt" ); 
            return TCL_ERROR;
        }

        int id, cnt;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetIntFromObj ( interp, objv[2], &cnt ); 
        array_columncnt ( id, cnt );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_array_hlevelcnt ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 4 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "array_hlevelcnt id level cnt" ); 
            return TCL_ERROR;
        }

        int id, level, cnt;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetIntFromObj ( interp, objv[2], &level ); 
        Tcl_GetIntFromObj ( interp, objv[3], &cnt ); 
        array_hlevelcnt ( id, level, cnt );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_array_vlevelcnt ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 4 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "array_vlevelcnt id level cnt" ); 
            return TCL_ERROR;
        }

        int id, level, cnt;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetIntFromObj ( interp, objv[2], &level ); 
        Tcl_GetIntFromObj ( interp, objv[3], &cnt ); 
        array_vlevelcnt ( id, level, cnt );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_array_marginx ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 4 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "array_marginx id level space" ); 
            return TCL_ERROR;
        }

        int id, level;
        double space;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetIntFromObj ( interp, objv[2], &level ); 
        Tcl_GetDoubleFromObj ( interp, objv[3], &space ); 
        array_marginx ( id, level, space );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}


int tcl_array_marginy ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 4 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "array_marginy id level space" ); 
            return TCL_ERROR;
        }

        int id, level;
        double space;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetIntFromObj ( interp, objv[2], &level ); 
        Tcl_GetDoubleFromObj ( interp, objv[3], &space ); 
        array_marginy ( id, level, space );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}


// rectangle
int tcl_rectangle_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 1 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "rectangle_create" ); 
            return TCL_ERROR;
        }

        int id = rectangle_create ();
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_rectangle_size ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 4 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "rectangle_size id w h" ); 
            return TCL_ERROR;
        }

        int id;
        double w, h;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetDoubleFromObj ( interp, objv[2], &w ); 
        Tcl_GetDoubleFromObj ( interp, objv[3], &h ); 
        rectangle_size ( id, w, h );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}


// transform
int tcl_transform_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 1 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "transform_create" ); 
            return TCL_ERROR;
        }

        int id = transform_create ();
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_transform_translate ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 5 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "transform_translate id tx ty tz" ); 
            return TCL_ERROR;
        }

        int id;
        double x, y, z;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetDoubleFromObj ( interp, objv[2], &x ); 
        Tcl_GetDoubleFromObj ( interp, objv[3], &y ); 
        Tcl_GetDoubleFromObj ( interp, objv[4], &z );
        transform_translate3f ( id, x, y, z );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_transform_scale ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 5 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "transform_scale id sx sy sz" ); 
            return TCL_ERROR;
        }

        int id;
        double x, y, z;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetDoubleFromObj ( interp, objv[2], &x ); 
        Tcl_GetDoubleFromObj ( interp, objv[3], &y ); 
        Tcl_GetDoubleFromObj ( interp, objv[4], &z ); 
        stringstream ss;
        ss << x << ' ' << y << ' ' << z;
        transform_scale ( id, ss.str().c_str() );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

// pickablegroup
int tcl_pickablegroup_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 1 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "pickablegroup_create" ); 
            return TCL_ERROR;
        }

        int id = pickablegroup_create ();
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_pickablegroup_name ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "pickablegroup_name id name" ); 
            return TCL_ERROR;
        }

        int id, len;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        const char* name = Tcl_GetStringFromObj ( objv[2], &len);
        pickablegroup_name ( id, name );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}


// switchnode
int tcl_switchnode_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 1 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "switchnode_create" ); 
            return TCL_ERROR;
        }

        int id = switchnode_create ();
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_switchnode_props ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "switchnode_props id isvisible" ); 
            return TCL_ERROR;
        }

        int id, isvisible;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetBooleanFromObj ( interp, objv[2], &isvisible ); 
        switchnode_visible ( id, isvisible );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

// groupnode
int tcl_groupnode_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 1 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "groupnode_create" ); 
            return TCL_ERROR;
        }

        int id = groupnode_create ();
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_groupnode_props ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "groupnode_props id name" ); 
            return TCL_ERROR;
        }

        int id, len;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        const char* name = Tcl_GetStringFromObj ( objv[2], &len);
        groupnode_name ( id, name );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}


// text
int tcl_text_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc > 2 || objc < 1 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "text_create text" ); 
            return TCL_ERROR;
        }

        const char* name = "";
        if ( objc == 2 )
        {
            int len;
            name = Tcl_GetStringFromObj ( objv[1], &len);
        }

        int id = text_create ( name );
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_text_string ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "text_string id name" ); 
            return TCL_ERROR;
        }

        int id, len;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        const char* name = Tcl_GetStringFromObj ( objv[2], &len);
        text_string ( id, name );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_text_font ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "text_font id fontid" ); 
            return TCL_ERROR;
        }

        int id, fontid;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetIntFromObj ( interp, objv[2], &fontid ); 
        text_font ( id, fontid );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_text_anchor ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "text_anchor id anchor" ); 
            return TCL_ERROR;
        }

        int id, anchor;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetIntFromObj ( interp, objv[2], &anchor ); 
        text_anchor ( id, anchor );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_text_justify ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "text_justify id justify" ); 
            return TCL_ERROR;
        }

        int id, justify;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetIntFromObj ( interp, objv[2], &justify ); 
        text_font ( id, justify );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}


// font
int tcl_font_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 1 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "font_create" ); 
            return TCL_ERROR;
        }

        int id = font_create ();
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        //	Tcl_NewStringObj ( (const char*)(e.what()), e.what().size() );
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_font_family ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try 
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "font_family id family" );
            return TCL_ERROR;
        }

        int id, len;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        const char* family = Tcl_GetStringFromObj ( objv[2], &len);
        font_family ( id, family );
        return TCL_OK;
    } catch ( std::exception& e ) {
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_font_size ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try 
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "font_size id size" );
            return TCL_ERROR;
        }

        int id;
        double size;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetDoubleFromObj ( interp, objv[2], &size ); 
        font_size ( id, size );
        return TCL_OK;
    } catch ( std::exception& e ) {
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

/*PLAIN = 1, BOLD = 2, ITALIC = 3, BOLDITALIC = 4*/
int tcl_font_style ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try 
    {
        if ( objc != 3 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "font_style id style" );
            return TCL_ERROR;
        }

        int id, style;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetIntFromObj ( interp, objv[2], &style ); 
        font_style ( id, style );
        return TCL_OK;
    } catch ( std::exception& e ) {
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_line_create ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try
    {
        if ( objc != 1 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "line_create" ); 
            return TCL_ERROR;
        }

        int id = line_create ();
        Tcl_Obj* rstobj = Tcl_NewIntObj(id);
        Tcl_SetObjResult ( interp, rstobj );
        return TCL_OK;
    } catch ( std::exception& e ) {
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_line_points ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try 
    {
        if ( objc != 6 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "line_points id x1 y1 x2 y2" );
            return TCL_ERROR;
        }

        int id;
        double x1, y1, x2, y2;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetDoubleFromObj ( interp, objv[2], &x1 ); 
        Tcl_GetDoubleFromObj ( interp, objv[3], &y1 ); 
        Tcl_GetDoubleFromObj ( interp, objv[4], &x2 ); 
        Tcl_GetDoubleFromObj ( interp, objv[5], &y2 ); 
        line_points ( id, x1, y1, x2, y2 );
        return TCL_OK;
    } catch ( std::exception& e ) {
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

int tcl_line_point ( ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[] )
{
    try 
    {
        if ( objc != 6 ) {
            Tcl_WrongNumArgs ( interp, 0, objv, "line_point id x1 y1 isFirstPoint" );
            return TCL_ERROR;
        }

        int id;
        double x, y;
        int isFirstPoint;
        Tcl_GetIntFromObj ( interp, objv[1], &id ); 
        Tcl_GetDoubleFromObj ( interp, objv[2], &x ); 
        Tcl_GetDoubleFromObj ( interp, objv[3], &y ); 
        Tcl_GetBooleanFromObj ( interp, objv[4], &isFirstPoint ); 
        line_point ( id, x, y, isFirstPoint );
        return TCL_OK;
    } catch ( std::exception& e ) {
        Tcl_SetResult ( interp, (char*)e.what(), TCL_VOLATILE );
        return TCL_ERROR;
    }
}

void register_tclcmds ( Tcl_Interp* interp )
{
    Tcl_CreateObjCommand ( interp, "camera_create",         tcl_camera_create           ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "camera_translate",      tcl_camera_translate        ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "camera_scale",          tcl_camera_scale            ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "camera_reset",          tcl_camera_reset            ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "camera_name",           tcl_camera_name             ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "find_view",             tcl_find_view               ,0 ,0 );

    Tcl_CreateObjCommand ( interp, "viewport_create",       tcl_viewport_create         ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "viewport_geometry",     tcl_viewport_geometry       ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "viewport_attachcamera", tcl_viewport_attachcamera   ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "viewport_name",         tcl_viewport_name           ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "viewport_dirty",        tcl_viewport_dirty          ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "viewport_add",          tcl_viewport_add            ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "viewport_delete",       tcl_viewport_delete         ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "update",                tcl_update                  ,0 ,0 );

    Tcl_CreateObjCommand ( interp, "mesh_load",             tcl_mesh_load               ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "mesh_save",             tcl_mesh_save               ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "mesh_unload",           tcl_mesh_unload             ,0 ,0 );

    Tcl_CreateObjCommand ( interp, "dump_node",             tcl_dump_node               ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "get_bbox",              tcl_get_bbox                ,0 ,0 );

    Tcl_CreateObjCommand ( interp, "pick",                  tcl_pick                    ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "pick_volume",           tcl_pick_volume             ,0 ,0 );

    Tcl_CreateObjCommand ( interp, "add_child",             tcl_add_child               ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "remove_child",          tcl_remove_child            ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "clear_child",           tcl_clear_child             ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "node_delete",           tcl_node_delete             ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "node_visible",          tcl_node_visible            ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "update_bbox",           tcl_update_bbox             ,0 ,0 );
    //Tcl_CreateObjCommand ( interp, "set_fillcolori",        tcl_set_fillcolori          ,0 ,0 );
    //Tcl_CreateObjCommand ( interp, "set_bordercolori",      tcl_set_bordercolori        ,0 ,0 );
    
    Tcl_CreateObjCommand ( interp, "color_create",          tcl_color_create            ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "color_rgbas",           tcl_color_rgbas             ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "color_rgbai",           tcl_color_rgbai             ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "attrset_create",        tcl_attrset_create          ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "attrset_fgcolor",       tcl_attrset_fgcolor         ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "attrset_bgcolor",       tcl_attrset_bgcolor         ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "set_attrset",           tcl_set_attrset             ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "unset_attrset",         tcl_unset_attrset           ,0 ,0 );

    Tcl_CreateObjCommand ( interp, "mesh_create",           tcl_mesh_create             ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "layer_create",          tcl_layer_create            ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "layer_name",            tcl_layer_name              ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "layer_visible",         tcl_layer_visible           ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "lod_create",            tcl_lod_create              ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "lod_delimiters",        tcl_lod_delimiters          ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "kdtree_create",         tcl_kdtree_create           ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "array_create",          tcl_array_create            ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "array_rowcnt",          tcl_array_rowcnt            ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "array_columncnt",       tcl_array_columncnt         ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "array_hlevelcnt",       tcl_array_hlevelcnt         ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "array_vlevelcnt",       tcl_array_vlevelcnt         ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "array_marginx",         tcl_array_marginx           ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "array_marginy",         tcl_array_marginy           ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "rectangle_create",      tcl_rectangle_create        ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "rectangle_size",        tcl_rectangle_size          ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "transform_create",      tcl_transform_create        ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "transform_translate",   tcl_transform_translate     ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "transform_scale",       tcl_transform_scale         ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "pickablegroup_create",  tcl_pickablegroup_create   	,0 ,0 );
    Tcl_CreateObjCommand ( interp, "pickablegroup_name",    tcl_pickablegroup_name     	,0 ,0 );
    Tcl_CreateObjCommand ( interp, "switchnode_create",     tcl_switchnode_create      	,0 ,0 );
    Tcl_CreateObjCommand ( interp, "switchnode_props",      tcl_switchnode_props       	,0 ,0 );
    Tcl_CreateObjCommand ( interp, "groupnode_create",      tcl_groupnode_create       	,0 ,0 );
    Tcl_CreateObjCommand ( interp, "groupnode_props",       tcl_groupnode_props         ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "text_create",           tcl_text_create             ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "text_string",           tcl_text_string             ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "text_font",             tcl_text_font               ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "text_anchor",           tcl_text_anchor             ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "text_justify",          tcl_text_justify            ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "font_create",           tcl_font_create             ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "font_family",           tcl_font_family             ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "font_size",             tcl_font_size               ,0 ,0 );
    Tcl_CreateObjCommand ( interp, "font_style",            tcl_font_style              ,0 ,0 );               
    Tcl_CreateObjCommand ( interp, "line_create",           tcl_line_create             ,0 ,0 );               
    Tcl_CreateObjCommand ( interp, "line_create",           tcl_line_create             ,0 ,0 );               
    Tcl_CreateObjCommand ( interp, "line_points",           tcl_line_points             ,0 ,0 );               
    Tcl_CreateObjCommand ( interp, "line_point",            tcl_line_point              ,0 ,0 );               
}
