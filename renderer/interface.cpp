#include "interface.h"
#include "nodemgr.h"
#include "nodes.h"
#include "vec3.h"
#include "renderfunctor.h"
#include "renderflow.h"
#include "savemesh.h"
#include "loadmesh.h"
#include "qviewport.h"
#include "arrayexpander.h"
#include "volumepicker.h"
#include "nodedumper.h"
//#include "colorchanger.h"
#include "colornode.h"
#include "attrset.h"

#include <fstream>
#include <stdexcept>

using namespace std;
// camera management

void init_windowsystem ()
{
    QViewport::getInst().show();
    //    QViewport::getInst().show();
}

int camera_create ( const char* name )
{
    // new camera
    // add it to manager
    return NodeMgr::getInst().addNode<CameraOrtho> ();
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

float find_view ( float* min, float* max, float percentOfView, int camid, int vpid )
{
    vec3f a ( min ), b(max);
    BBox bbox ( a, b );
    //BBox bbox ( vec3f(min), vec3f(max) );
    vec3f center = bbox.center();
    float maxDimension = bbox.dimension().max_element();
    if ( 0 == maxDimension )
    {
        stringstream ss;
        ss << "min max should be different";
        throw std::invalid_argument ( ss.str().c_str() );
    }
    float scale        = 2 * percentOfView / maxDimension;

    camera_reset ( camid );
    camera_translate ( camid, center.x(), center.y(), center.z() );
    camera_scale ( camid, scale );
    sgwindow_update ( vpid );
    return scale;
}

int viewport_create ( const char* name )
{
    int id = NodeMgr::getInst().addNode<Viewport> ();
    viewport_name ( id, name );
    return id;
}

void viewport_geometry ( int id, int x, int y, int w, int h )
{
    Viewport* p = NodeMgr::getInst().getNodePtr<Viewport> (id);
    if ( p )
    {
        p->position ( x, y );
        p->size ( w, h );
    }
}

void viewport_attachcamera ( int id, int camid )
{
    Viewport* p = NodeMgr::getInst().getNodePtr<Viewport> (id);
    if ( p )
        p->attachcamera ( camid );
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
        RenderOption opt;
        opt.painter = &painter;
        RenderFlow renderflow ( *p, opt );
        p->update ();
    }
}

void viewport_dirty ( int id )
{
    Viewport* p = NodeMgr::getInst().getNodePtr<Viewport> (id);
    if ( p )
        p->dirty ( true );
}

void viewport_add ( int id )
{
    QViewport::getInst().add_viewport( id );
}

void viewport_delete ( int id )
{
    QViewport::getInst().remove_viewport( id );
}

void sgwindow_update ( int id )
{
    if ( id < 0 )
    {
        QViewport::getInst().dirty_all();
        QViewport::getInst().update();
    }
    else
    {
        viewport_dirty ( id );
        QViewport::getInst().update();
    }
}

void add_child ( int parent, int child )
{
    NodeMgr& nv = NodeMgr::getInst();
    NodeMgr::iterator pp1 = nv.find ( parent );
    NodeMgr::iterator pp2 = nv.find ( child );
    if ( pp1!=nv.end() && pp2!=nv.end() )
        pp1->second->addChild ( pp2->second );
}

