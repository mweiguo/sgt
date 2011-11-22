#include "sgr_lc.h"
#include "sgr_nodes.h"
#include "bbox2d.h"
#include "mat4.h"
#include "vec4.h"
#include "kdtree.h"
#include "sgr_font.h"
#include "sgr_texture.h"

#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <ctime>
using namespace std;

#include <IL/il.h>
#include <IL/ilut.h>

#include <sqlite3.h>

LoadTex_Proc loadtex_proc = 0;

void initLC ()
{
    ilInit();
    ilutRenderer(ILUT_OPENGL);
}

GlobalLCRecord::GlobalLCRecord ()
{
    type = SLC_NONE;
    depth = -1;
    value = -1;
    memset ( minmax, 0, sizeof(minmax) );
}

// --------------------------------------------------------------------------------

SceneRecord::SceneRecord ( const char* nm )
{
    memset  ( name,  0, 32 * sizeof(char) ); 
    strncpy ( name, nm, 32 ); 
}

// --------------------------------------------------------------------------------

MaterialRecord::MaterialRecord ()
{
    memset  ( name,  0, 32 * sizeof(char) ); 
    background_color.xyzw ( 0.8, 0.8, 0.8, 1.0 );
    foreground_color.xyzw ( 0.2, 0.2, 0.2, 1.0 );
    linewidth = 0;
    linetype = 0xFFFF;//LINETYPE_SOLID;
    linetypefactor = 1;
    memset  ( fontfile,  0, 32 * sizeof(char) ); 
    fontIdx = -1;
    memset  ( texturefile,  0, 32 * sizeof(char) ); 
    textureIdx = -1;
}

MaterialRecord::MaterialRecord ( const char* nm, const vec4i& bg, const vec4i& fg, float lw, 
				 unsigned short lt, unsigned short ltfactor, 
				 const char* fontfilename, const char* texfilename )
{
    memset  ( name,  0, 32 * sizeof(char) ); 
    strncpy ( name, nm, 32 ); 
    background_color = bg;
    foreground_color = fg;
    linewidth = lw;
    linetype  = lt;
    linetypefactor  = ltfactor;
    memset  ( fontfile,  0, 32 * sizeof(char) ); 
    strncpy ( fontfile, fontfilename, 32 ); 
    fontIdx = -1;
    memset  ( texturefile,  0, 32 * sizeof(char) ); 
    strncpy ( texturefile, texfilename, 32 ); 
    textureIdx = -1;
}

// --------------------------------------------------------------------------------

LayerRecord::LayerRecord ( const char* nm, bool isVisible, bool isPickable, int matIdx )
{
    memset  ( name,  0, 32 * sizeof(char) ); 
    strncpy ( name, nm, 32 ); 
    materialIdx = matIdx;
    visible = isVisible;
    pickable = isPickable;
}

// --------------------------------------------------------------------------------

LODPageRecord::LODPageRecord ()
{
    memset  ( kdtreepath,  0, 32 * sizeof(char) ); 
    delayloading = false;
    imposter = false;
    kdtree = -1;
}

LODPageRecord::LODPageRecord ( const char* kdtpath, bool delay, bool imp )
{
    memset  ( kdtreepath,  0, 32 * sizeof(char) ); 
    strncpy ( kdtreepath, kdtpath, 32 ); 
    delayloading = delay;
    imposter = imp;
    kdtree = -1;
}

// --------------------------------------------------------------------------------

DrawableRecord::DrawableRecord ( int matidx ) : materialIdx(matidx)
{
}

// --------------------------------------------------------------------------------

FillableRecord::FillableRecord ( bool isFillTexture, float texang, float texscale, int matidx )
    : DrawableRecord(matidx)
{
    filltexture = isFillTexture;
    textureAngle = texang;
    textureScale = texscale;
}

// --------------------------------------------------------------------------------
RectRecord::RectRecord ()
{
    data[0].xyz ( 0.0f, 0.0f, 0.0f );
    data[1].xyz ( 0.0f, 0.0f, 0.0f );
    data[2].xyz ( 0.0f, 0.0f, 0.0f );
    data[3].xyz ( 0.0f, 0.0f, 0.0f );
}

RectRecord::RectRecord ( const vec3f& p0, const vec3f& p1, const vec3f& p2, const vec3f& p3, bool isFillTexture, float texang, float texscale, int matidx )
    : FillableRecord ( isFillTexture, texang, texscale, matidx)
{
    data[0] = p0;
    data[1] = p1;
    data[2] = p2;
    data[3] = p3; 
}

// --------------------------------------------------------------------------------

// LineRecord::LineRecord ()
// {
//     data[0].xy ( 0.0f, 0.0f );
//     data[1].xy ( 0.0f, 0.0f );
// }

// LineRecord::LineRecord ( const vec2f& p0, const vec2f& p1, int matidx )
//     : DrawableRecord(matidx)
// {
//     data[0] = p0;
//     data[1] = p1;
// }


// --------------------------------------------------------------------------------

TextRecord::TextRecord ()
{
    start = -1;
    scale = 1;
    rotz  = 0;
    silhouetteStart = silhouetteEnd = -1;
}

TextRecord::TextRecord ( int b, const vec3f& offset, float s, float r, int sstart, int send, int matidx )
    : DrawableRecord(matidx)
{
    start = b;
    pos = offset;
    scale = s;
    rotz = r;
    silhouetteStart = sstart;
    silhouetteEnd = send;
}

// --------------------------------------------------------------------------------

