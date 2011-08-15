#include "sgr_nodes.h"
#include <cstdlib>
#include <cstring>
#include <map>
#include <iostream>
#include <sstream>
using namespace std;
// --------------------------------------------------------------------------------

int SLCNode::getDepth()
{
    SLCNode* p = parent;
    int depth = 0;
    while ( p != 0 ) {
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

// --------------------------------------------------------------------------------

SLCGroupNode::SLCGroupNode ( const char* gname ) : SLCNode()
{
    memset ( name, 0, sizeof(char)*32 );
    strncpy ( name, gname, 32 );
}

string SLCGroupNode::toXML () const
{
    stringstream ss;
    ss << "<group name=\"" << name << "\">" << endl;

    for ( list<SLCNode*>::const_iterator pp=children.begin(); pp!=children.end(); ++pp )
	ss << (*pp)->toXML();

    ss << "</group>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

SLCLayerNode::SLCLayerNode ( const char* lname ) : SLCNode()
{
    memset ( name, 0, sizeof(char)*32 );
    strncpy ( name, lname, 32 );
}

string SLCLayerNode::toXML () const
{
    stringstream ss;
    ss << "<layer name=\"" << name << "\">";
    for ( list<SLCNode*>::const_iterator pp=children.begin(); pp!=children.end(); ++pp )
	ss << (*pp)->toXML();
    ss << "</layer>" << endl;
    return ss.str();
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

string SLCQuadNode::toXML () const
{
    stringstream ss;
    ss << "<quad>" << 
	pnts[0].x() << ' ' << pnts[0].y() << ' ' << pnts[0].z() << ' ' << 
	pnts[1].x() << ' ' << pnts[1].y() << ' ' << pnts[1].z() << ' ' << 
	pnts[2].x() << ' ' << pnts[2].y() << ' ' << pnts[2].z() << ' ' <<
	pnts[3].x() << ' ' << pnts[3].y() << ' ' << pnts[3].z() << ' ' << "</quad>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

SLCModel::SLCModel ( const char* Name )
{
    memset ( name, 0, sizeof(char)*32 );
    strncpy ( name, Name, 32 );
}

string SLCModel::toXML () const
{
    stringstream ss;
    ss << "<model name=\"" << name << "\">" << endl;
    for ( list<SLCNode*>::const_iterator pp=children.begin(); pp!=children.end(); ++pp )
	ss << (*pp)->toXML();
    ss << "</model>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

SLCTexture::SLCTexture ( const char* Name, const char* fileName ) : SLCNode()
{
    memset ( name,     0, sizeof(char)*32 );
    memset ( filename, 0, sizeof(char)*32 );
    
    strncpy ( name,     Name,     32 );
    strncpy ( filename, fileName, 32 );

}

string SLCTexture::toXML () const
{
    stringstream ss;
    ss << "<texture name=\"" << name << "\" filename=\"" << filename << "\">";
    for ( list<SLCNode*>::const_iterator pp=children.begin(); pp!=children.end(); ++pp )
	ss << (*pp)->toXML();
    ss << "</texture>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

string SLCAppearance::toXML () const
{
    stringstream ss;
    ss << "<appearance>" << endl;
    for ( list<SLCNode*>::const_iterator pp=children.begin(); pp!=children.end(); ++pp )
	ss << (*pp)->toXML();
    ss << "</appearance>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

string SLCMaterial::toXML () const
{
    stringstream ss;
    ss << "<material diffuse=\"" 
       << diffuse.x() << ' ' << diffuse.y() << ' ' << diffuse.z() << "\" ambient=\""
       << ambient.x() << ' ' << ambient.y() << ' ' << ambient.z() << "\" specular\""
       << specular.x() << ' ' << specular.y() << ' ' << specular.z() << "\"/>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

string SLCMesh::toXML () const
{
    stringstream ss;
    ss << "<mesh>" << endl;
    for ( list<SLCNode*>::const_iterator pp=children.begin(); pp!=children.end(); ++pp )
	ss << (*pp)->toXML();
    ss << "</mesh>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

string SLCVertex::toXML () const
{
    stringstream ss;
    ss << "<vertex>";
    for ( int i=0; i<data.size(); i++ )
	ss << data[i].x() << ' ' << data[i].y() << ' ' << data[i].z() << ' ';
    ss << "</vertex>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

string SLCNormal::toXML () const
{
    stringstream ss;
    ss << "<normal>";
    for ( int i=0; i<data.size(); i++ )
	ss << data[i].x() << ' ' << data[i].y() << ' ' << data[i].z() << ' ';
    ss << "</normal>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

string SLCTexCoord::toXML () const
{
    stringstream ss;
    ss << "<texcoord>";
    for ( int i=0; i<data.size(); i++ )
	ss << data[i].x() << ' ' << data[i].y() << ' ';
    ss << "</texcoord>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

string SLCVertexIndex::toXML () const
{
    stringstream ss;
    ss << "<vertexindex>";
    for ( int i=0; i<data.size(); i++ )
	ss << data[i] << ' ';
    ss << "</vertexindex>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

string SLCNormalIndex::toXML () const
{
    stringstream ss;
    ss << "<normalindex>";
    for ( int i=0; i<data.size(); i++ )
	ss << data[i] << ' ';
    ss << "</normalindex>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

string SLCTexCoordIndex::toXML () const
{
    stringstream ss;
    ss << "<texcoordindex>";
    for ( int i=0; i<data.size(); i++ )
	ss << data[i] << ' ';
    ss << "</texcoordindex>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

string SLCTransform::toXML () const
{
    stringstream ss;
    ss << "<transform>";
    for ( int i=0; i<16; i++ )
	ss << mat[i] << ' ';
    ss << "</transform>" << endl;
    return ss.str();
}

// --------------------------------------------------------------------------------

