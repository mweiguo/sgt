#include "sgr_stateset.h"
#include <sstream>
#include <GL/glew.h>
using namespace std;

SSTStateSet::SSTStateSet ()
{
    parent = 0;
}

void SSTStateSet::addChild ( SSTStateSet* ss )
{
    children.push_back ( ss );
    ss->parent = this;
}

void SSTStateSet::render ()
{
    if ( meshes.empty() == false )
    {
	applyStates();
	draw ();
	for ( list<SSTStateSet*>::iterator pp=children.begin(); pp!=children.end(); ++pp )
	{
	    (*pp)->render ();
	}
    }
}

void SSTStateSet::applyStates ()
{
    for ( vector<SSTState>::iterator pp=states.begin(); pp!=states.end(); ++pp )
    {
	switch ( pp->type )
	{
	case SST_TEXTURE:
	    glBindTexture ( GL_TEXTURE_2D, pp->valuei );
	    break;
	case SST_AMBIENT:
	    glMaterialfv ( GL_FRONT, GL_AMBIENT, (float*)&(pp->valuefv) );
	    break;
	case SST_DIFFUSE:
	    glMaterialfv ( GL_FRONT, GL_DIFFUSE, (float*)&(pp->valuefv) );
	    break;
	case SST_SPECULAR:
	    glMaterialfv ( GL_FRONT, GL_SPECULAR, (float*)&(pp->valuefv) );
	    break;
	}
    }
}

void SSTStateSet::draw ()
{
    int drawMeshDisplayList;
    for ( vector<int>::iterator pp=meshes.begin(); pp!=meshes.end(); ++pp )
    {	
	int mesh = (*pp);
// 	// get model's transform matrix
// 	float m[16] = getTransformMatrix ( mesh );
// 	// apply matrix
// 	glPushMatrix();
// 	glMultMatrixf ( m );
// 	glCallList ( drawMeshDisplayList );
// 	glPopMatrix();
    }
}

string SSTStateSet::toXML() const
{
    stringstream ss;
    if ( parent == NULL )
    {
        ss << "<stateset_root>" << endl;
        for ( list<SSTStateSet*>::const_iterator pp=children.begin(); pp!=children.end(); ++pp )
            ss << (*pp)->toXML();
        ss << "</stateset_root>" << endl;
    }
    else
    {
        ss << "<ss";
        for ( int i=0; i<states.size(); i++ ) {
            const SSTState& state = states[i];
            switch ( state.type )
            {
            case SST_TEXTURE:
                ss << " texture=\"" << state.valuei << "\"";
                break;
            case SST_AMBIENT:
                ss << " ambient=\"" << state.valuefv.x() << ' ' << state.valuefv.y() << ' ' << state.valuefv.z() << "\"";
                break;
            case SST_DIFFUSE:
                ss << " diffuse=\"" << state.valuefv.x() << ' ' << state.valuefv.y() << ' ' << state.valuefv.z() << "\"";
                break;
            case SST_SPECULAR:
                ss << " specular=\"" << state.valuefv.x() << ' ' << state.valuefv.y() << ' ' << state.valuefv.z() << "\"";
                break;
//             case SST_MESH:
//                 ss << " mesh=\"" << state.valuei << "\"";
//                 break;
            }
        }
        ss << ">" << endl;
	for ( int i=0; i<meshes.size(); i++ )
	    ss << meshes[i] << ' ';
	if ( meshes.empty() == false )
	    ss << endl;

        for ( list<SSTStateSet*>::const_iterator pp=children.begin();
              pp!=children.end(); ++pp ) {
            ss << (*pp)->toXML();
        }

        ss << "</ss>" << endl;
    }
    return ss.str();
}