PLineRecord::PLineRecord()
{
    start = end = -1;
}

PLineRecord::PLineRecord ( int s, int e, int matidx )
    : DrawableRecord(matidx)
{
    start = s;
    end = e;
}

// --------------------------------------------------------------------------------

PolyRecord::PolyRecord ()
{
    start = end = texcoordstart = texcoordend = tessellationstart = tessellationend = -1; 
}

PolyRecord::PolyRecord ( int s, int e, int ts, int te, int tessStart, int tessEnd, bool isFillTexture, float texang, float texscale, int matidx )
    : FillableRecord( isFillTexture, texang, texscale, matidx )
{
    start = s;
    end = e;
    texcoordstart = ts;
    texcoordend = te;
    tessellationstart = tessStart;
    tessellationend = tessEnd;
}

// --------------------------------------------------------------------------------

SmartTileRecord::SmartTileRecord ()
{
    levelcnt = 0;
    tilecnt = 0;    
    pDB = 0;
}

SmartTileRecord::SmartTileRecord ( const vec3f& p0, const vec3f& p1, const vec3f& p2, const vec3f& p3, int l, const char* name, int matidx )
    : DrawableRecord ( matidx )
{
    data[0] = p0;
    data[1] = p1;
    data[2] = p2;
    data[3] = p3;
    levelcnt = l;
    tilecnt = 0;
    strcpy ( dbname, name );
    pDB = 0;
}

// --------------------------------------------------------------------------------

LC::LC ()
{
    globalLCEntry = 0;
    memset ( levelLCEntries, 0, sizeof(LevelLCEntry*)*256);
    sceneEntry         = 0;
    materialEntry      = 0;
    layerEntry         = 0;
    lineEntry          = 0;
    triangleEntry      = 0;
    textEntry          = 0;
    rectEntry          = 0;
    plineEntry         = 0;
    polyEntry          = 0;
    textBufferEntry    = 0;
    textSilhouetteBufferEntry = 0;
    plineBufferEntry   = 0;
    polyTessellationBufferEntry = 0;
    texCoordBufferEntry = 0;
    smartTilesEntry     = 0;

    cursorDepth = 0;
    memset ( cursor, 0, sizeof(int)*256);
}

LC::~LC ()
{
}

