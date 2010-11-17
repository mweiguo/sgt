#include "sgr_interface.h"
#include "sgr_nodemgr.h"
#include "sgr_nodes.h"
#include "sgr_vec3.h"
#include "sgr_renderfunctor.h"
#include "sgr_glrenderfunctor.h"
#include "sgr_renderflow.h"
#include "sgr_savemesh.h"
#include "sgr_loadscene.h"
//#include "sgr_qviewport.h"
#include "sgr_arrayexpander.h"
#include "sgr_scenenode.h"
#include "sgr_nodedumper.h"
#include "sgr_volumepicker.h"
#include "sgr_colornode.h"
#include "sgr_attrset.h"
#include "sgr_polylinenode.h"
#include "sgr_meshpointnode.h"
//#include "sgr_tools.h"
#include "sgr_nodetransformer.h"
#include "sgr_boxpicker.h"
#include "sgr_crosspicker.h"
#include "sgr_childrenfinder.h"
#include "sgr_projection.h"
#include "sgr_mat4.h"

#include <fstream>
#include <stdexcept>

using namespace std;
using namespace SGR;
// camera management

//void init_windowsystem ()
//{
//    QViewport::getInst().show();
//}
//
//void system_mode ( int m )
//{
//    QViewport::getInst().mode ( m );
//}

//void select_tool ( int tool )
//{
//    if ( DRAWLINE_TOOL == tool )
//        GETools::getInst().selectTool ( GETools::MESHLINE_TOOL );
//}
//
//void cad_enviroment ( int camid )
//{
//    QViewport::getInst().setEnviroment ( camid );
//}
void use_renderlib ( int lib )
{
    if ( 1 == lib )
	Rendering::renderlib = Rendering::RENDER_IN_QT;
    else
	Rendering::renderlib = Rendering::RENDER_IN_OPENGL;
}

void node_translate ( int id, float tx, float ty, float tz )
{
    SGNode* node = NodeMgr::getInst().getNodePtr<SGNode> (id);
    if ( node )
    {
        NodeTransformer transformer;
        transformer.translate ( node, tx, ty, tz );
    }
}

void node_scale ( int id, float s )
{
    SGNode* node = NodeMgr::getInst().getNodePtr<SGNode> (id);
    if ( node )
    {
        NodeTransformer transformer;
        transformer.scale ( node, s );
    }
}

void camera_create ( int id, const char* nm )
{
    // new camera
    // add it to manager
    CameraOrtho* t = NodeMgr::getInst().addNode<CameraOrtho> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "camera node create: id is invalid. name: " << nm << ", id:" << id;
        throw invalid_argument ( ss.str() );
    }
    t->name ( nm );
}

void camera_translate ( int id, float tx, float ty, float tz )
{
    CameraOrtho* cam = NodeMgr::getInst().getNodePtr<CameraOrtho> (id);
    if ( cam )
        cam->translate ( vec3f(tx, ty, tz) );
}

void camera_scale ( int id, float scale )
{
    CameraOrtho* cam = NodeMgr::getInst().getNodePtr<CameraOrtho> (id);
    if ( cam )
        cam->zoom  ( scale );
}

void camera_reset ( int id )
{
    CameraOrtho* cam = NodeMgr::getInst().getNodePtr<CameraOrtho> (id);
    if ( cam )
    {
        cam->translate ( vec3f() );
        cam->zoom ( 1 );
    }
}

void camera_name ( int id, const char* nm )
{
    CameraOrtho* cam = NodeMgr::getInst().getNodePtr<CameraOrtho> (id);
    if ( cam )
        cam->name ( nm );
}

void get_cameramatrix ( int id, float* mat )
{
    CameraOrtho* cam = NodeMgr::getInst().getNodePtr<CameraOrtho> (id);
    if ( cam )
	cam->mvmatrix ().toArray ( mat );
}

void get_camerainversematrix ( int id, float* mat )
{
    CameraOrtho* cam = NodeMgr::getInst().getNodePtr<CameraOrtho> (id);
    if ( cam )
        cam->inversematrix ().toArray ( mat );
}

void projection_create ( int id )
{
    Projection* t = NodeMgr::getInst().addNode<Projection> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "projection node create: id is invalid. id:" << id;
        throw invalid_argument ( ss.str() );
    }
}

void projection_frustum ( int id, float l, float r, float b, float t, float n, float f )
{
    Projection* proj = NodeMgr::getInst().getNodePtr<Projection> (id);
    if ( proj )
	proj->frustum ( l, r, b, t, n, f );
}

void projection_ortho ( int id, float l, float r, float b, float t, float n, float f )
{
    Projection* proj = NodeMgr::getInst().getNodePtr<Projection> (id);
    if ( proj )
	proj->ortho ( l, r, b, t, n, f );
}

void get_projectionmatrix ( int id, float* mat )
{
    Projection* proj = NodeMgr::getInst().getNodePtr<Projection> (id);
    if ( proj )
	proj->projmatrix ().toArray(mat);
}

void get_projectioninversematrix ( int id, float* mat )
{
    Projection* proj = NodeMgr::getInst().getNodePtr<Projection> (id);
    if ( proj )
	proj->inversematrix ().toArray(mat);
}

//float find_view ( float* min, float* max, float percentOfView, int camid, int vpid )
//{
//    vec3f a ( min ), b(max);
//    BBox bbox ( a, b );
//    //BBox bbox ( vec3f(min), vec3f(max) );
//    vec3f center = bbox.center();
//    float minDimension = bbox.dimension().w() < bbox.dimension().h() ? bbox.dimension().h() : bbox.dimension().w() ;
//    if ( 0 == minDimension )
//    {
//        stringstream ss;
//        ss << "find_view: min max should be different";
//        throw std::invalid_argument ( ss.str().c_str() );
//    }
//
//    // calc scale
//    vec3f demension = bbox.dimension();
//    Viewport* t = NodeMgr::getInst().getNodePtr<Viewport> (vpid);
//    if ( NULL != t )
//    {
//        float scale;
//        const vec2i& size = t->viewportSize ();
//        if ( size.w() > size.h() )
//        {
//            scale = demension.w() > demension.h() ? 1 / demension.w() : 1 / demension.h();
//            scale = scale * size.h() / size.w();
//        }
//        else
//            scale = demension.w() > demension.h() ? 1 / demension.w() : 1 / demension.h();
//
//        camera_reset ( camid );
//        camera_translate ( camid, center.x(), center.y(), center.z() );
//        camera_scale ( camid, scale );
//    //    sgwindow_update ( vpid );
//        return scale;
//    }
//    return 1;
//    
////    //float scale        = percentOfView / minDimension;
////    float scale        = demension.w() > demension.h() ? 1 / demension.w() : 1 / demension.h();
////
////    camera_reset ( camid );
////    camera_translate ( camid, center.x(), center.y(), center.z() );
////    camera_scale ( camid, scale );
//////    sgwindow_update ( vpid );
////    return scale;
//}

