#include "sgr_lc.h"
#include "sgr_nodes.h"
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "bbox2d.h"
#include "kdtree.h"
using namespace std;

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
    backgroud_color.xyz ( 0.8, 0.8, 0.8 );
    foregroud_color.xyz ( 0.2, 0.2, 0.2 );
    linewidth = 0;
    linetype = LINETYPE_SOLID;
}

MaterialRecord::MaterialRecord ( const char* nm, const vec3i& bg, const vec3i& fg, float lw, int linetype )
{
    memset  ( name,  0, 32 * sizeof(char) ); 
    strncpy ( name, nm, 32 ); 
    backgroud_color = bg;
    foregroud_color = fg;
    linewidth = lw;
    linetype  = linetype;
}

// --------------------------------------------------------------------------------

LayerRecord::LayerRecord ( const char* nm, int f, int matIdx )
{
    memset  ( name,  0, 32 * sizeof(char) ); 
    strncpy ( name, nm, 32 ); 
    flags = f; 
    materialIdx = matIdx;
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

LC::LC ()
{
    globalLCEntry = 0;
    memset ( levelLCEntries, 0, sizeof(LevelLCEntry*)*256);
    sceneEntry         = 0;
    materialEntry      = 0;
    layerEntry         = 0;
    lineEntry          = 0;
    triangleEntry      = 0;
    rectEntry          = 0;
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
    } else
        out.write ( (char*)materialEntry, materialEntry->LCLen * sizeof(MaterialRecord) + sizeof(int) );
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
    ifstream in;
    in.open ( filename, ifstream::binary );
    if ( in.is_open () == false ) {
        cout << "open file " << filename << " error" << endl;
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
    materialEntry = (MaterialEntry*)malloc ( sizeof(int) + sz );
    materialEntry->LCLen = length;
    in.read ( (char*)materialEntry->LCRecords, sz );
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

    // read GlobalLCEntry
    in.read ( (char*)&length, sizeof(int) );
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
	    
	    kdtrees.push_back ( new KdTree<int>() );
	    kdtrees.back ()->load ( lodpage.kdtreepath );
	    lodpage.kdtree = kdtrees.size()-1;
	}
    }

    return true;
}

void LC::free ()
{
    ::free ( sceneEntry );
    ::free ( materialEntry );
    ::free ( layerEntry );
    ::free ( lodEntry );
    ::free ( lodpageEntry );
    ::free ( lineEntry );
    ::free ( triangleEntry );
    ::free ( rectEntry );
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
    globalLCEntry = 0;
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

void LC::updateMinMax ()
{
    int offset = cursor[cursorDepth];
    LevelLCRecord& record = levelLCEntries[cursorDepth]->LCRecords[offset];
    GlobalLCRecord& grecord = globalLCEntry->LCRecords[record.gOffset];
    BBox2d bbox;
    switch ( grecord.type )
    {
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
        vec2f& p0 = quad.data[0];
        vec2f& p1 = quad.data[1];
        vec2f& p2 = quad.data[2];
        vec2f& p3 = quad.data[3];
        bbox.init ( p0 );
        bbox.expandby ( p1 );
        bbox.expandby ( p2 );
        bbox.expandby ( p3 );
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

