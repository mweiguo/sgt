#include "sgr_nodes.h"
#include <cstdlib>
#include <cstring>
#include <map>
#include <list>
#include <iostream>
#include <sstream>
#include <mat4f.h>
#include <bbox2d.h>
using namespace std;

typedef struct {
    float mat[16];
} MyMat;
list<MyMat> _current_matrix_stack;
float _current_matrix[16];
void calcCurrentMatrix ()
{
    mat_loadidentity ( _current_matrix );
    for ( list<MyMat>::iterator pp=_current_matrix_stack.begin();
	  pp!=_current_matrix_stack.end(); ++pp )
    {
	mat_multmatrix ( _current_matrix, pp->mat );
    }
}

// --------------------------------------------------------------------------------

int SLCNode::getDepth()
{
    SLCNode* p = parent;
    int depth = 0;
    while ( p != 0 ) {
	if ( p->getType() != SLC_TRANSFORM )
	    depth++;
        p = p->parent;
    }
    return depth;
}

void SLCNode::addChild ( SLCNode* node )
{
    children.push_back ( node );
    node->parent = this;
}

void SLCNode::push_front_child ( SLCNode* node )
{
    children.push_front ( node );
    node->parent = this;
}

// --------------------------------------------------------------------------------

SLCSceneNode::SLCSceneNode ( const char* gname ) : SLCNode(), name(gname)
{
//     hightlight_mat = new SLCMaterial ("hightlight_mat");
//     hightlight_mat->background_color = vec4i ( 255, 255, 0, 255 );
//     addChild ( hightlight_mat );
}

SLCSceneNode::~SLCSceneNode ()
{
//    delete hightlight_mat;
}