void viewport_create ( int id, const char* nm )
{
    Viewport* t = NodeMgr::getInst().addNode<Viewport> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "viewport node create: id is invalid. name: " << nm << ", id: " << id;
        throw invalid_argument ( ss.str() );
    }
    t->name ( nm );
    //viewport_name ( id, name );
    //return id;
}

void viewport_geometry ( int id, int x, int y, int w, int h )
{
    Viewport* p = NodeMgr::getInst().getNodePtr<Viewport> (id);
    if ( p )
    {
        p->position ( x, y );
        p->viewportSize ( w, h );
    }
}

void viewport_attachcamera ( int id, int camid )
{
    Viewport* p = NodeMgr::getInst().getNodePtr<Viewport> (id);
    if ( p )
        p->attachcamera ( camid );
}

void viewport_attachprojection ( int id, int projid )
{
    Viewport* p = NodeMgr::getInst().getNodePtr<Viewport> (id);
    if ( p )
        p->attachproj ( projid );
}

void viewport_name ( int id, const char* nm )
{
    Viewport* p = NodeMgr::getInst().getNodePtr<Viewport> (id);
    if ( p )
        p->name ( nm );
}

void viewport_update ( int id, QPainter& painter )
{
    Viewport* p = NodeMgr::getInst().getNodePtr<Viewport> (id);
    if ( p )
    {
        QRenderOption opt;
        opt.painter = &painter;
        RenderFlow renderflow ( *p, opt );
        p->update ();
    }
}

void viewport_update2 ( int id )
{
    Viewport* p = NodeMgr::getInst().getNodePtr<Viewport> (id);
    if ( p )
    {
        GLRenderOption opt;
        RenderFlow renderflow ( *p, opt );
        p->update ();
    }
}

void get_viewportmatrix ( int id, float* mat )
{
    Viewport* p = NodeMgr::getInst().getNodePtr<Viewport> (id);
    if ( p )
	p->vpmatrix().toArray ( mat );
}

void get_viewportinversematrix ( int id, float* mat )
{
    Viewport* p = NodeMgr::getInst().getNodePtr<Viewport> (id);
    if ( p )
	p->inversematrix().toArray ( mat );;
}


void viewport_dirty ( int id )
{
    Viewport* p = NodeMgr::getInst().getNodePtr<Viewport> (id);
    if ( p )
        p->dirty ( true );
}

//void viewport_add ( int id )
//{
//    QViewport::getInst().add_viewport( id );
//}
//
//void viewport_delete ( int id )
//{
//    QViewport::getInst().remove_viewport( id );
//}
//
//void sgwindow_update ( int id )
//{
//    if ( id < 0 )
//    {
//        QViewport::getInst().dirty_all();
//        QViewport::getInst().update();
//    }
//    else
//    {
//        viewport_dirty ( id );
//        QViewport::getInst().update();
//    }
//}

void add_child ( int parent, int child )
{
    NodeMgr& nv = NodeMgr::getInst();
    NodeMgr::iterator pp1 = nv.find ( parent );
    NodeMgr::iterator pp2 = nv.find ( child );
    if ( pp1!=nv.end() && pp2!=nv.end() )
        pp1->second->addChild ( pp2->second );
}

void SGR_DLL pushfront_child ( int parent, int child )
{
    NodeMgr& nv = NodeMgr::getInst();
    NodeMgr::iterator pp1 = nv.find ( parent );
    NodeMgr::iterator pp2 = nv.find ( child );
    if ( pp1!=nv.end() && pp2!=nv.end() )
        pp1->second->pushfrontChild ( pp2->second );
}

void remove_child ( int parent, int child )
{
    NodeMgr& nv = NodeMgr::getInst();
    NodeMgr::iterator pp1 = nv.find ( parent );
    NodeMgr::iterator pp2 = nv.find ( child );
    if ( pp1!=nv.end() && pp2!=nv.end() )
        pp1->second->removeChild ( pp2->second );
}

void remove_parent ( int nodeid )
{
    SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>(nodeid);
    if ( node )
	node->setParentNode ( 0 );
}

void clear_child ( int id )
{
    NodeMgr& nv = NodeMgr::getInst();
    NodeMgr::iterator pp1 = nv.find ( id );
    if ( pp1!=nv.end()  )
        pp1->second->removeAllChild ();
}

void node_delete ( int id )
{
    NodeMgr::getInst().deleteNode ( id );
//     NodeMgr::iterator pp = NodeMgr::getInst().find(id);
//     if ( pp != NodeMgr::getInst().end() )
//     {
//         SGNode* node = pp->second;
//         node->removeAllChild ();
//         node->setParentNode ( NULL );
//         delete node;
//         NodeMgr::getInst().erase ( id );
//     }
}

void node_visible ( int id, bool isVisible )
{
    DrawableNode* drawable = NodeMgr::getInst().getNodePtr<DrawableNode>(id);
    if ( drawable )
    {
        drawable->setVisible ( isVisible );
    }
}

void set_userdata ( int id, void* data )
{
    NodeMgr::iterator pp = NodeMgr::getInst().find(id);
    if ( pp != NodeMgr::getInst().end() )
    {
        SGNode* node = pp->second;
        node->setUserData ( data );
    }
}

void get_userdata ( int id, void** data )
{
    *data = NULL;
    NodeMgr::iterator pp = NodeMgr::getInst().find(id);
    if ( pp != NodeMgr::getInst().end() )
    {
        SGNode* node = pp->second;
        *data = node->getUserData ();
    }
}

void update_bbox ( int id )
{
    SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>( id );
    if ( node )
        node->updateBBox();
}

// mesh
void mesh_create ( int id )
{
    MeshNode3f* t = NodeMgr::getInst().addNode<MeshNode3f> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "mesh node create: id is invalid. id: " << id;
        throw invalid_argument ( ss.str() );
    }
}

void mesh_coords ( int id, float* coords3d, int elementN )
{
    MeshNode3f* node = NodeMgr::getInst().getNodePtr<MeshNode3f>( id );
    if ( node )
        node->setCoords ( (vec3f*)coords3d, elementN );
}

void mesh_subcoords ( int id, int* indexes, int elementN, float* coords3d )
{
    MeshNode3f* node = NodeMgr::getInst().getNodePtr<MeshNode3f>( id );
    if ( node )
        node->setSubCoords ( indexes, elementN, (vec3f*)coords3d );
}

int mesh_appendcoords ( int id, float* coords3d, int elementN )
{
    MeshNode3f* node = NodeMgr::getInst().getNodePtr<MeshNode3f>( id );
    if ( node )
    {
        node->appendCoords ( (vec3f*)coords3d, elementN );
        return node->coordsN ()-1;
    }
    return -1;
}