void LC::save ( const char* filename )
{
    ofstream out;
    out.open ( filename, ofstream::binary );

    // write scene data
    if ( NULL==sceneEntry || 0==sceneEntry->LCLen ) {
        SceneEntry tmp; out.write ( (char*)&tmp, sizeof(SceneEntry) );
    } else
        out.write ( (char*)sceneEntry, sceneEntry->LCLen * sizeof(SceneRecord) + sizeof(int) );
    // write meterial data
    if ( NULL==materialEntry || 0==materialEntry->LCLen ) {
        MaterialEntry tmp; out.write ( (char*)&tmp, sizeof(MaterialEntry) );
    } else {
        materialEntry->LCLen /= 2;
        out.write ( (char*)materialEntry, materialEntry->LCLen * sizeof(MaterialRecord) + sizeof(int) );
    }
    // write layer data
    if ( NULL==layerEntry || 0==layerEntry->LCLen ) {
        LayerEntry tmp; out.write ( (char*)&tmp, sizeof(LayerEntry) );
    } else
        out.write ( (char*)layerEntry, layerEntry->LCLen * sizeof(LayerRecord) + sizeof(int) );
    // write lod data
    if ( NULL==lodEntry || 0==lodEntry->LCLen ) {
        LODEntry tmp; out.write ( (char*)&tmp, sizeof(LODEntry) );
    } else
        out.write ( (char*)lodEntry, lodEntry->LCLen * sizeof(LODRecord) + sizeof(int) );
    // write lodpage data
    if ( NULL==lodpageEntry || 0==lodpageEntry->LCLen ) {
        LODPageEntry tmp; out.write ( (char*)&tmp, sizeof(LODPageEntry) );
    } else
        out.write ( (char*)lodpageEntry, lodpageEntry->LCLen * sizeof(LODPageRecord) + sizeof(int) );
    // write line data
    if ( NULL==lineEntry || 0==lineEntry->LCLen ) {
        LineEntry tmp; out.write ( (char*)&tmp, sizeof(LineEntry) );
    } else
        out.write ( (char*)lineEntry, lineEntry->LCLen * sizeof(LineRecord) + sizeof(int) );
    // write triangle data
    if ( NULL==triangleEntry || 0==triangleEntry->LCLen ) {
        TriangleEntry tmp; out.write ( (char*)&tmp, sizeof(TriangleEntry) );
    } else 
        out.write ( (char*)triangleEntry, triangleEntry->LCLen * sizeof(TriangleRecord) + sizeof(int) );
    // write rect data
    if ( NULL==rectEntry || 0==rectEntry->LCLen ) {
        RectEntry tmp; out.write ( (char*)&tmp, sizeof(RectEntry) );
    } else
        out.write ( (char*)rectEntry, rectEntry->LCLen * sizeof(RectRecord) + sizeof(int) );
    // write pline data
    if ( NULL==plineEntry || 0==plineEntry->LCLen ) {
        PLineEntry tmp; out.write ( (char*)&tmp, sizeof(PLineEntry) );
    } else
        out.write ( (char*)plineEntry, plineEntry->LCLen * sizeof(PLineRecord) + sizeof(int) );
    // write poly data
    if ( NULL==polyEntry || 0==polyEntry->LCLen ) {
        PolyEntry tmp; out.write ( (char*)&tmp, sizeof(PolyEntry) );
    } else
        out.write ( (char*)polyEntry, polyEntry->LCLen * sizeof(PolyRecord) + sizeof(int) );
    // write text data
    if ( NULL==textEntry || 0==textEntry->LCLen ) {
        TextEntry tmp; out.write ( (char*)&tmp, sizeof(TextEntry) );
    } else
        out.write ( (char*)textEntry, textEntry->LCLen * sizeof(TextRecord) + sizeof(int) );
    // write text buffer data
    if ( NULL==textBufferEntry || 0==textBufferEntry->LCLen ) {
        TextBufferEntry tmp;
	out.write ( (char*)&tmp, sizeof(TextBufferEntry) );
    } else
        out.write ( (char*)textBufferEntry, textBufferEntry->LCLen * sizeof(char) + sizeof(int) );
    // write text silhouette buffer data
    if ( NULL==textSilhouetteBufferEntry || 0==textSilhouetteBufferEntry->LCLen ) {
        TextSilhouetteBufferEntry tmp;
	out.write ( (char*)&tmp, sizeof(TextSilhouetteBufferEntry) );
    } else
        out.write ( (char*)textSilhouetteBufferEntry, textSilhouetteBufferEntry->LCLen * sizeof(vec3f) + sizeof(int) );
    // write pline buffer data
    if ( NULL==plineBufferEntry || 0==plineBufferEntry->LCLen ) {
        PLineBufferEntry tmp;
	out.write ( (char*)&tmp, sizeof(PLineBufferEntry) );
    } else {
        out.write ( (char*)plineBufferEntry, plineBufferEntry->LCLen * sizeof(vec3f) + sizeof(int) );
    }
    // write poly tessellation buffer data
    if ( NULL==polyTessellationBufferEntry || 0==polyTessellationBufferEntry->LCLen ) {
	PolyTessellationBufferEntry tmp;
	out.write ( (char*)&tmp, sizeof(PolyTessellationBufferEntry) );
    } else {
        out.write ( (char*)polyTessellationBufferEntry, polyTessellationBufferEntry->LCLen * sizeof(vec3f) + sizeof(int) );
    }
    // write texcoord buffer data
    if ( NULL==texCoordBufferEntry || 0==texCoordBufferEntry->LCLen ) {
	TexCoordBufferEntry tmp;
	out.write ( (char*)&tmp, sizeof(TexCoordBufferEntry) );
    } else {
        out.write ( (char*)texCoordBufferEntry, texCoordBufferEntry->LCLen * sizeof(vec2f) + sizeof(int) );
    }
    // write smartiles data
    if ( NULL==smartTilesEntry || 0==smartTilesEntry->LCLen ) {
	SmartTilesEntry tmp;
	out.write ( (char*)&tmp, sizeof(SmartTilesEntry) );
    } else {
        out.write ( (char*)smartTilesEntry, smartTilesEntry->LCLen * sizeof(SmartTileRecord) + sizeof(int) );
    }


    // write globalLCEntry
    out.write ( (char*)globalLCEntry, globalLCEntry->LCLen * sizeof(GlobalLCRecord) + sizeof(int) );
    // write levelLCEntry
    int levelLCEntriesSize = 0;
    for ( int i=0; i<256; i++ ) {
        if ( levelLCEntries[i] != 0 ) {
            out.write ( (char*)levelLCEntries[i], levelLCEntries[i]->LCLen * sizeof(LevelLCRecord) + sizeof(int) );
            levelLCEntriesSize += levelLCEntries[i]->LCLen * sizeof(LevelLCRecord) + sizeof(int);
        }
        else
            break;
    }
    out.close ();
}


