#include "sgr_lc.h"
#include "sgr_nodes.h"
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

GroupRecord::GroupRecord ( const char* nm )
{
    memset  ( name,  0, 32 * sizeof(char) ); 
    strncpy ( name, nm, 32 ); 
}

// --------------------------------------------------------------------------------

LayerRecord::LayerRecord ( const char* nm, int f )
{
    memset  ( name,  0, 32 * sizeof(char) ); 
    strncpy ( name, nm, 32 ); 
    flags = f; 
}

// --------------------------------------------------------------------------------

ModelRecord::ModelRecord ( const char* nm )
{
    memset  ( name,  0, 32 * sizeof(char) ); 
    strncpy ( name, nm, 32 ); 
}

// --------------------------------------------------------------------------------

AppearanceRecord::AppearanceRecord ()
{
}

// --------------------------------------------------------------------------------

MaterialRecord::MaterialRecord ()
{
    diffuse.xyz ( 0.8, 0.8, 0.8 );
    ambient.xyz ( 0.2, 0.2, 0.2 );
    specular.xyz ( 0.2, 0.2, 0.2 );
}

MaterialRecord::MaterialRecord ( const vec3f& diff, const vec3f& amb, const vec3f& spec )
{
    diffuse  = diff;
    ambient  = amb;
    specular = spec;
}

// --------------------------------------------------------------------------------

TextureRecord::TextureRecord ()
{
    memset ( name,     0, 32*sizeof(char)); 
    memset ( filename, 0, 32*sizeof(char)); 
}

TextureRecord::TextureRecord ( const char* Name, const char* FileName )
{
    memcpy ( name,     Name,     32*sizeof(char) );
    memcpy ( filename, FileName, 32*sizeof(char) );
}

// --------------------------------------------------------------------------------

MeshRecord::MeshRecord ()
{
    vertexOffset = vertexLength =  normalOffset = normalLength = texCoordOffset = texCoordLength = vertexIndexOffset = vertexIndexLength = normalIndexOffset = normalIndexLength = texCoordIndexOffset = texCoordIndexLength = -1;
}

// --------------------------------------------------------------------------------

TransformRecord::TransformRecord ()
{
    memset ( m, 0, sizeof(float)*16 );
}

TransformRecord::TransformRecord ( float* mat )
{
    memcpy ( m, mat, sizeof(float)*16 );
}

// --------------------------------------------------------------------------------

LC::LC ()
{
    globalLCEntry = 0;
    memset ( levelLCEntries, 0, sizeof(LevelLCEntry*)*256);
    groupEntry         = 0;
    layerEntry         = 0;
    lineEntry          = 0;
    triangleEntry      = 0;
    quadEntry          = 0;
    modelEntry         = 0;
    appearanceEntry    = 0;
    materialEntry      = 0;
    textureEntry       = 0;
    meshEntry          = 0;
    vertexEntry        = 0;
    normalEntry        = 0;
    texCoordEntry      = 0;
    vertexIndexEntry   = 0;
    normalIndexEntry   = 0;
    texCoordIndexEntry = 0;
    transformEntry     = 0;

    cursorDepth = 0;
    memset ( cursor, 0, sizeof(int)*256);
}

LC::~LC ()
{
}