int mesh_appendcoord ( int id, float x, float y, float z )
{
    MeshNode3f* node = NodeMgr::getInst().getNodePtr<MeshNode3f>( id );
    if ( node )
    {
        int idx = node->coordsN ();
        node->appendCoords ( vec3f(x,y,z) );
        return idx;
    }
    return -1;
}

void get_mesh_coord ( int id, int idx, float& x, float& y, float& z )
{
    MeshNode3f* node = NodeMgr::getInst().getNodePtr<MeshNode3f>( id );
    if ( node )
    {
        vec3f pos = (*node)[idx];
        x = pos.x();
        y = pos.y();
        z = pos.z();
    }
}

// mesh line
void meshline_create ( int id )
{
    MeshLineNode* t = NodeMgr::getInst().addNode<MeshLineNode> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "meshline node create: id is invalid. id: " << id;
        throw invalid_argument ( ss.str() );
    }
}

void meshline_type ( int id, int type )
{
    MeshLineNode* node = NodeMgr::getInst().getNodePtr<MeshLineNode>( id );
    if ( node )
        node->type ( type );
}

void meshline_data ( int id, int* data, int elementN )
{
    MeshLineNode* node = NodeMgr::getInst().getNodePtr<MeshLineNode>( id );
    if ( node )
        node->setData ( data, elementN );
}

void meshline_subdata ( int id, int* indexes, int elementN, int* data )
{
    MeshLineNode* node = NodeMgr::getInst().getNodePtr<MeshLineNode>( id );
    if ( node )
        node->setData ( indexes, elementN, data );
}

void meshline_appendpoint ( int id, int coordIdx )
{
    MeshLineNode* node = NodeMgr::getInst().getNodePtr<MeshLineNode>( id );
    if ( node )
        node->appendCoordIdx ( coordIdx );
}

int meshline_pointsN ( int id )
{
    MeshLineNode* node = NodeMgr::getInst().getNodePtr<MeshLineNode>( id );
    if ( node )
        return node->pntsize();
    return -1;
}

int get_meshline_coordindexes ( int id, int* coords )
{
    MeshLineNode* node = NodeMgr::getInst().getNodePtr<MeshLineNode>( id );
    if ( node )
    {
        MeshLineNode::pntiterator pp, end=node->pntend();
        for ( pp=node->pntbegin(); pp!=end; ++pp )
        {
            *(coords++) = *pp;
        }
        return node->pntsize();
    }
    return -1;
}

void meshpoint_create ( int id, int coordIdx )
{
    MeshPointNode* t = NodeMgr::getInst().addNode<MeshPointNode> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "mesh point node create: id is invalid. id: " << id << ", coordIdx: " << coordIdx;
        throw invalid_argument ( ss.str() );
    }
    t->coordIdx ( coordIdx );
}

void meshpoint_coordidx ( int id, int coordidx )
{
    MeshPointNode* node = NodeMgr::getInst().getNodePtr<MeshPointNode>( id );
    if ( node )
        node->coordIdx ( coordidx );
}

void meshpoint_size ( int id, float sz )
{
    MeshPointNode* node = NodeMgr::getInst().getNodePtr<MeshPointNode>( id );
    if ( node )
        node->pointSize ( sz );
}

int get_meshpoint_coordindex ( int id )
{
    MeshPointNode* node = NodeMgr::getInst().getNodePtr<MeshPointNode>( id );
    if ( node )
        return node->coordIdx ();
    return -1;
}

void color_create ( int id, unsigned int color )
{
    ColorNode* t = NodeMgr::getInst().addNode<ColorNode> (id, color);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "color node create: id is invalid. id: " << id << ", color: " << color;
        throw invalid_argument ( ss.str() );
    }
}

void color_rgbas ( int id, const char* color )
{
    ColorNode* node = NodeMgr::getInst().getNodePtr<ColorNode>( id );
    if ( node )
        node->setColor( color );
}

void color_rgbai ( int id, unsigned int color )
{
    ColorNode* node = NodeMgr::getInst().getNodePtr<ColorNode>( id );
    if ( node )
        node->setColor( color );
}

void attrset_create ( int id, int layerid )
{
    //LayerNode* node = NodeMgr::getInst().getNodePtr<LayerNode>( layerid );
    //if ( node )
    //{
    //int renderOrder = LayerMgr::getInst().indexof(node);
    AttrSet* t = NodeMgr::getInst().addNode<AttrSet> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "attrset node create: id is invalid. id: " << id << "layerid: " << layerid;
        throw invalid_argument ( ss.str() );
    }
    //}
    //else
    //{
    //    stringstream ss;
    //    ss << "layer node not exist or id " << layerid << " is not a layer";
    //    throw std::invalid_argument ( ss.str().c_str() );
    //}
}

void attrset_create_byrenderorder ( int id, int renderorder )
{
    AttrSet* t = NodeMgr::getInst().addNode<AttrSet> (id, renderorder);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "attrset node create: id is invalid. id: " << id << ", renderorder: " << renderorder;
        throw invalid_argument ( ss.str() );
    }
}

void attrset_fgcolor ( int id, int colorid )
{
    AttrSet* node = NodeMgr::getInst().getNodePtr<AttrSet>( id );
    if ( node )
    {
        ColorNode* colornode = NodeMgr::getInst().getNodePtr<ColorNode>( colorid );
        if ( colornode )
            node->setFgColor (colornode);
    }
}

void attrset_bgcolor ( int id, int colorid )
{
    AttrSet* node = NodeMgr::getInst().getNodePtr<AttrSet>( id );
    if ( node )
    {
        ColorNode* colornode = NodeMgr::getInst().getNodePtr<ColorNode>( colorid );
        if ( colornode )
            node->setBgColor (colornode);
    }
}

void attrset_font ( int id, int fontid )
{
    AttrSet* node = NodeMgr::getInst().getNodePtr<AttrSet>( id );
    if ( node )
    {
        FontNode* fontnode = NodeMgr::getInst().getNodePtr<FontNode>( fontid );
        if ( fontnode )
            node->setFont ( fontnode );
    }
}

void set_attrset ( int nodeid, int attrsetid )
{
    DrawableNode* node = NodeMgr::getInst().getNodePtr<DrawableNode>( nodeid );
    if ( node )
    {
        AttrSet* attrsetNode = NodeMgr::getInst().getNodePtr<AttrSet>( attrsetid );
        if ( attrsetNode )
            node->setAttrSet ( attrsetNode );
    }
}

void unset_attrset ( int nodeid )
{
    DrawableNode* node = NodeMgr::getInst().getNodePtr<DrawableNode>( nodeid );
    if ( node )
    {
        node->setAttrSet ( 0 );
    }
}

int attrset_refcnt ( int nodeid )
{
    AttrSet* attrsetNode = NodeMgr::getInst().getNodePtr<AttrSet>( nodeid );
    if ( attrsetNode )
        return attrsetNode->getRefCnt();
    return -1;
}