// build location cache
bool LC::load ( const char* filename )
{
    string path = filename;
    size_t pos = path.find_last_of ( "/" );
    if ( pos != string::npos )
	path = path.substr ( 0, pos+1 );
    else
    {
	pos = path.find_last_of ( "\\" );
	if ( pos != string::npos )
	    path = path.substr ( 0, pos+1 );
	else
	    path = "";
    }
    ifstream in;
    in.open ( filename, ifstream::binary );
    if ( in.is_open () == false ) {
//        cerr << "open file " << filename << " error" << endl;
        return false;
    }
    in.seekg (0, ios::end);
    int totallength = in.tellg();
    in.seekg (0, ios::beg);

    int length, sz;
    // read scene data
    in.read ( (char*)&length, sizeof(int) );
    sz = length==0 ? sizeof(SceneRecord) : sizeof(SceneRecord) * length;
    sceneEntry = (SceneEntry*)malloc ( sizeof(int) + sz );
    sceneEntry->LCLen = length;
    in.read ( (char*)sceneEntry->LCRecords, sz );
    // read material data
    in.read ( (char*)&length, sizeof(int) );
    sz = length==0 ? sizeof(MaterialRecord) : sizeof(MaterialRecord) * length;
    if ( length == 0 ) materialEntry = (MaterialEntry*)malloc ( sizeof(int) + sz );
    else               materialEntry = (MaterialEntry*)malloc ( sizeof(int) + sz * 2 );
    materialEntry->LCLen = length * 2;
    in.read ( (char*)materialEntry->LCRecords, sz );
    if ( length != 0 )
        memcpy ( materialEntry->LCRecords+length, materialEntry->LCRecords, sz );
      // process fonts
    for ( int ii=0; ii<length; ii++ )
    {
	Font* ft = 0;
	MaterialRecord& mr = materialEntry->LCRecords[ii];
	if ( string("") == mr.fontfile )
	    strcpy ( mr.fontfile, "simhei.ttf" );
	for ( size_t jj=0; jj<fonts.size(); jj++ )
	{
	    if ( fonts[jj]->fontfilename == mr.fontfile ) {
		ft = fonts[jj];
		mr.fontIdx = jj;
		break;
	    }
	}

	if ( NULL == ft && string("")!=mr.fontfile ) {
	    ft = new Font ( mr.fontfile );
	    mr.fontIdx = fonts.size();
	    fonts.push_back ( ft );
	}
    }
      // process textures
    for ( int ii=0; ii<length; ii++ )
    {
	Texture* tex = 0;
	MaterialRecord& mr = materialEntry->LCRecords[ii];
	for ( size_t jj=0; jj<textures.size(); jj++ )
	{
	    if ( textures[jj]->texturefilename == mr.texturefile ) {
		tex = textures[jj];
		mr.textureIdx = jj;
		break;
	    }
	}
	if ( NULL == tex && string("")!=mr.texturefile ) {
 	    tex = new Texture ( mr.texturefile );
	    if ( tex->texture == 0 )
		cerr << "load texture '" << mr.texturefile << "' failed" << endl;
	    mr.textureIdx = textures.size();
	    textures.push_back ( tex );
	}
    }
    
    // read layer data
    in.read ( (char*)&length, sizeof(int) );
    sz = length==0 ? sizeof(LayerRecord) : sizeof(LayerRecord) * length;
    layerEntry = (LayerEntry*)malloc ( sizeof(int) + sz );
    layerEntry->LCLen = length;
    in.read ( (char*)layerEntry->LCRecords, sz );
    // read lod data
    in.read ( (char*)&length, sizeof(int) );
    sz = length==0 ? sizeof(LODRecord) : sizeof(LODRecord) * length;
    lodEntry = (LODEntry*)malloc ( sizeof(int) + sz );
    lodEntry->LCLen = length;
    in.read ( (char*)lodEntry->LCRecords, sz );
    // read lodpage data
    in.read ( (char*)&length, sizeof(int) );
    sz = length==0 ? sizeof(LODPageRecord) : sizeof(LODPageRecord) * length;
    lodpageEntry = (LODPageEntry*)malloc ( sizeof(int) + sz );
    lodpageEntry->LCLen = length;
    in.read ( (char*)lodpageEntry->LCRecords, sz );
    // read line data
    in.read ( (char*)&length, sizeof(int) );
    sz = length==0 ? sizeof(LineRecord) : sizeof(LineRecord) * length;
    lineEntry = (LineEntry*)malloc ( sizeof(int) + sz );
    lineEntry->LCLen = length;
    in.read ( (char*)lineEntry->LCRecords, sz );
    // read triangle data
    in.read ( (char*)&length, sizeof(int) );
    sz = length==0 ? sizeof(TriangleRecord) : sizeof(TriangleRecord) * length;
    triangleEntry = (TriangleEntry*)malloc ( sizeof(int) + sz );
    triangleEntry->LCLen = length;
    in.read ( (char*)triangleEntry->LCRecords, sz );
    // read rect data
    in.read ( (char*)&length, sizeof(int) );
    sz = length==0 ? sizeof(RectRecord) : sizeof(RectRecord) * length;
    rectEntry = (RectEntry*)malloc ( sizeof(int) + sz );
    rectEntry->LCLen = length;
    in.read ( (char*)rectEntry->LCRecords, sz );
    // read pline data
    in.read ( (char*)&length, sizeof(int) );
    sz = length==0 ? sizeof(PLineRecord) : sizeof(PLineRecord) * length;
    plineEntry = (PLineEntry*)malloc ( sizeof(int) + sz );
    plineEntry->LCLen = length;
    in.read ( (char*)plineEntry->LCRecords, sz );
    // read poly data
    in.read ( (char*)&length, sizeof(int) );
    sz = length==0 ? sizeof(PolyRecord) : sizeof(PolyRecord) * length;
    polyEntry = (PolyEntry*)malloc ( sizeof(int) + sz );
    polyEntry->LCLen = length;
    in.read ( (char*)polyEntry->LCRecords, sz );
    // read text data
    in.read ( (char*)&length, sizeof(int) );
    sz = length==0 ? sizeof(TextRecord) : sizeof(TextRecord) * length;
    textEntry = (TextEntry*)malloc ( sizeof(int) + sz );
    textEntry->LCLen = length;
    in.read ( (char*)textEntry->LCRecords, sz );
//     for ( int i=0; i<length; i++ )
// 	textSilhouettes.push_back ( TextSilhouetteRecord() );
    // read text buffer data
    in.read ( (char*)&length, sizeof(int) );
    sz = length==0 ? sizeof(TextBufferEntry) : (sizeof(int) + sizeof(char) * length);
    textBufferEntry = (TextBufferEntry*)malloc ( sz );
    textBufferEntry->LCLen = length;
    in.read ( (char*)textBufferEntry->LCRecords, sz-sizeof(int) );
    // read text silhouette buffer data
    in.read ( (char*)&length, sizeof(int) );
    sz = length==0 ? sizeof(TextSilhouetteBufferEntry) : (sizeof(int) + sizeof(vec3f) * length);
    textSilhouetteBufferEntry = (TextSilhouetteBufferEntry*)malloc ( sz );
    textSilhouetteBufferEntry->LCLen = length;
    in.read ( (char*)textSilhouetteBufferEntry->LCRecords, sz-sizeof(int) );
    // read pline buffer data
    in.read ( (char*)&length, sizeof(int) );
    sz = length==0 ? sizeof(PLineBufferEntry) : (sizeof(int) + sizeof(vec3f) * length);
    plineBufferEntry = (PLineBufferEntry*)malloc ( sz );
    plineBufferEntry->LCLen = length;
    in.read ( (char*)plineBufferEntry->LCRecords, sz-sizeof(int) );
    // read poly tessellation buffer data
    in.read ( (char*)&length, sizeof(int) );
    sz = length==0 ? sizeof(PolyTessellationBufferEntry) : (sizeof(int) + sizeof(vec3f) * length);
    polyTessellationBufferEntry = (PolyTessellationBufferEntry*)malloc ( sz );
    polyTessellationBufferEntry->LCLen = length;
    in.read ( (char*)polyTessellationBufferEntry->LCRecords, sz-sizeof(int) );
    // read texcoord buffer data
    in.read ( (char*)&length, sizeof(int) );
    sz = length==0 ? sizeof(TexCoordBufferEntry) : (sizeof(int) + sizeof(vec2f) * length);
    texCoordBufferEntry = (TexCoordBufferEntry*)malloc ( sz );
    texCoordBufferEntry->LCLen = length;
    in.read ( (char*)texCoordBufferEntry->LCRecords, sz-sizeof(int) );
    // read smartiles data
    in.read ( (char*)&length, sizeof(int) );
    sz = length==0 ? sizeof(SmartTilesEntry) : (sizeof(int) + sizeof(SmartTileRecord) * length);
    smartTilesEntry = (SmartTilesEntry*)malloc ( sz );
    smartTilesEntry->LCLen = length;
    in.read ( (char*)smartTilesEntry->LCRecords, sz-sizeof(int) );
    // load db
    for ( int i=0; i<length; i++ )
    {
        if ( strcmp("", smartTilesEntry->LCRecords[i].dbname)!=0 ) {
            if ( SQLITE_OK != sqlite3_open ( smartTilesEntry->LCRecords[i].dbname, (sqlite3**)&smartTilesEntry->LCRecords[i].pDB)) {
                smartTilesEntry->LCRecords[i].pDB = NULL;
                cerr << "load database " << smartTilesEntry->LCRecords[i].dbname << " failed" << endl;
            }
        }
    }

    // read GlobalLCEntry
    in.read ( (char*)&length, sizeof(int) );
//    cout << "sz = " << sz << ',' << length << endl;
    sz = length==0 ? sizeof(GlobalLCRecord) : sizeof(GlobalLCRecord) * length;
    globalLCEntry = (GlobalLCEntry*)malloc ( sizeof(int) + sz );
    globalLCEntry->LCLen = length;
    in.read ( (char*)globalLCEntry->LCRecords, sz );
    // read levelLCEntries
    for ( int i=0; i<256; i++ )
    {
        in.read ( (char*)&length, sizeof(int) );
        sz = length==0 ? sizeof(LevelLCRecord) : sizeof(LevelLCRecord) * length;
        levelLCEntries[i] = (LevelLCEntry*)malloc ( sizeof(int) + sz );
        levelLCEntries[i]->LCLen = length;
        in.read ( (char*)levelLCEntries[i]->LCRecords, sz );
	if ( in.tellg() >= totallength )
	    break;
    }
    in.close();

    // load kdtrees
    for ( int i=0; i<lodpageEntry->LCLen; i++ )
    {
	LODPageRecord& lodpage = lodpageEntry->LCRecords[i];
	if ( 0 != strcmp ( "", lodpage.kdtreepath ) ) {
	    string tmp;
	    tmp.assign ( lodpage.kdtreepath, lodpage.kdtreepath + strlen(lodpage.kdtreepath) );
	    string kdfilepath = path + tmp;
	    kdtrees.push_back ( new KdTree<int>() );
	    clock_t t = clock ();
	    kdtrees.back ()->load ( kdfilepath.c_str() );
	    cout << "load kdtree " << kdfilepath << " ok, ellapse " << clock() - t << endl;
	    lodpage.kdtree = kdtrees.size()-1;
	}
    }

    return true;
}