void LC::saveLC ( const char* filename )
{
    ofstream out;
    out.open ( filename, ofstream::binary );

    // write group data
    if ( NULL==groupEntry || 0==groupEntry->LCLen ) {
        GroupEntry tmp; out.write ( (char*)&tmp, sizeof(GroupEntry) );
    } else
        out.write ( (char*)groupEntry, groupEntry->LCLen * sizeof(GroupRecord) + sizeof(int) );
    // write layer data
    if ( NULL==layerEntry || 0==layerEntry->LCLen ) {
        LayerEntry tmp; out.write ( (char*)&tmp, sizeof(LayerEntry) );
    } else
        out.write ( (char*)layerEntry, layerEntry->LCLen * sizeof(LayerRecord) + sizeof(int) );
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
    // write quad data
    if ( NULL==quadEntry || 0==quadEntry->LCLen ) {
        QuadEntry tmp; out.write ( (char*)&tmp, sizeof(QuadEntry) );
    } else
        out.write ( (char*)quadEntry, quadEntry->LCLen * sizeof(QuadRecord) + sizeof(int) );
    // write model data
    if ( NULL==modelEntry || 0==modelEntry->LCLen ) {
        ModelEntry tmp; out.write ( (char*)&tmp, sizeof(ModelEntry) );
    } else
        out.write ( (char*)modelEntry, modelEntry->LCLen * sizeof(ModelRecord) + sizeof(int) );
    // write appearance data
    if ( NULL==appearanceEntry || 0==appearanceEntry->LCLen ) {
        AppearanceEntry tmp; out.write ( (char*)&tmp, sizeof(AppearanceEntry) );
    } else
        out.write ( (char*)appearanceEntry, appearanceEntry->LCLen * sizeof(AppearanceRecord) + sizeof(int) );
    // write meterial data
    if ( NULL==materialEntry || 0==materialEntry->LCLen ) {
        MaterialEntry tmp; out.write ( (char*)&tmp, sizeof(MaterialEntry) );
    } else
        out.write ( (char*)materialEntry, materialEntry->LCLen * sizeof(MaterialRecord) + sizeof(int) );
    // write texture data
    if ( NULL==textureEntry || 0==textureEntry->LCLen ) {
        TextureEntry tmp; out.write ( (char*)&tmp, sizeof(TextureEntry) );
    } else
        out.write ( (char*)textureEntry, textureEntry->LCLen * sizeof(TextureRecord) + sizeof(int) );
    // write mesh data
    if ( NULL==meshEntry || 0==meshEntry->LCLen ) {
        MeshEntry tmp; out.write ( (char*)&tmp, sizeof(MeshEntry) );
    } else
        out.write ( (char*)meshEntry, meshEntry->LCLen * sizeof(MeshRecord) + sizeof(int) );
    // write vertex data
    if ( NULL==vertexEntry || 0==vertexEntry->LCLen ) {
        VertexEntry tmp; out.write ( (char*)&tmp, sizeof(VertexEntry) );
    } else
        out.write ( (char*)vertexEntry, vertexEntry->LCLen * sizeof(VertexRecord) + sizeof(int) );
    // write normal data
    if ( NULL==normalEntry || 0==normalEntry->LCLen ) {
        NormalEntry tmp; out.write ( (char*)&tmp, sizeof(NormalEntry) );
    } else
        out.write ( (char*)normalEntry, normalEntry->LCLen * sizeof(NormalRecord) + sizeof(int) );
    // write texCoord data
    if ( NULL==texCoordEntry || 0==texCoordEntry->LCLen ) {
        TexCoordEntry tmp; out.write ( (char*)&tmp, sizeof(TexCoordEntry) );
    } else
        out.write ( (char*)texCoordEntry, texCoordEntry->LCLen * sizeof(TexCoordRecord) + sizeof(int) );
    // write vertexIndex data
    if ( NULL==vertexIndexEntry || 0==vertexIndexEntry->LCLen ) {
        VertexIndexEntry tmp; out.write ( (char*)&tmp, sizeof(VertexIndexEntry) );
    } else
        out.write ( (char*)vertexIndexEntry, vertexIndexEntry->LCLen * sizeof(VertexIndexRecord) + sizeof(int) );
    // write normalIndex data
    if ( NULL==normalIndexEntry || 0==normalIndexEntry->LCLen ) {
        NormalIndexEntry tmp; out.write ( (char*)&tmp, sizeof(NormalIndexEntry) );
    } else
        out.write ( (char*)normalIndexEntry, normalIndexEntry->LCLen * sizeof(NormalIndexRecord) + sizeof(int) );
    // write texCoordIndex data
    if ( NULL==texCoordIndexEntry || 0==texCoordIndexEntry->LCLen ) {
        TexCoordIndexEntry tmp; out.write ( (char*)&tmp, sizeof(TexCoordIndexEntry) );
    } else
        out.write ( (char*)texCoordIndexEntry, texCoordIndexEntry->LCLen * sizeof(TexCoordIndexRecord) + sizeof(int) );
    // write transformEntry data
    if ( NULL==transformEntry || 0==transformEntry->LCLen ) {
        TransformEntry tmp; out.write ( (char*)&tmp, sizeof(TransformEntry) );
    } else
        out.write ( (char*)transformEntry, transformEntry->LCLen * sizeof(TransformEntry) + sizeof(int) );

    // write globalLCEntry
    cout << "globalLCEntry size : " << globalLCEntry->LCLen * sizeof(GlobalLCRecord) + sizeof(int) << endl;
    out.write ( (char*)globalLCEntry, globalLCEntry->LCLen * sizeof(GlobalLCRecord) + sizeof(int) );
    // write levelLCEntry
    int levelLCEntriesSize = 0;
    for ( int i=0; i<256; i++ ) {
        if ( levelLCEntries[i] != 0 ) {
            cout << "levelLCEntries[" << i << "] size : " << levelLCEntries[i]->LCLen * sizeof(LevelLCRecord) + sizeof(int) << endl;
            out.write ( (char*)levelLCEntries[i], levelLCEntries[i]->LCLen * sizeof(LevelLCRecord) + sizeof(int) );
            levelLCEntriesSize += levelLCEntries[i]->LCLen * sizeof(LevelLCRecord) + sizeof(int);
        }
        else
            break;
    }
    cout << "levelLCEntries total size : " << levelLCEntriesSize << endl;
        
    out.close ();
}