int get_attrset ( int nodeid )
{
    AttrSet* attrsetNode = NodeMgr::getInst().getNodePtr<AttrSet>( nodeid );
    if ( attrsetNode )
        return attrsetNode->getID();
    return -1;
}

unsigned int get_attrset_fgcolor ( int attrsetid )
{
    AttrSet* attrsetNode = NodeMgr::getInst().getNodePtr<AttrSet>( attrsetid );
    if ( attrsetNode )
        return attrsetNode->getFgColor()->getColor();
    throw invalid_argument ( "attrsetid is invalid" );
}

unsigned int get_attrset_bgcolor ( int attrsetid )
{
    AttrSet* attrsetNode = NodeMgr::getInst().getNodePtr<AttrSet>( attrsetid );
    if ( attrsetNode )
        return attrsetNode->getBgColor()->getColor();
    throw invalid_argument ( "attrsetid is invalid" );
}

int get_attrset_font ( int attrsetid )
{
    AttrSet* attrsetNode = NodeMgr::getInst().getNodePtr<AttrSet>( attrsetid );
    if ( attrsetNode )
        return attrsetNode->getFont()->getID();
    return -1;
}

//void set_bordercolori ( int id, unsigned int color, bool isByGroup )
//{
//    //SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>( id );
//    //BorderColorChanger<unsigned int> changer ( color, isByGroup );
//    //node->accept ( changer );
//}
//
//void set_fillcolori ( int id, unsigned int color, bool isByGroup )
//{
//    //SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>( id );
//    //FillColorChanger<unsigned int> changer ( color, isByGroup );
//    //node->accept ( changer );
//}
//
//void set_fgcolori ( int id, unsigned int color )
//{
//}
//
//void set_bgcolori ( int id, unsigned int color )
//{
//}
//
//void set_bordercolors ( int id, const char* color, bool isByGroup )
//{
//    //SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>( id );
//    //BorderColorChanger<string> changer ( color, isByGroup );
//    //node->accept ( changer );
//}
//
//void set_fillcolors ( int id, const char* color, bool isByGroup )
//{
//    //SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>( id );
//    //FillColorChanger<string> changer ( color, isByGroup );
//    //node->accept ( changer );
//}

// mesh
void scene_create ( int id )
{
    SceneNode* t = NodeMgr::getInst().addNode<SceneNode> ( id );
    if ( NULL == t )
    {
        stringstream ss;
        ss << "scene node create: id is invalid. id: " << id;
        throw invalid_argument ( ss.str() );
    }
}

// layer
void layer_create ( int id, const char* name )
{
    LayerNode* t = NodeMgr::getInst().addNode<LayerNode> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "layer node create: id is invalid. id: " << id << ", name: " << name;
        throw invalid_argument ( ss.str() );
    }

    layer_name ( id, name );
}

void layer_name ( int id, const char* nm )
{
    LayerNode* layer = NodeMgr::getInst().getNodePtr<LayerNode> ( id );
    if ( layer )
        layer->name ( nm );
}

void layer_visible ( int id, bool isVisible )
{
    LayerNode* layer = NodeMgr::getInst().getNodePtr<LayerNode> ( id );
    if ( layer )
        layer->setVisible ( isVisible );
}

void layer_fgcolor ( int id, int color )
{
    LayerNode* layer = NodeMgr::getInst().getNodePtr<LayerNode> ( id );
    if ( layer )
        layer->setFgColor ( GColor(color) );
}

void layer_bgcolor ( int id, int color )
{
    LayerNode* layer = NodeMgr::getInst().getNodePtr<LayerNode> ( id );
    if ( layer )
        layer->setBgColor ( GColor(color) );
}

void layer_font ( int id, const char* family, int size, int /*style*/, int /*weight*/, const char* name )
{
    LayerNode* layer = NodeMgr::getInst().getNodePtr<LayerNode> ( id );
    if ( layer )
        layer->setFont (  name, family, size );
}

// lod
void lod_create ( int id )
{
    LODNode* t = NodeMgr::getInst().addNode<LODNode> ( id );
    if ( NULL == t )
    {
        stringstream ss;
        ss << "lod node create: id is invalid. id: " << id ;
        throw invalid_argument ( ss.str() );
    }
}

void lod_delimiters ( int id, const char* delims )
{
    LODNode* lod = NodeMgr::getInst().getNodePtr<LODNode> ( id );
    if ( lod )
        lod->setdelimiters ( delims );
}

// kdtree
void kdtreenode_create ( int id )
{
    KdTreeNode* t = NodeMgr::getInst().addNode<KdTreeNode> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "kdtree node create: id is invalid. id: " << id ;
        throw invalid_argument ( ss.str() );
    }
}

void kdtreenode_build ( int id )
{
    KdTreeNode* node = NodeMgr::getInst().getNodePtr<KdTreeNode>(id);
    if ( node )
        node->buildKdTree ();
}

// array
void array_create ( int id )
{
    ArrayNode* t = NodeMgr::getInst().addNode<ArrayNode> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "array node create: id is invalid. id: " << id;
        throw invalid_argument ( ss.str() );
    }
}

void array_rowcnt ( int id, int cnt )
{
    ArrayNode* parray = NodeMgr::getInst().getNodePtr<ArrayNode>(id);
    if ( parray )
        parray->setRowCnt ( cnt );
}

void array_columncnt ( int id, int cnt )
{
    ArrayNode* parray = NodeMgr::getInst().getNodePtr<ArrayNode>(id);
    if ( parray )
        parray->setColumnCnt ( cnt );
}

void array_hlevelcnt ( int id, int level, int cnt )
{
    ArrayNode* parray = NodeMgr::getInst().getNodePtr<ArrayNode>(id);
    if ( parray )
        parray->setHLevels ( level, cnt );
}

void array_vlevelcnt ( int id, int level, int cnt )
{
    ArrayNode* parray = NodeMgr::getInst().getNodePtr<ArrayNode>(id);
    if ( parray )
        parray->setVLevels ( level, cnt );
}

void array_marginx ( int id, int level, float space )
{
    ArrayNode* parray = NodeMgr::getInst().getNodePtr<ArrayNode>(id);
    if ( parray )
        parray->setMarginX ( level, space );
}

void array_marginy ( int id, int level, float space )
{
    ArrayNode* parray = NodeMgr::getInst().getNodePtr<ArrayNode>(id);
    if ( parray )
        parray->setMarginY ( level, space );
}

void expand_arraynode ( int id )
{
    ArrayNode* parray = NodeMgr::getInst().getNodePtr<ArrayNode>(id);
    if ( parray ) {
        SGNode* parent = parray->getParentNode();
        ArrayExpander expander ( parent, true );
        expander ( *parray );
    }
}

// rectangle
void rectangle_create ( int id )
{
    RectangleNodef* t = NodeMgr::getInst().addNode<RectangleNodef> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "rectangle node create: id is invalid. id: " << id;
        throw invalid_argument ( ss.str() );
    }
}