void LC::free ()
{
    for ( int i=0; i<smartTilesEntry->LCLen; i++ ) {
        if ( smartTilesEntry->LCRecords[i].pDB != 0 ) {
            sqlite3_close ( (sqlite3*)smartTilesEntry->LCRecords[i].pDB );
        }
    }

    ::free ( sceneEntry );
    ::free ( materialEntry );
    ::free ( layerEntry );
    ::free ( lodEntry );
    ::free ( lodpageEntry );
    ::free ( lineEntry );
    ::free ( triangleEntry );
    ::free ( rectEntry );
    ::free ( plineEntry );
    ::free ( polyEntry );
    ::free ( textEntry );
    ::free ( textBufferEntry );
    ::free ( textSilhouetteBufferEntry );
    ::free ( plineBufferEntry );
    ::free ( polyTessellationBufferEntry );
    ::free ( texCoordBufferEntry );
    ::free ( smartTilesEntry );
    ::free ( globalLCEntry );
    for ( int i=0; i<256; i++ )
    {
        if ( levelLCEntries[i] != 0 ) {
            ::free ( levelLCEntries[i] );
            levelLCEntries[i] = 0;
            break;
        }
    }
    
    sceneEntry = 0;
    materialEntry = 0;
    layerEntry = 0;
    lodEntry = 0;
    lodpageEntry = 0;
    lineEntry = 0;
    triangleEntry = 0;
    rectEntry = 0;
    plineEntry = 0;
    polyEntry = 0;
    textEntry = 0;
    textBufferEntry = 0;
    textSilhouetteBufferEntry = 0;
    plineBufferEntry = 0;
    polyTessellationBufferEntry = 0;
    texCoordBufferEntry = 0;
    smartTilesEntry = 0;
    globalLCEntry = 0;

    for ( vector<Font*>::iterator pp=fonts.begin(); pp!=fonts.end(); ++pp )
        delete *pp;
    for ( vector<Texture*>::iterator pp=textures.begin(); pp!=textures.end(); ++pp )
        delete *pp;

    for ( map<string,unsigned int>::iterator pp=smartile_textures.begin();
          pp!=smartile_textures.end(); ++pp )
        glDeleteTextures ( 1, &(pp->second) );
    smartile_textures.clear();
}