void remove_child ( int parent, int child )
{
    NodeMgr& nv = NodeMgr::getInst();
    NodeMgr::iterator pp1 = nv.find ( parent );
    NodeMgr::iterator pp2 = nv.find ( child );
    if ( pp1!=nv.end() && pp2!=nv.end() )
        pp1->second->removeChild ( pp2->second );
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
    NodeMgr::iterator pp = NodeMgr::getInst().find(id);
    if ( pp != NodeMgr::getInst().end() )
    {
        SGNode* node = pp->second;
        node->removeAllChild ();
        node->setParentNode ( NULL );
        delete node;
        NodeMgr::getInst().erase ( id );
    }
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

int color_create ( unsigned int color )
{
    return NodeMgr::getInst().addNode<ColorNode> (color);
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

int attrset_create ( int layerid )
{
    LayerNode* node = NodeMgr::getInst().getNodePtr<LayerNode>( layerid );
    if ( node )
    {
        int renderOrder = LayerMgr::getInst().indexof(node);
        return NodeMgr::getInst().addNode<AttrSet> (renderOrder);
    }
    stringstream ss;
    ss << "layer node not exist or id " << layerid << " is not a layer";
    throw std::invalid_argument ( ss.str().c_str() );
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

void set_attrset ( int nodeid, int attrsetid )
{
    DrawableNode* node = NodeMgr::getInst().getNodePtr<DrawableNode>( nodeid );
    if ( node )
    {
        AttrSet* attrsetNode = NodeMgr::getInst().getNodePtr<AttrSet>( nodeid );
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
    {
        return attrsetNode->getRefCnt();
    }
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
int mesh_create ()
{
    return NodeMgr::getInst().addNode<MeshNode> ();
}

// layer
int layer_create ( const char* name )
{
    int id = NodeMgr::getInst().addNode<LayerNode> ();
    layer_name ( id, name );
    return id;
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

// lod
int lod_create ()
{
    return NodeMgr::getInst().addNode<LODNode> ();
}

void lod_delimiters ( int id, const char* delims )
{
    LODNode* lod = NodeMgr::getInst().getNodePtr<LODNode> ( id );
    if ( lod )
        lod->setdelimiters ( delims );
}

// kdtree
int kdtree_create ()
{
    return NodeMgr::getInst().addNode<KdTreeNode> ();
}

void kdtree_build ( int id )
{
    KdTreeNode* node = NodeMgr::getInst().getNodePtr<KdTreeNode>(id);
    if ( node )
        node->buildKdTree ();
}

// array
int array_create ()
{
    return NodeMgr::getInst().addNode<ArrayNode> ();
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
int rectangle_create ()
{
    return NodeMgr::getInst().addNode<RectangleNodef> ();
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
int transform_create ()
{
    return NodeMgr::getInst().addNode<TransformNode> ();
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
int pickablegroup_create ()
{
    return NodeMgr::getInst().addNode<PickableGroup> ();
}

void pickablegroup_name ( int id, const char* nm )
{
    PickableGroup* pick = NodeMgr::getInst().getNodePtr<PickableGroup> (id);
    if ( pick )
        pick->name ( nm );
}

// switchnode
int switchnode_create ()
{
    return NodeMgr::getInst().addNode<SwitchNode> ();
}

void switchnode_visible ( int id, bool isVisible )
{
    SwitchNode* node = NodeMgr::getInst().getNodePtr<SwitchNode> (id);
    if ( node )
        node->setVisible ( isVisible );
}

// groupnode
int groupnode_create ()
{
    return NodeMgr::getInst().addNode<GroupNode> ();
}

void groupnode_name ( int id, const char* nm )
{
    GroupNode* node = NodeMgr::getInst().getNodePtr<GroupNode> (id);
    if ( node )
        node->name ( nm );
}

int text_create ( const char* str )
{
    int id = NodeMgr::getInst().addNode<TextNode> ();
    text_string ( id, str );
    return id;
}

void text_string ( int id, const char* str )
{
    TextNode* node = NodeMgr::getInst().getNodePtr<TextNode> (id);
    if ( node )
        node->text ( str );
}

void text_font ( int id, int fontid )
{
    TextNode* node = NodeMgr::getInst().getNodePtr<TextNode> (id);
    if ( node )
    {
        FontNode* fnode = NodeMgr::getInst().getNodePtr<FontNode> (id);
        if ( fnode )
        {
            node->fontnode ( fnode );
        }
    }
}

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
int font_create ()
{
    return NodeMgr::getInst().addNode<FontNode> ();
}

void font_family ( int id, const char* f )
{
    FontNode* node = NodeMgr::getInst().getNodePtr<FontNode> (id);
    if ( node )
        node->family ( f );
}

void font_size ( int id, float sz )
{
    FontNode* node = NodeMgr::getInst().getNodePtr<FontNode> (id);
    if ( node )
        node->size ( sz );
}

/*PLAIN = 1, BOLD = 2, ITALIC = 3, BOLDITALIC = 4*/
void font_style ( int id, int style )
{
    /* FontNode* node = NodeMgr::getInst().getNodePtr<FontNode> (id); */
    /* if ( node ) */
}


// root
//   |-transform
//        |- mesh
int mesh_load ( const char* file )
{
    // load mesh
    NodeMgr::getInst();
    LoadMesh loadmesh ( file, true, true );
    int seed = SeedGenerator::getInst().seed();
    NodeMgr::getInst()[seed] = loadmesh.root();
    //add_child ( 0, seed );
    return seed;
    //return loadmesh.root(); 
    //return 0;
}

void mesh_save ( const char* file, int meshid )
{
    SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>(meshid);
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

void mesh_translate ( int id, float tx, float ty, float tz )
{
    // get transform node
    transform_translate3f ( id, tx, ty, tz );
}

void mesh_scale ( int id, float scale )
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

    int* pData = data;
    for ( int i=0; i<renderlist.size(); ++i )
        *pData++ = renderlist[i]->getID();

    return renderlist.size();
}

int pick_volume ( float minx, float miny, float minz, float maxx, float maxy, float maxz, int camid, int* data )
{
    BBox box ( vec3f(minx, miny, minz), vec3f(maxx, maxy, maxz) );
    RenderList renderlist;
    VolumePicker<back_insert_iterator<RenderList> > picker ( box, camid, back_inserter(renderlist) );
    SGNode* node = NodeMgr::getInst().getNodePtr<SGNode>(0);
    picker ( *node );

    int* pData = data;
    for ( int i=0; i<renderlist.size(); ++i )
        *pData++ = renderlist[i]->getID();

    return renderlist.size();
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
        node->getBBox().min().xyz ( min );
        node->getBBox().max().xyz ( max );
    }
}

void get_scenepos ( int vpid, float* viewportCoord, float* sceneCoord )
{
    Viewport* node = NodeMgr::getInst().getNodePtr<Viewport>( vpid );
    if ( node ) 
    {
        vec4f rst = node->camera()->inversematrix() * node->inversematrix() * vec4f ( vec3f(viewportCoord) );
        rst.xyz().xyz ( sceneCoord );
    }
}

void get_viewportpos ( int vpid, float* sceneCoord, float* viewportCoord )
{
    Viewport* node = NodeMgr::getInst().getNodePtr<Viewport>( vpid );
    if ( node ) 
    {
        vec4f rst = node->vpmatrix() * node->camera()->mvmatrix() * vec4f ( vec3f(sceneCoord) );
//        vec4f rst = node->camera()->inversematrix() * node->inversematrix() * vec4f ( vec3f(viewportCoord) );
        rst.xyz().xyz ( viewportCoord );
    }
}


int line_create ()
{
    return NodeMgr::getInst().addNode<LineNodef> ();
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