void rectangle_size ( int id, float width, float height )
{
    RectangleNodef* rect = NodeMgr::getInst().getNodePtr<RectangleNodef>(id);
    if ( rect )
    {
        rect->w ( width );
        rect->h ( height );
    }
}

// transform
void transform_create ( int id )
{
    TransformNode* t = NodeMgr::getInst().addNode<TransformNode> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "transform node create: id is invalid. id: " << id;
        throw invalid_argument ( ss.str() );
    }
}

void transform_translate3f ( int id, float tx, float ty, float tz )
{
    TransformNode* node = NodeMgr::getInst().getNodePtr<TransformNode> (id);
    if ( node )
        node->setTranslate ( tx, ty, tz );
}

void transform_translates ( int id, const char* str )
{
    TransformNode* node = NodeMgr::getInst().getNodePtr<TransformNode> (id);
    if ( node )
        node->setTranslate ( str );
}

void transform_scale ( int id, float sx, float sy, float sz )
{
    TransformNode* node = NodeMgr::getInst().getNodePtr<TransformNode> (id);
    if ( node )
        node->setScale ( sx, sy, sz );
}
void transform_scale ( int id, const char* str )
{
    TransformNode* node = NodeMgr::getInst().getNodePtr<TransformNode> (id);
    if ( node )
        node->setScale ( str );
}

// pickablegroup
void pickablegroup_create ( int id, const char* nm )
{
    PickableGroup* t = NodeMgr::getInst().addNode<PickableGroup> (id, nm);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "pickablegroup node create: id is invalid. id: " << id;
        throw invalid_argument ( ss.str() );
    }
}

void pickablegroup_name ( int id, const char* nm )
{
    PickableGroup* pick = NodeMgr::getInst().getNodePtr<PickableGroup> (id);
    if ( pick )
        pick->name ( nm );
}

// switchnode
void switchnode_create ( int id )
{
    SwitchNode* t = NodeMgr::getInst().addNode<SwitchNode> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "switch node create: id is invalid. id: " << id;
        throw invalid_argument ( ss.str() );
    }
}

void switchnode_visible ( int id, bool isVisible )
{
    SwitchNode* node = NodeMgr::getInst().getNodePtr<SwitchNode> (id);
    if ( node )
        node->setVisible ( isVisible );
}

void switchnode_selection ( int id, int idx )
{
    SwitchNode* node = NodeMgr::getInst().getNodePtr<SwitchNode> (id);
    if ( node )
        node->selection ( idx );
}

// groupnode
void groupnode_create ( int id, const char* name )
{
    GroupNode* t = NodeMgr::getInst().addNode<GroupNode> (id, name);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "group node create: id is invalid. id: " << id << " name: " << name;
        throw invalid_argument ( ss.str() );
    }
}

void groupnode_name ( int id, const char* nm )
{
    GroupNode* node = NodeMgr::getInst().getNodePtr<GroupNode> (id);
    if ( node )
        node->name ( nm );
}

void text_create ( int id, const char* str )
{
    TextNode* t = NodeMgr::getInst().addNode<TextNode> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "text node create: id is invalid. id: " << id << ", content: " << str;
        throw invalid_argument ( ss.str() );
    }
    t->text ( str );
    //text_string ( id, str );
    //return id;
}

void text_string ( int id, const char* str )
{
    TextNode* node = NodeMgr::getInst().getNodePtr<TextNode> (id);
    if ( node )
        node->text ( str );
}

//void text_font ( int id, int fontid )
//{
//    TextNode* node = NodeMgr::getInst().getNodePtr<TextNode> (id);
//    if ( node )
//    {
//        FontNode* fnode = NodeMgr::getInst().getNodePtr<FontNode> (fontid);
//        if ( fnode )
//        {
//            node->fontnode ( fnode );
//        }
//    }
//}

//  1     2     3
//  4     5     6
//  7     8     9
void text_anchor ( int id, int anchor )
{
    TextNode* node = NodeMgr::getInst().getNodePtr<TextNode> (id);
    if ( node )
    {
        switch ( anchor )
        {
        case 1:
            node->setAnchor ( TextNode::AnchorLEFT | TextNode::AnchorTOP );
            break;
        case 2:
            node->setAnchor ( TextNode::AnchorTOP | TextNode::AnchorHCENTER );
            break;
        case 3:
            node->setAnchor ( TextNode::AnchorRIGHT | TextNode::AnchorTOP );
            break;
        case 4:
            node->setAnchor ( TextNode::AnchorLEFT | TextNode::AnchorVCENTER );
            break;
        case 5:
            node->setAnchor ( TextNode::AnchorCENTER );
            break;
        case 6:
            node->setAnchor ( TextNode::AnchorRIGHT | TextNode::AnchorVCENTER );
            break;
        case 7:
            node->setAnchor ( TextNode::AnchorLEFT | TextNode::AnchorBOTTOM );
            break;
        case 8:
            node->setAnchor ( TextNode::AnchorBOTTOM | TextNode::AnchorHCENTER  );
            break;
        case 9:
            node->setAnchor ( TextNode::AnchorRIGHT | TextNode::AnchorBOTTOM );
            break;
        }
    }
}

//  1     2     3
//  4     5     6
//  7     8     9
void text_justify ( int id, int justify )
{
    TextNode* node = NodeMgr::getInst().getNodePtr<TextNode> (id);
    if ( node )
    {
        switch ( justify )
        {
        case 1:
            node->setAlignFlag ( TextNode::AlignLeft & TextNode::AlignTop );
            break;
        case 2:
            node->setAlignFlag ( TextNode::AlignTop );
            break;
        case 3:
            node->setAlignFlag ( TextNode::AlignRight & TextNode::AlignTop );
            break;
        case 4:
            node->setAlignFlag ( TextNode::AlignLeft );
            break;
        case 5:
            node->setAlignFlag ( TextNode::AlignCenter );
            break;
        case 6:
            node->setAlignFlag ( TextNode::AlignRight );
            break;
        case 7:
            node->setAlignFlag ( TextNode::AlignLeft & TextNode::AlignBottom );
            break;
        case 8:
            node->setAlignFlag ( TextNode::AlignBottom );
            break;
        case 9:
            node->setAlignFlag ( TextNode::AlignRight & TextNode::AlignBottom );
            break;
        }
    }
}

// font
void font_create ( int id, const char* desc )
{
    FontNode* t = NodeMgr::getInst().addNode<FontNode> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "font node create: id is invalid. id: " << id;
        throw invalid_argument ( ss.str() );
    }
    t->setDesc ( desc );
}

void font_family ( int id, const char* f )
{
    FontNode* node = NodeMgr::getInst().getNodePtr<FontNode> (id);
    if ( node )
        node->family ( f );
}