// build location cache
bool LC::loadLC ( const char* filename )
{
    ifstream in;
    in.open ( filename, ifstream::binary );
    if ( in.is_open () == false ) {
        cout << "open file " << filename << " error" << endl;
        return false;
    }
    in.seekg (0, ios::end);
    int length = in.tellg();
    in.seekg (0, ios::beg);
    // allocate memory:
    char* buffer = (char*)malloc(sizeof(char) * length);
    in.read (buffer,length);
    in.close();

    int offset = 0;
    // read group data
    groupEntry = (GroupEntry*)(buffer + offset);
    offset += sizeof(int) + (groupEntry->LCLen==0 ? sizeof(GroupRecord) : sizeof(GroupRecord) * groupEntry->LCLen);
    // read layer data
    layerEntry = (LayerEntry*)(buffer + offset);
    offset += sizeof(int) + (layerEntry->LCLen==0 ? sizeof(LayerRecord) : sizeof(LayerRecord) * layerEntry->LCLen);
    // read line data
    lineEntry  = (LineEntry*)(buffer + offset);
    offset += sizeof(int) + (lineEntry->LCLen==0 ? sizeof(LineRecord) : sizeof(LineRecord) * lineEntry->LCLen);
    // read triangle data
    triangleEntry = (TriangleEntry*)(buffer + offset);
    offset += sizeof(int) + (triangleEntry->LCLen==0 ? sizeof(TriangleRecord) : sizeof(TriangleRecord) * triangleEntry->LCLen);
    // read quad data
    quadEntry = (QuadEntry*)(buffer + offset);
    offset += sizeof(int) + (quadEntry->LCLen==0 ? sizeof(QuadRecord) : sizeof(QuadRecord) * quadEntry->LCLen);
    // read model data
    modelEntry = (ModelEntry*)(buffer + offset);
    offset += sizeof(int) + (modelEntry->LCLen==0 ? sizeof(ModelRecord) : sizeof(ModelRecord) * modelEntry->LCLen);
    // read appearance data
    appearanceEntry = (AppearanceEntry*)(buffer + offset);
    offset += sizeof(int) + (appearanceEntry->LCLen==0 ? sizeof(AppearanceRecord) : sizeof(AppearanceRecord) * appearanceEntry->LCLen);
    // read meterial data
    materialEntry = (MaterialEntry*)(buffer + offset);
    offset += sizeof(int) + (materialEntry->LCLen==0 ? sizeof(MaterialRecord) : sizeof(MaterialRecord) * materialEntry->LCLen);
    // read texture data
    textureEntry = (TextureEntry*)(buffer + offset);
    offset += sizeof(int) + (textureEntry->LCLen==0 ? sizeof(TextureRecord) : sizeof(TextureRecord) * textureEntry->LCLen);
    // read mesh data
    meshEntry = (MeshEntry*)(buffer + offset);
    offset += sizeof(int) + (meshEntry->LCLen==0 ? sizeof(MeshRecord) : sizeof(MeshRecord) * meshEntry->LCLen);
    // read vertex data
    vertexEntry = (VertexEntry*)(buffer + offset);
    offset += sizeof(int) + (vertexEntry->LCLen==0 ? sizeof(VertexRecord) : sizeof(VertexRecord) * vertexEntry->LCLen);
    // read normal data
    normalEntry = (NormalEntry*)(buffer + offset);
    offset += sizeof(int) + (normalEntry->LCLen==0 ? sizeof(NormalRecord) : sizeof(NormalRecord) * normalEntry->LCLen);
    // read texCoord data
    texCoordEntry = (TexCoordEntry*)(buffer + offset);
    offset += sizeof(int) + (texCoordEntry->LCLen==0 ? sizeof(TexCoordRecord) : sizeof(TexCoordRecord) * texCoordEntry->LCLen);
    // read vertexIndex data
    vertexIndexEntry = (VertexIndexEntry*)(buffer + offset);
    offset += sizeof(int) + (vertexIndexEntry->LCLen==0 ? sizeof(VertexIndexRecord) : sizeof(VertexIndexRecord) * vertexIndexEntry->LCLen);
    // read normalIndex data
    normalIndexEntry = (NormalIndexEntry*)(buffer + offset);
    offset += sizeof(int) + (normalIndexEntry->LCLen==0 ? sizeof(NormalIndexRecord) : sizeof(NormalIndexRecord) * normalIndexEntry->LCLen);
    // read texCoordIndex data
    texCoordIndexEntry = (TexCoordIndexEntry*)(buffer + offset);
    offset += sizeof(int) + (texCoordIndexEntry->LCLen==0 ? sizeof(TexCoordIndexRecord) : sizeof(TexCoordIndexRecord) * texCoordIndexEntry->LCLen);
    // read texCoordIndex data
    transformEntry = (TransformEntry*)(buffer + offset);
    offset += sizeof(int) + (transformEntry->LCLen==0 ? sizeof(TransformRecord) : sizeof(TransformRecord) * transformEntry->LCLen);

    // read GlobalLCEntry
    globalLCEntry = (GlobalLCEntry*)(buffer+offset);
    // read levelLCEntries
    offset += sizeof(int) + globalLCEntry->LCLen * sizeof(GlobalLCRecord);
    for ( int i=0; i<256; i++ )
    {
        levelLCEntries[i] = (LevelLCEntry*)(buffer + offset);
        offset += sizeof(int) + levelLCEntries[i]->LCLen * sizeof(LevelLCRecord);
        if ( offset >= length )
            break;
    }
    return true;
}

