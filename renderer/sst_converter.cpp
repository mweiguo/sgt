#include <slc_lc.h>
#include "sst_converter.h"
#include <iostream>
using namespace std;
void SSTConverter::convert ( const char* lcfilename, const char* ssfilename )
{
    texManager.clear();

    if ( false == lc.loadLC ( lcfilename ) )
    {
        cout << "load lc file error" << endl;
        return;
    }
    // traverse LC to build StateSet
    lc.toElement ( ROOT );
    traverse ();

    // check 
    ssbuilder.save ( ssfilename );
}

void SSTConverter::parseAppearance ()
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
    ssbuilder.begin ();
    if ( texrcd ) {
	string filename = texrcd->filename;
	map<string,int>::iterator pp = texManager.find ( filename );
	if ( pp == texManager.end() ) {
	    texManager.insert ( pair<string,int>(filename, textureOffset) );
	    ssbuilder.addTexture ( textureOffset );
	} else
	    ssbuilder.addTexture ( pp->second );
    } if ( matrcd ) {
        ssbuilder.addAmbient ( matrcd->ambient.x(), matrcd->ambient.y(), matrcd->ambient.z(), 1 );
        ssbuilder.addDiffuse ( matrcd->diffuse.x(), matrcd->diffuse.y(), matrcd->diffuse.z(), 1 );
        ssbuilder.addSpecular ( matrcd->specular.x(), matrcd->specular.y(), matrcd->specular.z(), 1 );
    }
    if ( meshrcd )
        ssbuilder.addMesh ( meshOffset );
    ssbuilder.end ();
}

// void SSTConverter::parseTransform ()
// {
//     TransformRecord* transrcd = &(lc.transformEntry->LCRecords[ lc.getValue() ]);
//     ssbuilder.begin();
//     ssbuilder.addMatrix ( transrcd->mat );
//     ssbuilder.end();
// }

void SSTConverter::traverse ()
{
    if ( lc.toElement ( FIRST_CHILD ) >= 0 ) {
        if ( lc.getType() == SLC_APPEARANCE )
            parseAppearance ();
//         else if ( lc.getType() == SLC_TRANSFORM )
//             parseTransform ();
        else
            traverse ();

        while ( lc.toElement ( NEXT_SIBLING )>=0 ) {
            if ( lc.getType() == SLC_APPEARANCE )
                parseAppearance ();
// 	    else if ( lc.getType() == SLC_TRANSFORM )
// 		parseTransform ();
            else
                traverse ();
        }
        lc.toElement ( PARENT );
    }
}

int main ( int argc, char* argv[] )
{
    if ( argc != 3 ) {
        cout << "usage : " << argv[0] << " slcFileName outputFileName" << endl;
        return 0;
    }
        
    SSTConverter converter;
    converter.convert ( argv[1], argv[2] );
    return 0;
}