void font_size ( int id, int sz )
{
    FontNode* node = NodeMgr::getInst().getNodePtr<FontNode> (id);
    if ( node )
        node->pointSize ( sz );
}

/*PLAIN = 1, BOLD = 2, ITALIC = 3, BOLDITALIC = 4*/
void font_style ( int /*id*/, int /*style*/ )
{
    /* FontNode* node = NodeMgr::getInst().getNodePtr<FontNode> (id); */
    /* if ( node ) */
}

void font_name ( int id, const char* name )
{
    FontNode* node = NodeMgr::getInst().getNodePtr<FontNode> (id);
    if ( node )
        node->defName ( name );
}

void get_font_desc ( int fontid, char* buffer )
{
    FontNode* node = NodeMgr::getInst().getNodePtr<FontNode> (fontid);
    if ( node )
    {
        string s = node->getDesc();
        strcpy ( buffer, s.c_str() );
    }
}
#include <tinylog.h>
// root
//   |-transform
//        |- mesh
int scene_load ( const char* file, int* data )
{
    // load mesh
    LoadScene loadscene ( file, true, true );
    
    for ( list<SGNode*>::iterator pp=loadscene.begin(); pp!=loadscene.end(); ++pp )
        *data++ = (*pp)->getID();
    return loadscene.size();
    //return loadscene.root()->getID();
    //int seed = SeedGenerator::getInst().seed();
    //NodeMgr::getInst()[seed] = loadmesh.root();
    //add_child ( 0, seed );
    //return seed;
    //return loadmesh.root(); 
    //return 0;
}

int node_load ( const char* file, int* data )
{
    // load mesh
    NodeMgr::getInst();
    LoadScene loadscene ( file, true, true, true );
    for ( list<SGNode*>::iterator pp=loadscene.begin(); pp!=loadscene.end(); ++pp )
        *data++ = (*pp)->getID();
    return loadscene.size();
    //int seed = SeedGenerator::getInst().seed();
    //NodeMgr::getInst()[seed] = loadmesh.root();
    //add_child ( 0, seed );
    //return seed;
    //return loadmesh.root(); 
    //return 0;
}

void node_save ( const char* file, int nodeid )
{
    SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>(nodeid);
    if ( node )
        SaveMesh saver ( file, node );
}

void unload_node ( int id )
{
    SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>( id );
    if ( node )
    {
        node->setParentNode ( NULL );
        UnloadNode unloadnode ( node );
        NodeMgr::getInst().erase ( id );
    }
}

void scene_translate ( int id, float tx, float ty, float tz )
{
    // get transform node
    transform_translate3f ( id, tx, ty, tz );
}

void scene_scale ( int id, float scale )
{
    transform_scale ( id, scale, scale, scale );
}

int pick ( float x, float y, float z, int camid, int* data )
{
//    BBox box ( vec3f(x, y, z), vec3f(x+1, y+1, z+1) );
    BBox box ( vec3f(x-0.01, y-0.01, z-0.01), vec3f(x+0.01, y+0.01, z+0.01) );
    RenderList renderlist;
    VolumePicker<back_insert_iterator<RenderList> > picker ( box, camid, back_inserter(renderlist) );
    SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>(0);
    picker ( *node );

    //int* pData = data;
    //int sz = renderlist.size();
    //for ( int i=0; i<sz; ++i )
    //    *pData++ = renderlist[i]->getID();

    //for ( int i=0; i<sz; ++i )
    //    delete renderlist[i];

    //return sz;

    int* pData = data;
    vector<SGNode*>::iterator pp, end = picker.pickedNodes().end();
    for ( pp=picker.pickedNodes().begin(); pp!=end; ++pp )
    {
        *pData++ = (*pp)->getID();
    }
    return picker.pickedNodes().size();
}

int pick_volume ( float x1, float y1, float z1, float x2, float y2, float z2, int camid, int* data )
{
    BBox box;
    box.init ( vec3f(x1, y1, z1) );
    box.expandby ( vec3f(x2, y2, z2) );
    RenderList renderlist;
    VolumePicker<back_insert_iterator<RenderList> > picker ( box, camid, back_inserter(renderlist) );
    SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>(0);
    picker ( *node );

    //int* pData = data;
    //int sz = renderlist.size();
    //for ( int i=0; i<renderlist.size(); ++i )
    //    *pData++ = renderlist[i]->getID();

    //for ( int i=0; i<sz; ++i )
    //    delete renderlist[i];

    //return sz;

    int* pData = data;
    vector<SGNode*>::iterator pp, end = picker.pickedNodes().end();
    for ( pp=picker.pickedNodes().begin(); pp!=end; ++pp )
    {
        *pData++ = (*pp)->getID();
    }
    return picker.pickedNodes().size();

}

int box_pick ( float x1, float y1, float z1, float x2, float y2, float z2, int camid, int* data )
{
    static BoxPicker picker;
    static float lx1, ly1, lz1, lx2, ly2, lz2;
    static int lcamid;

    const int ACTION_NONE                  = 0;
    const int ACTION_GETSIZE               = 1;
    const int ACTION_GETDATA_AFTER_GETSIZE = 2;
    const int ACTION_GETDATA               = 3;

    int state = ACTION_NONE;
    if ( NULL == data )
    {
        state = ACTION_GETSIZE;
    }
    else
    {
        if ( lx1 == x1 && ly1 == y1 && lz1 == z1 && lx2 == x2 && ly2 == y2 && lz2 == z2 && lcamid == camid )
            state = ACTION_GETDATA_AFTER_GETSIZE;
        else
            state = ACTION_GETDATA;
    }

    if ( ACTION_GETSIZE == state  )
    {
        BBox box;
        box.init ( vec3f(x1, y1, z1) );
        box.expandby ( vec3f(x2, y2, z2) );
        SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>(0);
        picker.initialize ( box, camid );
        picker.doAction ( *node );
        lx1 = x1; ly1 = y1; lz1 = z1; lx2 = x2; ly2 = y2; lz2 = z2; lcamid = camid;
    }

    if ( ACTION_GETDATA_AFTER_GETSIZE == state || ACTION_GETDATA == state )
    {
        int* pData = data;
        vector<SGNode*>::iterator pp, end = picker.pickedNodes().end();
        for ( pp=picker.pickedNodes().begin(); pp!=end; ++pp )
        {
            *pData++ = (*pp)->getID();
        }
    }

    return picker.pickedNodes().size();

}