void LC::allocGlobalEntry ( int byteSize )
{
    globalLCEntry = (GlobalLCEntry*)malloc ( byteSize );
}

void LC::buildLevelLC ()
{
    vector<int> gIndexEntries[256];
    vector<int> firstChildEntries[256];
    for ( int i=0; i<globalLCEntry->LCLen; i++ ) {
        GlobalLCRecord& record = globalLCEntry->LCRecords[i];
        int depth = record.depth;
        vector<int>& gIndexEntry = gIndexEntries[depth];
        if ( globalLCEntry->LCRecords[i+1].depth == depth + 1 )
            firstChildEntries[depth].push_back ( gIndexEntries[depth+1].size() );
        else
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

            for ( int j=0; j<gIndexEntry.size(); j++ ) {
                tempEntry->LCRecords[j].gOffset       = gIndexEntry[j];
                tempEntry->LCRecords[j].firstChildIdx = firstChildEntry[j];
            }
        } else 
            levelLCEntries[i] = 0;
    }
}

void LC::allocObjEntries ( const ObjSize& sz )
{
    groupEntry = (GroupEntry*)malloc(sz.GroupEntrySize);
    layerEntry = (LayerEntry*)malloc(sz.LayerEntrySize);
    lineEntry  = (LineEntry*)malloc(sz.LineEntrySize);
    triangleEntry = (TriangleEntry*)malloc(sz.TriangleEntrySize);
    quadEntry  = (QuadEntry*)malloc(sz.QuadEntrySize);
    modelEntry = (ModelEntry*)malloc(sz.ModelEntrySize);
    appearanceEntry = (AppearanceEntry*)malloc(sz.AppearanceEntrySize);
    materialEntry = (MaterialEntry*)malloc(sz.MaterialEntrySize);
    textureEntry  = (TextureEntry*)malloc(sz.TextureEntrySize);
    meshEntry = (MeshEntry*)malloc(sz.MeshEntrySize);
    vertexEntry = (VertexEntry*)malloc(sz.VertexEntrySize);
    normalEntry = (NormalEntry*)malloc(sz.NormalEntrySize);
    texCoordEntry = (TexCoordEntry*)malloc(sz.TexCoordEntrySize);
    vertexIndexEntry = (VertexIndexEntry*)malloc(sz.VertexIndexEntrySize);
    normalIndexEntry = (NormalIndexEntry*)malloc(sz.NormalIndexEntrySize);
    texCoordIndexEntry = (TexCoordIndexEntry*)malloc(sz.TexCoordIndexEntrySize);
    transformEntry = (TransformEntry*)malloc(sz.TransformEntrySize);
}