void LC::buildLevelLC ()
{
    vector<int> gIndexEntries[256];
    vector<int> firstChildEntries[256];
    for ( int i=0; i<globalLCEntry->LCLen; i++ ) {
        GlobalLCRecord& record = globalLCEntry->LCRecords[i];
        int depth = record.depth;
        vector<int>& gIndexEntry = gIndexEntries[depth];
        if ( globalLCEntry->LCRecords[i+1].depth == depth + 1 ) {
            int tmp = gIndexEntries[depth+1].size();
            vector<int>& tempv = firstChildEntries[depth];
            tempv.push_back ( tmp );
        } else
            firstChildEntries[depth].push_back ( -1 );
        gIndexEntry.push_back ( i );
    }

    for ( int i=0; i<256; i++ )
    {
        vector<int>& gIndexEntry = gIndexEntries[i];
        vector<int>& firstChildEntry = firstChildEntries[i];
        if ( gIndexEntry.empty () == false ) {
            int sz = gIndexEntry.size() * sizeof(LevelLCRecord) + sizeof(int);
            LevelLCEntry* tempEntry = (LevelLCEntry*)malloc ( sz );
            memset ( tempEntry, 0, sz );
            levelLCEntries[i] = tempEntry;
            tempEntry->LCLen = gIndexEntry.size();

            for ( size_t j=0; j<gIndexEntry.size(); j++ ) {
                tempEntry->LCRecords[j].gOffset       = gIndexEntry[j];
                tempEntry->LCRecords[j].firstChildIdx = firstChildEntry[j];
            }
        } else 
            levelLCEntries[i] = 0;
    }
}

void LC::freeLevelLC ()
{
    for ( int i=0; i<256; i++ )
    {
        if ( levelLCEntries[i] != 0 ) {
            ::free ( levelLCEntries[i] );
            levelLCEntries[i] = 0;
        }
    }
    cursorDepth = 0;
    memset ( cursor, 0, sizeof(int)*256);
}

/**
 * return -1 if fails
 */
int LC::toElement ( int direction )
{
    // get LevelLCRecord from LCEntries
    // get offset of levelLCRecord
    switch ( direction )
    {
    case ROOT:
        cursorDepth = 0;
        cursor[0] = 0;
        break;
    case PARENT:
        if ( cursorDepth == 0 ) 
            return -1;
        else if ( cursorDepth > 0 )
            cursorDepth --;
        break;
    case FIRST_CHILD:
    {
        int offset = cursor[cursorDepth];
        LevelLCRecord& record = levelLCEntries[cursorDepth]->LCRecords[ offset ];

        if ( -1 == record.firstChildIdx )
            return -1;
        cursor[++cursorDepth] = record.firstChildIdx;
        break;
    }
    case NEXT_SIBLING: 
    {
        int offset = cursor[cursorDepth];
        if ( offset >= levelLCEntries[cursorDepth]->LCLen-1 )
            return -1;

        // find lastChildOffset
        int lastChildOffset = levelLCEntries[cursorDepth]->LCLen-1;
        int parentoffset = cursor[cursorDepth-1] + 1;
        while (1)
        {
            if ( parentoffset < levelLCEntries[cursorDepth-1]->LCLen ) {
                if ( levelLCEntries[cursorDepth-1]->LCRecords[ parentoffset ].firstChildIdx != -1 ) {
                    lastChildOffset = levelLCEntries[cursorDepth-1]->LCRecords[ parentoffset ].firstChildIdx - 1;
                    break;
                }
            } else
                break;
            parentoffset ++;
        }

        if ( offset >= lastChildOffset )
            return -1;
        cursor[cursorDepth] = cursor[cursorDepth] + 1;
        break;
    }
    }
    return cursor[cursorDepth];
}

int LC::getType ()
{
    int offset = cursor[cursorDepth];
    LevelLCRecord& record = levelLCEntries[cursorDepth]->LCRecords[offset];
    GlobalLCRecord grecord = globalLCEntry->LCRecords[record.gOffset];
    return grecord.type;
}

string LC::getTypeStr ()
{
    int t = getType();
    switch ( t )
    {
    case SLC_SCENE:
        return "SceneNode";
    case SLC_MATERIAL:
        return "MaterialNode";
    case SLC_LOD:
        return "LODNode";
    case SLC_LODPAGE:
        return "LODPageNode";
    case SLC_LAYER:
        return "LayerNode";
    case SLC_PLINE:
        return "PLineNode";
    case SLC_POLY:
        return "PolyNode";
    case SLC_LINE:
        return "LineNode";
    case SLC_TRIANGLE:
        return "TriangleNode";
    case SLC_RECT:
        return "RectNode";
    case SLC_TEXT:
        return "TextNode";
    case SLC_SMARTILES:
        return "SmartTiles";
    default:
        return "Unknown Type";
    }
}

