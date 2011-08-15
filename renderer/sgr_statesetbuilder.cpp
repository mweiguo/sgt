#include "sgr_statesetbuilder.h"
#include <fstream>
using namespace std;

SSTStateSetWeight::SSTStateSetWeight ()
{
    parent = 0;
    value = 0;
}

void SSTStateSetWeight::addChild ( SSTStateSetWeight* n )
{
    children.push_back ( n );
    n->parent = this;
}

// --------------------------------------------------------------------------------

SSTBuilder::SSTBuilder ()
{
}

void SSTBuilder::parseAppearance ( LC& lc )
{
    TextureRecord*  texrcd  = NULL;
    MaterialRecord* matrcd  = NULL;
    MeshRecord*     meshrcd = NULL;
    int          meshOffset = -1;
    int          textureOffset = -1;

    if ( lc.toElement ( FIRST_CHILD ) >= 0 ) {
        if ( lc.getType() == SLC_TEXTURE ) {
            texrcd = &(lc.textureEntry->LCRecords[ lc.getValue() ]);
	    textureOffset = lc.getValue();
        } else if ( lc.getType() == SLC_PHONG_MATERIAL )
            matrcd = &(lc.materialEntry->LCRecords[ lc.getValue() ]);
        else if ( lc.getType() == SLC_MESH ) {
            meshrcd = &(lc.meshEntry->LCRecords[ lc.getValue() ]);
	    meshOffset = lc.getValue();
	}
    }
    while ( lc.toElement ( NEXT_SIBLING ) >= 0 ) {
        if ( lc.getType() == SLC_TEXTURE ) {
            texrcd = &(lc.textureEntry->LCRecords[ lc.getValue() ]);
	    textureOffset = lc.getValue();
	} else if ( lc.getType() == SLC_PHONG_MATERIAL )
            matrcd = &(lc.materialEntry->LCRecords[ lc.getValue() ]);
        else if ( lc.getType() == SLC_MESH ) {
            meshrcd = &(lc.meshEntry->LCRecords[ lc.getValue() ]);
	    meshOffset = lc.getValue();
	}
    }
    lc.toElement ( PARENT );

    // StateSet builder
    begin ();
    if ( texrcd ) {
	string filename = texrcd->filename;
	map<string,int>::iterator pp = texManager.find ( filename );
	if ( pp == texManager.end() ) {
	    texManager.insert ( pair<string,int>(filename, textureOffset) );
	    addTexture ( textureOffset );
	} else
	    addTexture ( pp->second );
    } if ( matrcd ) {
        addAmbient ( matrcd->ambient.x(), matrcd->ambient.y(), matrcd->ambient.z(), 1 );
        addDiffuse ( matrcd->diffuse.x(), matrcd->diffuse.y(), matrcd->diffuse.z(), 1 );
        addSpecular ( matrcd->specular.x(), matrcd->specular.y(), matrcd->specular.z(), 1 );
    }
    if ( meshrcd )
        addMesh ( meshOffset );
    end ();
}

void SSTBuilder::traverse ( LC& lc )
{
    if ( lc.toElement ( FIRST_CHILD ) >= 0 ) {
        if ( lc.getType() == SLC_APPEARANCE )
            parseAppearance ( lc );
        else
            traverse ( lc );

        while ( lc.toElement ( NEXT_SIBLING )>=0 ) {
            if ( lc.getType() == SLC_APPEARANCE )
                parseAppearance ( lc );
            else
                traverse ( lc );
        }
        lc.toElement ( PARENT );
    }
}

void SSTBuilder::buildFromLC ( LC& lc )
{
    texManager.clear();

    // traverse LC to build StateSet
    lc.toElement ( ROOT );
    traverse ( lc );

    // check 
//    ssbuilder.save ( ssfilename );
}

void SSTBuilder::begin()
{
}

void SSTBuilder::collectWeight ( SSTStateSet* node, SSTStateSetWeight* wnode )
{
    int i=0;
    for ( list<SSTStateSet*>::iterator pp=node->children.begin(); pp!=node->children.end(); ++pp, ++i ) {
        SSTStateSet* ss = *pp;
        SSTStateSetWeight* weight = wnode->children[i];
        int matchCount = 0;
        for ( int i=0; i<ss->states.size(); i++ ) {
            SSTState& s = ss->states[i];
            for ( list<SSTState>::iterator pp1=preStates.begin(); pp1!=preStates.end(); ++pp1 ) {
                if ( s == *pp1 )
                    matchCount++;
            }
        }
        if ( matchCount != 0 ) {
            weight->value = matchCount;
            leaves.insert ( pair<SSTStateSet*, SSTStateSetWeight*>(ss, weight) );
            if ( ss->children.empty() == false )
                collectWeight ( ss, weight );
            //} else {
            //    leaves.insert ( pair<SSTStateSet*, SSTStateSetWeight*>(ss, weight) );
        }
    }
}

void SSTBuilder::end ()
{
    // find bestMatch
    collectWeight ( &root, &rootWeight );
    int maxweight = 0;
    SSTStateSet* bestMatch = &root;
    SSTStateSetWeight* bestMatchWeight = &rootWeight;
    for ( map<SSTStateSet*, SSTStateSetWeight*>::iterator pp=leaves.begin(); pp!=leaves.end(); ++pp )
    {
        int w = 0;
        SSTStateSet* ss = pp->first;
        SSTStateSetWeight* weight = pp->second;
        while ( weight != 0 ) {
            w += weight->value;
            weight = weight->parent;
        }
        if ( w > maxweight ) {
            maxweight = w;
            bestMatch = ss;
            bestMatchWeight = pp->second;
        }
    }
    
    SSTStateSet* ss = bestMatch;
    if ( maxweight != preStates.size() ) {
        // insert StateSet & StateSetWeight
        ss = new SSTStateSet();
        bestMatch->addChild ( ss );
        SSTStateSetWeight* sw = new SSTStateSetWeight();
        bestMatchWeight->addChild ( sw );

        // remove redundant states preStates
        while ( bestMatch != 0 ) {
            for ( int i=0; i<bestMatch->states.size(); i++ ) {
                SSTState &s = bestMatch->states[i];
                preStates.remove ( s );
            }   
            bestMatch = bestMatch->parent;
        }

        for ( list<SSTState>::iterator pp=preStates.begin(); pp!=preStates.end(); ++pp )
            ss->states.push_back ( *pp );
    }

    for ( int i=0; i<preMeshes.size(); i++ )
        ss->meshes.push_back ( preMeshes[i] );

    preStates.clear();
    preMeshes.clear();
    leaves.clear();
}

void SSTBuilder::addTexture ( int tex )
{
    preStates.push_back ( SSTState (SST_TEXTURE, tex) );
}

void SSTBuilder::addAmbient ( float r, float g, float b, float a )
{
    preStates.push_back ( SSTState (SST_AMBIENT, r, g, b, a) );
}

void SSTBuilder::addDiffuse ( float r, float g, float b, float a )
{
    preStates.push_back ( SSTState (SST_DIFFUSE, r, g, b, a) );
}

void SSTBuilder::addSpecular (  float r, float g, float b, float a )
{
    preStates.push_back ( SSTState (SST_SPECULAR, r, g, b, a) );
}

void SSTBuilder::addMatrix (  float* mat )
{
    preStates.push_back ( SSTState (SST_TRANSFORM, mat) );
}

void SSTBuilder::addMesh ( int meshIndex )
{
    preMeshes.push_back ( meshIndex );
}

void SSTBuilder::save ( const char* filename )
{
    ofstream o;
    o.open ( filename );
    o << root.toXML();
    o.close ();
}