void LC::freeLevelLC ()
{
    for ( int i=0; i<256; i++ )
    {
        if ( levelLCEntries[i] != 0 ) {
            free ( levelLCEntries[i] );
            levelLCEntries[i] = 0;
        }
    }
    cursorDepth = 0;
    memset ( cursor, 0, sizeof(int)*256);
}

void LC::freeWholeBuffer ()
{
    free ( (void*)groupEntry );
    groupEntry = 0;
    layerEntry = 0;
    lineEntry  = 0;
    triangleEntry = 0;
    quadEntry = 0;
    modelEntry = 0;
    appearanceEntry = 0;
    materialEntry = 0;
    textureEntry = 0;
    meshEntry = 0;
    vertexEntry = 0;
    normalEntry = 0;
    texCoordEntry = 0;
    vertexIndexEntry = 0;
    normalIndexEntry = 0;
    texCoordIndexEntry = 0;
    transformEntry = 0;
    globalLCEntry = 0;
    memset ( levelLCEntries, 0, sizeof(LevelLCEntry*)*256);
    cursorDepth = 0;
    memset ( cursor, 0, sizeof(int)*256);
}

void LC::freeGlobalEntry ()
{
    free ( globalLCEntry );
    globalLCEntry = 0;
}

void LC::freeObjEntries ()
{
    free ( groupEntry );             
    free ( layerEntry );             
    free ( lineEntry );      
    free ( triangleEntry );     
    free ( quadEntry );      
    free ( modelEntry );             
    free ( appearanceEntry );   
    free ( materialEntry );     
    free ( textureEntry );           
    free ( meshEntry );      
    free ( vertexEntry );            
    free ( normalEntry );            
    free ( texCoordEntry );     
    free ( vertexIndexEntry );  
    free ( normalIndexEntry );  
    free ( texCoordIndexEntry );
    free ( transformEntry );

    groupEntry = 0;
    layerEntry = 0;          
    lineEntry = 0;           
    triangleEntry = 0;     
    quadEntry = 0;           
    modelEntry = 0;          
    appearanceEntry = 0;   
    materialEntry = 0;     
    textureEntry = 0;        
    meshEntry = 0;           
    vertexEntry = 0;         
    normalEntry = 0;         
    texCoordEntry = 0;     
    vertexIndexEntry = 0;  
    normalIndexEntry = 0;  
    texCoordIndexEntry = 0;
    transformEntry = 0;
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
    case SLC_GROUP:
        return "GroupNode";
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
    case SLC_QUAD:
        return "QuadNode";
    case SLC_MODEL:
        return "ModelNode";
    case SLC_APPEARANCE:
        return "AppearanceNode";
    case SLC_PHONG_MATERIAL:
        return "MaterialNode";
    case SLC_TEXTURE:
        return "TextureNode";
    case SLC_MESH:
        return "MeshNode";
    case SLC_VERTEX:
        return "VertexNode";
    case SLC_NORMAL:
        return "NormalNode";
    case SLC_TEXCOORD:
        return "TexCoordNode";
    case SLC_VERTEXINDEX:
        return "VertexIndexNode";
    case SLC_NORMALINDEX:
        return "NormalIndexNode";
    case SLC_TEXCOORDINDEX:
        return "NormalIndexNode";
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
    for ( int i=0; i<modifyCommands.size(); i++ )
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
    free ( globalLCEntry );
    int sz = temp.size() * sizeof(GlobalLCRecord) + sizeof(int);
    globalLCEntry = (GlobalLCEntry*)malloc ( sz );
    memset ( globalLCEntry, 0, sz );
    globalLCEntry->LCLen = temp.size();
    memcpy ( globalLCEntry->LCRecords, &(temp[0]), globalLCEntry->LCLen * sizeof(GlobalLCRecord) );
    
    // clear modify command queue
    modifyCommands.clear();

    buildLevelLC ();
}