int LC::getDepth ()
{
    return cursorDepth;
}

int LC::getValue ()
{
    int offset = cursor[cursorDepth];
    LevelLCRecord& record = levelLCEntries[cursorDepth]->LCRecords[offset];
    GlobalLCRecord grecord = globalLCEntry->LCRecords[record.gOffset];
    return grecord.value;
}

int LC::getGIndex ()
{
    int offset = cursor[cursorDepth];
    LevelLCRecord& record = levelLCEntries[cursorDepth]->LCRecords[offset];
    return record.gOffset;
}

float* LC::getMinMax ()
{
    int offset = cursor[cursorDepth];
    LevelLCRecord& record = levelLCEntries[cursorDepth]->LCRecords[offset];
    GlobalLCRecord& grecord = globalLCEntry->LCRecords[record.gOffset];
    return grecord.minmax;
}

void LC::setMinMax ( float minx, float miny, float maxx, float maxy )
{
    int offset = cursor[cursorDepth];
    LevelLCRecord& record = levelLCEntries[cursorDepth]->LCRecords[offset];
    GlobalLCRecord& grecord = globalLCEntry->LCRecords[record.gOffset];
    grecord.minmax[0] = minx;
    grecord.minmax[1] = miny;
    grecord.minmax[2] = maxx;
    grecord.minmax[3] = maxy;
}

/**
 * 0x00 - 0x7F: 1 byte
 * 0xC0 - 0xDF: 2 bytes
 * 0xE0 - 0xEF: 3 bytes
 * 0xF0 - 0xFF: 4 bytes
 */
unsigned int utf8_strlen ( const char* str )
{
    const char* p = str;
    unsigned int cnt = 0;
    while ( *p )
    {
	switch ( *p & 0xF0 )
	{
	case 0xD0:
	    p += 2;
	    break;
	case 0xE0:
	    p += 3;
	    break;
	case 0xF0:
	    p += 4;
	    break;
	default:
	    p++;
	}
	cnt ++;
    };
    return cnt;
}

void LC::updateMinMax ()
{
    int offset = cursor[cursorDepth];
    LevelLCRecord& record = levelLCEntries[cursorDepth]->LCRecords[offset];
    GlobalLCRecord& grecord = globalLCEntry->LCRecords[record.gOffset];
    BBox2d bbox;
    switch ( grecord.type )
    {
    case SLC_SMARTILES:
    {
        SmartTileRecord& quad = smartTilesEntry->LCRecords[grecord.value];
        vec3f& p0 = quad.data[0];
        vec3f& p1 = quad.data[1];
        vec3f& p2 = quad.data[2];
        vec3f& p3 = quad.data[3];
        bbox.init ( p0.xy() );
        bbox.expandby ( p1.xy() );
        bbox.expandby ( p2.xy() );
        bbox.expandby ( p3.xy() );
        break;
    }
    case SLC_PLINE:
    {
	PLineRecord& pline = plineEntry->LCRecords[grecord.value];
	if ( pline.start < pline.end )
	{
	    bbox.init ( plineBufferEntry->LCRecords[pline.start].xy() );
	    for ( int i=pline.start; i<pline.end; i++ )
	    {
		bbox.expandby ( plineBufferEntry->LCRecords[i].xy() );
	    }
	}
	break;
    }
    case SLC_POLY:
    {
	// update minmax
	PolyRecord& poly = polyEntry->LCRecords[grecord.value];
	if ( poly.start < poly.end )
	{
	    bbox.init ( plineBufferEntry->LCRecords[poly.start].xy() );
	    for ( int i=poly.start; i<poly.end; i++ )
	    {
		bbox.expandby ( plineBufferEntry->LCRecords[i].xy() );
	    }
	}
	break;
    }
    case SLC_LINE:
    {
        LineRecord& line = lineEntry->LCRecords[grecord.value];
        vec2f& p0 = line.data[0];
        vec2f& p1 = line.data[1];
        bbox.init ( p0 );
        bbox.expandby ( p1 );
        break;
    }
    case SLC_TRIANGLE:
    {
        TriangleRecord& tri = triangleEntry->LCRecords[grecord.value];
        vec2f& p0 = tri.data[0];
        vec2f& p1 = tri.data[1];
        vec2f& p2 = tri.data[2];
        bbox.init ( p0 );
        bbox.expandby ( p1 );
        bbox.expandby ( p2 );
        break;
    }
    case SLC_RECT:
    {
        RectRecord& quad = rectEntry->LCRecords[grecord.value];
        vec3f& p0 = quad.data[0];
        vec3f& p1 = quad.data[1];
        vec3f& p2 = quad.data[2];
        vec3f& p3 = quad.data[3];
        bbox.init ( p0.xy() );
        bbox.expandby ( p1.xy() );
        bbox.expandby ( p2.xy() );
        bbox.expandby ( p3.xy() );
        break;
    }
    case SLC_TEXT:
    {
        TextRecord& text = textEntry->LCRecords[grecord.value];
	MaterialRecord& mat = materialEntry->LCRecords[text.materialIdx];
	Font* font = fonts[mat.fontIdx];
	vec3f& v0 = textSilhouetteBufferEntry->LCRecords[text.silhouetteStart];
	vec3f& v1 = textSilhouetteBufferEntry->LCRecords[text.silhouetteStart+1];
	vec3f& v2 = textSilhouetteBufferEntry->LCRecords[text.silhouetteStart+2];
	vec3f& v3 = textSilhouetteBufferEntry->LCRecords[text.silhouetteStart+3];
	
	string content = textBufferEntry->LCRecords + text.start;
	
	bbox.init ( text.pos.xy() );
	float w=0, h=0;
	font->getSize (content.c_str(), w, h );
	bbox.expandby ( text.pos.xy() + vec2f(w, h ) );
	mat4f m;
	m *= mat4f::translate_matrix ( text.pos.x(), text.pos.y(), 0 );
	m *= mat4f::rotate_matrix ( text.rotz, 2 );
	m *= mat4f::scale_matrix ( text.scale, text.scale, text.scale );

	// set bbox & silhouette
	vec4f tmp = m * vec4f(0, 0, text.pos.z(), 1);
	v0 = tmp.xy();
	bbox.init ( v0 );

	tmp = m * vec4f ( w, 0, text.pos.z(), 1 );
	v1 = tmp.xy();
	bbox.expandby ( v1 );

	tmp = m * vec4f ( w, h, text.pos.z(), 1);
	v2 = tmp.xy();
	bbox.expandby ( v2 );

	tmp = m * vec4f ( 0, h, text.pos.z(), 1 );
	v3 = tmp.xy();
	cout << v3.x() << "," << v3.y() << endl;
	bbox.expandby ( v3 );
        break;
    }
    }

    grecord.minmax[0] = bbox.minvec().x();
    grecord.minmax[1] = bbox.minvec().y();
    grecord.minmax[2] = bbox.maxvec().x();
    grecord.minmax[3] = bbox.maxvec().y();
}

