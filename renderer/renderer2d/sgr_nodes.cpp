#include "sgr_nodes.h"
#include <cstdlib>
#include <cstring>
#include <map>
#include <list>
#include <iostream>
#include <sstream>
#include <mat4f.h>
using namespace std;
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
}

string SLCSceneNode::toXML () const
{
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
    foreground_color = vec3i ( 0, 0, 0 );
    background_color = vec3i ( 0, 0, 0 );
    linetype = 0xFFFF;//LINETYPE_SOLID;
    linewidth = 0;
    fontfilename = "";
    texturefilename = "";
}

string SLCMaterial::toXML () const
{
    stringstream ss;
    ss << "<material id=\"" << name << "\">" << endl;
    ss << "<color foregroud_color=\"" << foreground_color.x() << ' ' << foreground_color.y() << ' ' << foreground_color.z() << "\" background_color=\"" << background_color.x() << ' ' << background_color.y() << ' ' << background_color.z() << "\"/>" << endl;
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

// --------------------------------------------------------------------------------

string SLCLineNode::toXML () const
{
    stringstream ss;
    ss << "<line>" << pnts[0].x() << ' ' << pnts[0].y() << ' ' << pnts[0].z() << ' ' << 
	pnts[1].x() << ' ' << pnts[1].y() << ' ' << pnts[1].z() << "</line>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

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

string SLCRectNode::toXML () const
{
    stringstream ss;
    ss << "<primitive type=\"rect\" bindmaterial=\"" << bindmat->name << "\">" << 
	pnts[0].x() << ' ' << pnts[0].y() << ' ' << z << ' ' <<
	pnts[1].x() << ' ' << pnts[1].y() << ' ' << z << ' ' <<
	pnts[2].x() << ' ' << pnts[2].y() << ' ' << z << ' ' <<
	pnts[3].x() << ' ' << pnts[3].y() << ' ' << z << "</primitive>" << endl;
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

string SLCTextNode::toXML () const
{
    stringstream ss;
    ss << "<primitive type=\"text\" bindmaterial=\"" << bindmat->name << "\" pos=\"" << 
	pos.x() << ' ' << pos.y() << ' ' << pos.z() << "\" scale=\"" << scale << "\" rotz=\"" << rotz <<
	"\">" << text << "</primitive>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

SLCPLineNode::SLCPLineNode ( SLCMaterial* mat ) : SLCPrimitiveNode (mat)
{
    z = 0.0f;
}

string SLCPLineNode::toXML () const
{
    stringstream ss;
    ss << "<primitive type=\"pline\" bindmaterial=\"" << bindmat->name << "\">";
    
    for ( vector<vec2f>::const_iterator pp=pnts.begin(); pp!=pnts.end(); ++pp )
	ss << pp->x() << ' ' << pp->y() << ' ' << z << ' ';
    ss <<"</primitive>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

SLCPolyNode::SLCPolyNode ( SLCMaterial* mat ) : SLCFillablePrimitiveNode (mat)
{
    z = 0.0f;
}

string SLCPolyNode::toXML () const
{
    stringstream ss;
    ss << "<primitive type=\"poly\" bindmaterial=\"" << bindmat->name << "\" " << SLCFillablePrimitiveNode::toXML () << ">";
    for ( vector<vec2f>::const_iterator pp=pnts.begin(); pp!=pnts.end(); ++pp )
	ss << pp->x() << ' ' << pp->y() << ' ' << z << ' ';
    ss <<"</primitive>" << endl;
    return ss.str();
}
 
// --------------------------------------------------------------------------------

SLCTransformNode::SLCTransformNode () : SLCNode()
{
    mat_loadidentity ( mat );
}

string SLCTransformNode::toXML () const
{
    stringstream ss;
    ss << "<transform mat=\"" << 
	mat[0] << ' ' << mat[1] << ' ' << mat[2] << ' ' << mat[3] << ' ' << 
	mat[4] << ' ' << mat[5] << ' ' << mat[6] << ' ' << mat[7] << ' ' << 
	mat[8] << ' ' << mat[9] << ' ' << mat[10] << ' ' << mat[11] << ' ' << 
	mat[12] << ' ' << mat[13] << ' ' << mat[14] << ' ' << mat[15] << ' ' << "\">" << endl;

    for ( list<SLCNode*>::const_iterator pp=children.begin(); pp!=children.end(); ++pp )
	ss << (*pp)->toXML();

    ss <<"</transform>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