string SLCSceneNode::toXML () const
{
    calcCurrentMatrix ();
    stringstream ss;
    ss << "<scene name=\"" << name << "\">" << endl;

    for ( list<SLCNode*>::const_iterator pp=children.begin(); pp!=children.end(); ++pp )
	ss << (*pp)->toXML();

    ss << "</scene>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

SLCMaterial::SLCMaterial ( const char* nm ) : SLCNode()
{
    name = nm;
    foreground_color = vec4i ( 0, 0, 0, 1 );
    background_color = vec4i ( 0, 0, 0, 1 );
    linetype = 0xFFFF;//LINETYPE_SOLID;
    linewidth = 1;
    fontfilename = "";
    texturefilename = "";
}

string SLCMaterial::toXML () const
{
    stringstream ss;
    ss << "<material id=\"" << name << "\">" << endl;
    ss << "<color foregroud_color=\"" << foreground_color.x() << ' ' << foreground_color.y() << ' ' << foreground_color.z() << ' ' << foreground_color.w() << "\" background_color=\"" << background_color.x() << ' ' << background_color.y() << ' ' << background_color.z()<< ' ' << background_color.w() << "\"/>" << endl;
    ss << "<line pattern=\"" << linetype << "\" factor=\"" << linetypefactor << "\" width=\"" << linewidth << "\"/>" << endl;
    if ( fontfilename != "" )
	ss << "<font path=\"" << fontfilename << "\"/>" << endl;
    if ( texturefilename != "" )
	ss << "<texture path=\"" << texturefilename << "\"/>" << endl;

    ss << "</material>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

SLCLayerNode::SLCLayerNode ( const char* lname, SLCMaterial* bindmaterial ) : SLCNode(), name(lname), bindmat(bindmaterial)
{
    visible = true;
    pickable = true;
}

string SLCLayerNode::toXML () const
{
    stringstream ss;
    ss << "<layer name=\"" << name << "\">" << endl;
    for ( list<SLCNode*>::const_iterator pp=children.begin(); pp!=children.end(); ++pp )
	ss << (*pp)->toXML();
    ss << "</layer>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

SLCLODNode::SLCLODNode () : SLCNode()
{
}

string SLCLODNode::toXML () const
{
    stringstream ss;
    ss << "<lod ";
    if ( false == scales.empty() ) 
    {
	ss << "\"";
	for ( list<float>::const_iterator pp=scales.begin(); pp!=scales.end(); ++pp )
	    ss << *pp << ' ';
	ss << "\"";
    }
    ss << ">" << endl;
    for ( list<SLCNode*>::const_iterator pp=children.begin(); pp!=children.end(); ++pp )
	ss << (*pp)->toXML();
    ss << "</lod>" << endl;
    return ss.str();
}


// --------------------------------------------------------------------------------

SLCLODPageNode::SLCLODPageNode () : SLCNode()
{
    delayloading = false;
    imposter = false;
    kdtree = "";
}

string SLCLODPageNode::toXML () const
{
    stringstream ss;
    ss << "<lodpage delayloading=\"" << delayloading << "\" imposter=\"" << imposter << "\"";
    if ( kdtree != "" )
	ss << " kdtree=\"" << kdtree << "\"";
    if ( children.empty() ) {
	ss << "/>" << endl;
	return ss.str();
    }
    ss << ">" << endl;

    for ( list<SLCNode*>::const_iterator pp=children.begin(); pp!=children.end(); ++pp )
	ss << (*pp)->toXML();

    ss << "</lodpage>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

SLCPrimitiveNode::SLCPrimitiveNode ( SLCMaterial* mat ) : SLCNode()
{
    bindmat = mat;
}

SLCPrimitiveNode::SLCPrimitiveNode ( const SLCPrimitiveNode& rhs )
{
    bindmat = rhs.bindmat;
}

// --------------------------------------------------------------------------------

string SLCLineNode::toXML () const
{
    stringstream ss;
    ss << "<line>" << pnts[0].x() << ' ' << pnts[0].y() << ' ' << pnts[0].z() << ' ' << 
	pnts[1].x() << ' ' << pnts[1].y() << ' ' << pnts[1].z() << "</line>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

SLCFillablePrimitiveNode::SLCFillablePrimitiveNode ( const SLCFillablePrimitiveNode& rhs )
    : SLCPrimitiveNode(rhs)
{
    textureScale = rhs.textureScale;
    textureAngle = rhs.textureAngle;
    filltexture  = rhs.filltexture;
}

string SLCFillablePrimitiveNode::toXML () const
{
    stringstream ss;
    if ( filltexture )
	ss << "filltexture=\"true\" textureScale=\"" << textureScale << "\" textureAngle=\"" << textureAngle << "\"";
    return ss.str();
}

// --------------------------------------------------------------------------------

string SLCTriNode::toXML () const
{
    stringstream ss;
    ss << "<triangle>" << 
	pnts[0].x() << ' ' << pnts[0].y() << ' ' << pnts[0].z() << ' ' << 
	pnts[1].x() << ' ' << pnts[1].y() << ' ' << pnts[1].z() << ' ' << 
	pnts[2].x() << ' ' << pnts[2].y() << ' ' << pnts[2].z() << ' ' << "</triangle>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

SLCRectNode::SLCRectNode ( SLCMaterial* mat ) 
    : SLCFillablePrimitiveNode ( mat )
{
    z = 0.0f;
}

SLCRectNode::SLCRectNode ( const SLCRectNode& rhs )
    : SLCFillablePrimitiveNode ( rhs )
{
    pnts[0] = rhs.pnts[0];
    pnts[1] = rhs.pnts[1];
    pnts[2] = rhs.pnts[2];
    pnts[3] = rhs.pnts[3];
    z = rhs.z;
}

string SLCRectNode::toXML () const
{
    stringstream ss;
    float p0[] = { pnts[0].x(), pnts[0].y(), z, 1 };
    mat_multvector ( _current_matrix, p0 );
    float p1[] = { pnts[2].x(), pnts[2].y(), z, 1 };
    mat_multvector ( _current_matrix, p1 );
    ss << "<primitive type=\"rect\" bindmaterial=\"" << bindmat->name << "\">" << 
	p0[0] << ' ' << p0[1] << ' ' << p1[0]-p0[0] << ' ' <<
	p1[1]-p0[1] << ' ' << 0 << "</primitive>" << endl;
    return ss.str();
}

SLCPrimitiveNode* SLCRectNode::copy()
{
    return new SLCRectNode (*this);
}

void SLCRectNode::getMinMax ( float* minmax )
{
    BBox2d box;
    box.init ( pnts[0] );
    box.expandby ( pnts[1] );
    box.expandby ( pnts[2] );
    box.expandby ( pnts[3] );
    minmax[0] = box.minvec().x();
    minmax[1] = box.minvec().y();
    minmax[2] = z;
    minmax[3] = box.maxvec().x();
    minmax[4] = box.maxvec().y();
    minmax[5] = z;
}

void SLCRectNode::setRect ( float x, float y, float zz, float w, float h )
{
    pnts[0] = vec3f(x,y,zz);
    pnts[1] = vec3f(x+w,y,zz);
    pnts[2] = vec3f(x+w,y+h,zz);
    pnts[3] = vec3f(x,y+h,zz);
}

void SLCRectNode::setSize ( float w, float h )
{
    float x = pnts[0].x();
    float y = pnts[0].y();
    float zz = z;
    pnts[1] = vec3f(x+w,y,zz);
    pnts[2] = vec3f(x+w,y+h,zz);
    pnts[3] = vec3f(x,y+h,zz);
}

// --------------------------------------------------------------------------------

SLCImageNode::SLCImageNode ()
    : SLCRectNode(NULL)
{
    texturefilename = "";
}

SLCImageNode::SLCImageNode ( const SLCImageNode& rhs )
    : SLCRectNode(rhs)
{
    texturefilename = rhs.texturefilename;
}

string SLCImageNode::toXML () const
{
    stringstream ss;
    float p0[] = { pnts[0].x(), pnts[0].y(), z, 1 };
    mat_multvector ( _current_matrix, p0 );
    float p1[] = { pnts[2].x(), pnts[2].y(), z, 1 };
    mat_multvector ( _current_matrix, p1 );
    ss << "<primitive type=\"image\" filename=\"" << texturefilename << "\">" << 
	p0[0] << ' ' << p0[1] << ' ' << p1[0]-p0[0] << ' ' <<
	p1[1]-p0[1] << ' ' << 0 << "</primitive>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

SLCTextNode::SLCTextNode ( SLCMaterial* mat ) : SLCPrimitiveNode (mat)
{
    pos.xyz(0, 0, 0);
    scale = 1;
    rotz = 0;
    text = "";
}

SLCTextNode::SLCTextNode ( const SLCTextNode& rhs )
    : SLCPrimitiveNode(rhs)
{
    pos = rhs.pos;
    scale = rhs.scale;
    rotz = rhs.rotz;
    text = rhs.text;
}

string SLCTextNode::toXML () const
{
    stringstream ss;
    float v[] = { pos.x(), pos.y(), pos.z(), 1 };
    mat_multvector ( _current_matrix, v );

    ss << "<primitive type=\"text\" bindmaterial=\"" << bindmat->name << "\" pos=\"" << 
	v[0] << ' ' << v[1] << ' ' << v[2] << "\" scale=\"" << scale << "\" rotz=\"" << rotz <<
	"\">" << text << "</primitive>" << endl;
    return ss.str();
}

SLCPrimitiveNode* SLCTextNode::copy()
{
    return new SLCTextNode(*this);
}

void SLCTextNode::getMinMax ( float* /*minmaxabc*/ )
{
}

// --------------------------------------------------------------------------------

SLCPLineNode::SLCPLineNode ( SLCMaterial* mat ) : SLCPrimitiveNode (mat)
{
    z = 0.0f;
}

SLCPLineNode::SLCPLineNode ( const SLCPLineNode& rhs )
    : SLCPrimitiveNode(rhs)
{
    pnts.assign ( rhs.pnts.begin(), rhs.pnts.end() );
    z = rhs.z;
}

string SLCPLineNode::toXML () const
{
    stringstream ss;
    ss << "<primitive type=\"pline\" bindmaterial=\"" << bindmat->name << "\">";
    
    for ( vector<vec2f>::const_iterator pp=pnts.begin(); pp!=pnts.end(); ++pp ) {
	float v[] = { pp->x(), pp->y(), z, 1 };
	mat_multvector ( _current_matrix, v );
	ss << v[0] << ' ' << v[1] << ' ' << v[2] << ' ';
    }
    ss <<"</primitive>" << endl;
    return ss.str();
}

SLCPrimitiveNode* SLCPLineNode::copy()
{
    return new SLCPLineNode(*this);
}

void SLCPLineNode::getMinMax ( float* minmax )
{
    BBox2d box;
    vector<vec2f>::iterator pp=pnts.begin();
    box.init ( *pp );

    for ( ++pp; pp!=pnts.end(); ++pp )
	box.expandby ( *pp );

    minmax[0] = box.minvec().x();
    minmax[1] = box.minvec().y();
    minmax[2] = z;
    minmax[3] = box.maxvec().x();
    minmax[4] = box.maxvec().y();
    minmax[5] = z;
}

// --------------------------------------------------------------------------------

SLCPolyNode::SLCPolyNode ( SLCMaterial* mat ) : SLCFillablePrimitiveNode (mat)
{
    z = 0.0f;
}

SLCPolyNode::SLCPolyNode ( const SLCPolyNode& rhs )
    : SLCFillablePrimitiveNode(rhs)
{
    pnts.assign(rhs.pnts.begin(), rhs.pnts.end());
    z = rhs.z;
}

string SLCPolyNode::toXML () const
{
    stringstream ss;
    ss << "<primitive type=\"poly\" bindmaterial=\"" << bindmat->name << "\" " << SLCFillablePrimitiveNode::toXML () << ">";
    for ( vector<vec2f>::const_iterator pp=pnts.begin(); pp!=pnts.end(); ++pp ) {
	float v[] = { pp->x(), pp->y(), z, 1 };
	mat_multvector ( _current_matrix, v );
	ss << v[0] << ' ' << v[1] << ' ' << v[2] << ' ';
    }
//	ss << pp->x() << ' ' << pp->y() << ' ' << z << ' ';
    ss <<"</primitive>" << endl;
    return ss.str();
}
 
SLCPrimitiveNode* SLCPolyNode::copy()
{
    return new SLCPolyNode(*this);
}

void SLCPolyNode::getMinMax ( float* minmax )
{
    BBox2d box;
    vector<vec2f>::iterator pp=pnts.begin();
    box.init ( *pp );

    for ( ++pp; pp!=pnts.end(); ++pp )
	box.expandby ( *pp );

    minmax[0] = box.minvec().x();
    minmax[1] = box.minvec().y();
    minmax[2] = z;
    minmax[3] = box.maxvec().x();
    minmax[4] = box.maxvec().y();
    minmax[5] = z;
}

// --------------------------------------------------------------------------------

SLCSmartTilesNode::SLCSmartTilesNode ( SLCMaterial* mat ) : SLCPrimitiveNode (mat)
{
    z = 0;
    levelcnt = 0;
}

SLCSmartTilesNode::SLCSmartTilesNode ( const SLCSmartTilesNode& rhs )
    : SLCPrimitiveNode(rhs)
{
    pnts[0] = rhs.pnts[0];
    pnts[1] = rhs.pnts[1];
    pnts[2] = rhs.pnts[2];
    pnts[3] = rhs.pnts[3];
    z = rhs.z;
    levelcnt = rhs.levelcnt;
}

string SLCSmartTilesNode::toXML () const
{
    stringstream ss;
    ss << "<primitive type=\"smartiles\" bindmaterial=\"" << bindmat->name << "\" levelcnt=\"" << levelcnt << "\">";
    for ( int i=0; i<4; i++ ) {
	float v[] = { pnts[i].x(), pnts[i].y(), z, 1 };
	mat_multvector ( _current_matrix, v );
	ss << v[0] << ' ' << v[1] << ' ' << v[2] << ' ';
    }
    ss <<"</primitive>" << endl;
    return ss.str();
}

SLCPrimitiveNode* SLCSmartTilesNode::copy()
{
    return new SLCSmartTilesNode(*this);
}

void SLCSmartTilesNode::getMinMax ( float* minmax )
{
    BBox2d box;
    box.init ( pnts[0] );
    box.expandby ( pnts[1] );
    box.expandby ( pnts[2] );
    box.expandby ( pnts[3] );
    minmax[0] = box.minvec().x();
    minmax[1] = box.minvec().y();
    minmax[2] = z;
    minmax[3] = box.maxvec().x();
    minmax[4] = box.maxvec().y();
    minmax[5] = z;
}

// --------------------------------------------------------------------------------

SLCTransformNode::SLCTransformNode () : SLCNode()
{
    mat_loadidentity ( mat );
}

string SLCTransformNode::toXML () const
{
    MyMat m;
    memcpy ( m.mat, mat, sizeof(float)*16 );
    _current_matrix_stack.push_back ( m );
    calcCurrentMatrix ();

    stringstream ss;
//     ss << "<transform mat=\"" << 
// 	mat[0] << ' ' << mat[1] << ' ' << mat[2] << ' ' << mat[3] << ' ' << 
// 	mat[4] << ' ' << mat[5] << ' ' << mat[6] << ' ' << mat[7] << ' ' << 
// 	mat[8] << ' ' << mat[9] << ' ' << mat[10] << ' ' << mat[11] << ' ' << 
// 	mat[12] << ' ' << mat[13] << ' ' << mat[14] << ' ' << mat[15] << ' ' << "\">" << endl;

    for ( list<SLCNode*>::const_iterator pp=children.begin(); pp!=children.end(); ++pp )
	ss << (*pp)->toXML();

    _current_matrix_stack.pop_back ();
    calcCurrentMatrix ();
//     ss <<"</transform>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