int cross_pick ( float x1, float y1, float z1, float x2, float y2, float z2, int camid, int* data )
{
    static CrossPicker picker;
    static float lx1, ly1, lz1, lx2, ly2, lz2;
    static int lcamid;

    const int ACTION_NONE                  = 0;
    const int ACTION_GETSIZE               = 1;
    const int ACTION_GETDATA_AFTER_GETSIZE = 2;
    const int ACTION_GETDATA               = 3;

    int state = ACTION_NONE;
    if ( NULL == data )
    {
        state = ACTION_GETSIZE;
    }
    else
    {
        if ( lx1 == x1 && ly1 == y1 && lz1 == z1 && lx2 == x2 && ly2 == y2 && lz2 == z2 && lcamid == camid )
            state = ACTION_GETDATA_AFTER_GETSIZE;
        else
            state = ACTION_GETDATA;
    }

    if ( ACTION_GETSIZE == state  )
    {
        BBox box;
        box.init ( vec3f(x1, y1, z1) );
        box.expandby ( vec3f(x2, y2, z2) );
        SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>(0);
        picker.initialize ( box, camid );
        picker.doAction ( *node );
        lx1 = x1; ly1 = y1; lz1 = z1; lx2 = x2; ly2 = y2; lz2 = z2; lcamid = camid;
    }

    if ( ACTION_GETDATA_AFTER_GETSIZE == state || ACTION_GETDATA == state )
    {
        int* pData = data;
        vector<SGNode*>::iterator pp, end = picker.pickedNodes().end();
        for ( pp=picker.pickedNodes().begin(); pp!=end; ++pp )
        {
            *pData++ = (*pp)->getID();
        }
    }

    return picker.pickedNodes().size();
    //BBox box;
    //box.init ( vec3f(x1, y1, z1) );
    //box.expandby ( vec3f(x2, y2, z2) );

    //SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>(0);

    //CrossPicker picker ( box, camid );
    //picker.doAction ( *node );

    //int* pData = data;
    //vector<SGNode*>::iterator pp, end = picker.pickedNodes().end();
    //for ( pp=picker.pickedNodes().begin(); pp!=end; ++pp )
    //{
    //    *pData++ = (*pp)->getID();
    //}
    //return picker.pickedNodes().size();
}

void dump_node ( int id, const char* filename )
{
    SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>( id );
    if ( node ) 
    {
        NodeDumper dumper;
        dumper( node );
        ofstream out;
        out.open ( filename, ofstream::out | ofstream::app );
        out << dumper.dumpstring();
        out.close();
    }
}

void get_bbox ( int id, float* min, float* max )
{
    SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>( id );
    if ( node ) 
    {
        node->getBBox().minvec().xyz ( min );
        node->getBBox().maxvec().xyz ( max );
    }
}

// void get_scenepos ( int vpid, float* viewportCoord, float* sceneCoord, int camid )
// {
//     Viewport* node = NodeMgr::getInst().getNodePtr<Viewport>( vpid );
//     if ( node ) 
//     {
//         CameraOrtho* cameraNode;
//         if ( -1 == camid )
//         {
//             cameraNode = node->camera();
//         }
//         else
//         {
//             cameraNode = NodeMgr::getInst().getNodePtr<CameraOrtho>( camid );
//         }

//         if ( cameraNode )
//         {
//             vec4f rst = cameraNode->inversematrix() * node->inversematrix() * vec4f ( vec3f(viewportCoord) );
//             rst.xyz().xyz ( sceneCoord );
//         }
//     }
// }

// void get_viewportpos ( int vpid, float* sceneCoord, float* viewportCoord, int camid )
// {
//     Viewport* node = NodeMgr::getInst().getNodePtr<Viewport>( vpid );
//     if ( node ) 
//     {
//         CameraOrtho* cameraNode;
//         if ( -1 == camid )
//             cameraNode = node->camera();
//         else
//             cameraNode = NodeMgr::getInst().getNodePtr<CameraOrtho>( camid );

//         if ( cameraNode )
//         {
//             vec4f rst = node->vpmatrix() * cameraNode->mvmatrix() * vec4f ( vec3f(sceneCoord) );
//             rst.xyz().xyz ( viewportCoord );
//         }
//     }
// }


int get_nodetype ( int nodeid )
{
    SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>( nodeid );
    if ( dynamic_cast<CameraOrtho*>(node) != NULL )
        return NODETYPE_CAMERA;
    else if ( dynamic_cast<Viewport*>(node) != NULL )
        return NODETYPE_VIEWPORT;
    else if ( dynamic_cast<MeshNode3f*>(node) != NULL )
        return NODETYPE_MESH;
    else if ( dynamic_cast<MeshLineNode*>(node) != NULL )
        return NODETYPE_MESHLINE;
    else if ( dynamic_cast<MeshPointNode*>(node) != NULL )
        return NODETYPE_MESHPOINT;
    else if ( dynamic_cast<ColorNode*>(node) != NULL )
        return NODETYPE_COLOR;
    else if ( dynamic_cast<AttrSet*>(node) != NULL )
        return NODETYPE_ATTRSET;
    else if ( dynamic_cast<SceneNode*>(node) != NULL )
        return NODETYPE_SCENE;
    else if ( dynamic_cast<LayerNode*>(node) != NULL )
        return NODETYPE_LAYER;
    else if ( dynamic_cast<LODNode*>(node) != NULL )
        return NODETYPE_LOD;
    else if ( dynamic_cast<KdTreeNode*>(node) != NULL )
        return NODETYPE_KDTREE;
    else if ( dynamic_cast<ArrayNode*>(node) != NULL )
        return NODETYPE_ARRAY;
    else if ( dynamic_cast<RectangleNodef*>(node) != NULL )
        return NODETYPE_RECTANGLE;
    else if ( dynamic_cast<TransformNode*>(node) != NULL )
        return NODETYPE_TRANSFORM;
    else if ( dynamic_cast<PickableGroup*>(node) != NULL )
        return NODETYPE_PICKABLEGROUP;
    else if ( dynamic_cast<SwitchNode*>(node) != NULL )
        return NODETYPE_SWITCH;
    else if ( dynamic_cast<GroupNode*>(node) != NULL )
        return NODETYPE_GROUP;
    else if ( dynamic_cast<TextNode*>(node) != NULL )
        return NODETYPE_TEXT;
    else if ( dynamic_cast<FontNode*>(node) != NULL )
        return NODETYPE_FONT;
    else if ( dynamic_cast<LineNodef*>(node) != NULL )
        return NODETYPE_LINE;
    else if ( dynamic_cast<PolylineNode2Df*>(node) != NULL )
        return NODETYPE_POLYLINE;
    else if ( dynamic_cast<PolyNode2Df*>(node) != NULL )
        return NODETYPE_POLY;
    else if ( dynamic_cast<PointNode*>(node) != NULL )
        return NODETYPE_POINT;
    else
        return NODETYPE_UNKNOWN;
   
}

int get_nodeparent ( int nodeid )
{
    SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>( nodeid );
    if ( node )
    {
        SGNode* p = node->getParentNode();
        if ( p )
            return p->getID();
    }
    
    return -1;
}

void get_nodechildren ( int nodeid, int* data )
{
    SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>( nodeid );
    if ( node )
    {
        for ( SGNode::iterator pp=node->begin(); pp!=node->end(); ++pp )
        {
            *(data++) = (*pp)->getID();
        }
    }
}