void LC::insertBeforeElement ()
{
    // get global index
    int offset = cursor[cursorDepth];
    LevelLCRecord& record = levelLCEntries[cursorDepth]->LCRecords[ offset ];
    // add record to command queue
    ModifyCommand cmd;
    cmd.commandMask = CMD_INSERT_BEFORE_ELEMENT;
    cmd.globalIndex = record.gOffset;
    modifyCommands.push_back ( cmd );
}

void LC::insertAfterElement ()
{
    // get global index
    int offset = cursor[cursorDepth];
    LevelLCRecord& record = levelLCEntries[cursorDepth]->LCRecords[ offset ];
    // add record to command queue
    ModifyCommand cmd;
    cmd.commandMask = CMD_INSERT_AFTER_ELEMENT;
    cmd.globalIndex = record.gOffset;
    modifyCommands.push_back ( cmd );
}

void LC::deleteElement ()
{
    // get global index
    int offset = cursor[cursorDepth];
    LevelLCRecord& record = levelLCEntries[cursorDepth]->LCRecords[ offset ];
    // add record to command queue
    ModifyCommand cmd;
    cmd.commandMask = CMD_DELETE_ELEMENT;
    cmd.globalIndex = record.gOffset;
    modifyCommands.push_back ( cmd );
}

void LC::updateElement ()
{
//     // get global index
//     int offset = cursor[cursorDepth];
//     LevelLCRecord& record = LevelLCEntry[cursorDepth].LCRecords[ offset ];
//     // add record to command queue
//     ModifyCommand cmd;
//     cmd.commandMask = CMD_DELETE_UPDATE;
//     cmd.globalIndex = record.gOffset;
//     modifyCommands.push_back ( cmd );
}

void LC::endEdit()
{
    // copy to a temp 
    vector< GlobalLCRecord > temp;
    temp.assign ( globalLCEntry->LCRecords, globalLCEntry->LCRecords + globalLCEntry->LCLen );
    
    // modify global LC entry
    for ( size_t i=0; i<modifyCommands.size(); i++ )
    {
        ModifyCommand& cmd = modifyCommands[i];
        switch ( cmd.commandMask )
        {
        case CMD_INSERT_BEFORE_ELEMENT:
        {
            // get GlobalLCRecord
            GlobalLCRecord& record = globalLCEntry->LCRecords[cmd.globalIndex];
            // copy it and insert it before current position
            GlobalLCRecord newrecord ( record );
            temp.insert ( temp.begin()+cmd.globalIndex, newrecord );
            break;
        }
        case CMD_INSERT_AFTER_ELEMENT:
        {
            // get GlobalLCRecord
            GlobalLCRecord& record = globalLCEntry->LCRecords[cmd.globalIndex];
            // copy it and insert it before current position
            GlobalLCRecord newrecord ( record );
            temp.insert ( temp.begin()+cmd.globalIndex+1, newrecord );
            break;
        }
        case CMD_DELETE_ELEMENT:
            // get GlobalLCRecord
            temp.erase ( temp.begin()+cmd.globalIndex );
            break;
        }
    }
    // copy back
    ::free ( globalLCEntry );
    int sz = temp.size() * sizeof(GlobalLCRecord) + sizeof(int);
    globalLCEntry = (GlobalLCEntry*)malloc ( sz );
    memset ( globalLCEntry, 0, sz );
    globalLCEntry->LCLen = temp.size();
    memcpy ( globalLCEntry->LCRecords, &(temp[0]), globalLCEntry->LCLen * sizeof(GlobalLCRecord) );
    
    // clear modify command queue
    modifyCommands.clear();

    buildLevelLC ();
}