int get_nodechildrenN ( int nodeid )
{
    SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>( nodeid );
    if ( node )
    {
        return node->size();
    }
    return -1;
}

template < class T >
int get_specchildrenid ( SGNode* node, int* data )
{
    ChildrenFinder<T> finder(node);
    for ( list<SGNode*>::iterator pp=finder.results().begin(); pp!=finder.results().end(); ++pp )
        *(data++) = (*pp)->getID();
    return finder.results().size();
}

int get_specnodechildren ( int nodeid, int nodetype, int* data )
{
    SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>( nodeid );
    if ( node )
    {
        switch ( nodetype )
        {
        case NODETYPE_CAMERA:
            return get_specchildrenid<SGR::CameraOrtho> ( node, data );
        case NODETYPE_VIEWPORT:
            return get_specchildrenid<SGR::Viewport> ( node, data );
        case NODETYPE_MESH:
            return get_specchildrenid<SGR::MeshNode3f> ( node, data );
        case NODETYPE_MESHLINE:
            return get_specchildrenid<SGR::MeshLineNode> ( node, data );
        case NODETYPE_MESHPOINT:
            return get_specchildrenid<SGR::MeshPointNode> ( node, data );
        case NODETYPE_COLOR:
            return get_specchildrenid<SGR::ColorNode> ( node, data );
        case NODETYPE_ATTRSET:
            return get_specchildrenid<SGR::AttrSet> ( node, data );
        case NODETYPE_SCENE:
            return get_specchildrenid<SGR::SceneNode> ( node, data );
        case NODETYPE_LAYER:
            return get_specchildrenid<SGR::LayerNode> ( node, data );
        case NODETYPE_LOD:
            return get_specchildrenid<SGR::LODNode> ( node, data );
        case NODETYPE_KDTREE:
            return get_specchildrenid<SGR::KdTreeNode> ( node, data );
        case NODETYPE_ARRAY:
            return get_specchildrenid<SGR::ArrayNode> ( node, data );
        case NODETYPE_RECTANGLE:
            return get_specchildrenid<SGR::RectangleNodef> ( node, data );
        case NODETYPE_TRANSFORM:
            return get_specchildrenid<SGR::TransformNode> ( node, data );
        case NODETYPE_PICKABLEGROUP:
            return get_specchildrenid<SGR::PickableGroup> ( node, data );
        case NODETYPE_SWITCH:
            return get_specchildrenid<SGR::SwitchNode> ( node, data );
        case NODETYPE_GROUP:
            return get_specchildrenid<SGR::GroupNode> ( node, data );
        case NODETYPE_TEXT:
            return get_specchildrenid<SGR::TextNode> ( node, data );
        case NODETYPE_FONT:
            return get_specchildrenid<SGR::FontNode> ( node, data );
        case NODETYPE_LINE:
            return get_specchildrenid<SGR::LineNodef> ( node, data );
        case NODETYPE_POLYLINE:
            return get_specchildrenid<SGR::Polyline2Df> ( node, data );
        case NODETYPE_POLY:
            return get_specchildrenid<SGR::PolyNode2Df> ( node, data );
        case NODETYPE_POINT:
            return get_specchildrenid<SGR::PointNode> ( node, data );
        default:
            return 0;
        }
    }
    return 0;
}

void line_create ( int id )
{
    LineNodef* t = NodeMgr::getInst().addNode<LineNodef> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "line node create: id is invalid. id: " << id;
        throw invalid_argument ( ss.str() );
    }
}

void line_points ( int id, float x1, float y1, float x2, float y2 )
{
    LineNodef* line = NodeMgr::getInst().getNodePtr<LineNodef>(id);
    if ( line )
    {
        line->setPoints ( x1, y1, x2, y2 );
    }
}

void line_point ( int id, float x, float y, bool isFirstPoint )
{
    LineNodef* line = NodeMgr::getInst().getNodePtr<LineNodef>(id);
    if ( line )
    {
        if ( isFirstPoint )
            line->point1 ( x, y );
        else
            line->point2 ( x, y );
    }
}

void get_line_point ( int id, float* xy, bool isFirst )
{
    LineNodef* line = NodeMgr::getInst().getNodePtr<LineNodef>(id);
    if ( line )
    {
        if ( isFirst )
        {
            xy[0] = line->point1().x ();
            xy[1] = line->point1().y ();
        }
        else
        {
            xy[0] = line->point2().x ();
            xy[1] = line->point2().y ();
        }
    }
}


void polyline_create ( int id )
{
    PolylineNode2Df* t = NodeMgr::getInst().addNode<PolylineNode2Df> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "polyline node create: id is invalid. id: " << id;
        throw invalid_argument ( ss.str() );
    }
}

void polyline_points ( int id, float* xycoords, int pointcnt )
{
    PolylineNode2Df* line = NodeMgr::getInst().getNodePtr<PolylineNode2Df>(id);
    if ( line )
    {
        vec2f* p = (vec2f*)xycoords;
        line->assignpoints ( p, p+pointcnt);
    }
}

void poly_create ( int id )
{
    PolyNode2Df* t = NodeMgr::getInst().addNode<PolyNode2Df> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "poly node create: id is invalid. id: " << id;
        throw invalid_argument ( ss.str() );
    }
}

void poly_points ( int id, float* xycoords, int pointcnt )
{
    PolyNode2Df* line = NodeMgr::getInst().getNodePtr<PolyNode2Df>(id);
    if ( line )
    {
        vec2f* p = (vec2f*)xycoords;
        line->assignpoints ( p, p+pointcnt);
    }
}

void point_create ( int id )
{
    PointNode* t = NodeMgr::getInst().addNode<PointNode> (id);
    if ( NULL == t )
    {
        stringstream ss;
        ss << "point node create: id is invalid. id: " << id;
        throw invalid_argument ( ss.str() );
    }
}

void point_coord ( int id, float x, float y )
{
    PointNode* pnt = NodeMgr::getInst().getNodePtr<PointNode>(id);
    if ( pnt )
    {
        pnt->xy ( x, y );
    }
}

void point_size ( int id, float sz )
{
    PointNode* pnt = NodeMgr::getInst().getNodePtr<PointNode>(id);
    if ( pnt )
    {
        pnt->pointSize ( sz );
    }
}

float get_point_size ( int id )
{
    PointNode* pnt = NodeMgr::getInst().getNodePtr<PointNode>(id);
    if ( pnt )
    {
        return pnt->pointSize ();
    }
    return -1.0f;
}

void get_point_coord ( int id, float* xyz )
{
    PointNode* pnt = NodeMgr::getInst().getNodePtr<PointNode>(id);
    if ( pnt )
    {
        xyz[0] = pnt->x();
        xyz[1] = pnt->y();
        xyz[2] = pnt->z();
    }
}

