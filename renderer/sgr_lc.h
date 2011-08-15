#ifndef _SLC_LC_H_
#define _SLC_LC_H_

#include <vec3.h>
#include <vector>
#include <string>
using namespace std;

// types
struct GlobalLCRecord
{
    int type;
    int depth;
    int value;
};

struct LevelLCRecord
{
    int gOffset;
    int firstChildIdx;
};

struct GroupRecord
{
    GroupRecord ( const char* nm="group" );
    char name[32];
};

struct LayerRecord
{
    LayerRecord ( const char* nm="layer", int f=0 );
    char name[32];
    int  flags;
};

template <class T >
struct DataGroup1
{
    DataGroup1 () {}
    DataGroup1 ( const T& p0 ) { data = p0; }
    T data;
};

template <class T >
struct DataGroup2
{
    DataGroup2 () {}
    DataGroup2 ( const T& p0, const T& p1 ) { data[0] = p0; data[1] = p1; }
    T data[2];
};

template <class T>
struct DataGroup3
{
    DataGroup3 () {}
    DataGroup3 ( const T& p0, const T& p1, const T& p2 ) { data[0] = p0; data[1] = p1; data[2] = p2; }
    T data[3];
};

template <class T>
struct DataGroup4
{
    DataGroup4 () {}
    DataGroup4 ( const T& p0, const T& p1, const T& p2, const T& p3 ) { data[0] = p0; data[1] = p1; data[2] = p2; data[3] = p3; }
    T data[4];
};

typedef DataGroup2<vec3f> LineRecord;
typedef DataGroup3<vec3f> TriangleRecord;
typedef DataGroup4<vec3f> QuadRecord;
// typedef DataGroup3<int>   TriangleIndexRecord;
// typedef DataGroup4<int>   QuadIndexRecord;

struct ModelRecord
{
    ModelRecord ( const char* nm="group" );
    char name[32];
};

struct AppearanceRecord
{
    AppearanceRecord ();
//    AppearanceRecord ( int material, int texture, int mesh );
//     int materialIdx;
//     int textureIdx;
//     int meshIdx;
};

struct MaterialRecord
{
    MaterialRecord ();
    MaterialRecord ( const vec3f& diff, const vec3f& amb, const vec3f& spec );
    vec3f diffuse;
    vec3f ambient;
    vec3f specular;
};

struct TextureRecord
{
    TextureRecord ();
    TextureRecord ( const char* name, const char* filename );
    char name[32];
    char filename[32];
};

struct MeshRecord
{
    MeshRecord ();
    int vertexOffset,        vertexLength;
    int normalOffset,        normalLength;
    int texCoordOffset,      texCoordLength;
    int vertexIndexOffset,   vertexIndexLength;
    int normalIndexOffset,   normalIndexLength;
    int texCoordIndexOffset, texCoordIndexLength;
};

typedef DataGroup1<vec3f> VertexRecord;
typedef DataGroup1<vec3f> NormalRecord;
typedef DataGroup1<vec2f> TexCoordRecord;
typedef DataGroup1<int>   VertexIndexRecord;
typedef DataGroup1<int>   TexCoordIndexRecord;
typedef DataGroup1<int>   NormalIndexRecord;

struct TransformRecord
{
    TransformRecord ();
    TransformRecord ( float* mat );
    float m[16];
};

template < class RecType >
struct LCEntry 
{
    LCEntry () : LCLen(0) {}
    int LCLen;
    RecType LCRecords[1];
};

typedef LCEntry<GlobalLCRecord>      GlobalLCEntry;
typedef LCEntry<LevelLCRecord>       LevelLCEntry;
typedef LCEntry<GroupRecord>         GroupEntry;
typedef LCEntry<LayerRecord>         LayerEntry;
typedef LCEntry<LineRecord>          LineEntry;
typedef LCEntry<TriangleRecord>      TriangleEntry;
typedef LCEntry<QuadRecord>          QuadEntry;
typedef LCEntry<ModelRecord>         ModelEntry;
typedef LCEntry<AppearanceRecord>    AppearanceEntry;
typedef LCEntry<MaterialRecord>      MaterialEntry;
typedef LCEntry<TextureRecord>       TextureEntry;
typedef LCEntry<MeshRecord>          MeshEntry;
typedef LCEntry<VertexRecord>        VertexEntry;
typedef LCEntry<NormalRecord>        NormalEntry;
typedef LCEntry<TexCoordRecord>      TexCoordEntry;
typedef LCEntry<VertexIndexRecord>   VertexIndexEntry;
typedef LCEntry<TexCoordIndexRecord> TexCoordIndexEntry;
typedef LCEntry<NormalIndexRecord>   NormalIndexEntry;
typedef LCEntry<TransformRecord>   TransformEntry;

// modify
struct ModifyCommand
{
    int commandMask;
    int globalIndex;
};

struct ObjSize
{
    int GroupEntrySize;
    int LayerEntrySize;
    int LineEntrySize;
    int TriangleEntrySize;
    int QuadEntrySize;
    int ModelEntrySize;
    int AppearanceEntrySize;
    int MaterialEntrySize;
    int TextureEntrySize;
    int MeshEntrySize;
    int VertexEntrySize;
    int NormalEntrySize;
    int TexCoordEntrySize;
    int VertexIndexEntrySize;
    int NormalIndexEntrySize;
    int TexCoordIndexEntrySize;
    int TransformEntrySize;
};

class LC
{
public:
    LC ();
    ~LC ();
    void saveLC ( const char* filename );
    bool loadLC ( const char* filename );
    void allocGlobalEntry ( int byteSize );
    void buildLevelLC ();
    void allocObjEntries ( const ObjSize& sz );

    void freeLevelLC ();
    void freeWholeBuffer ();
    void freeGlobalEntry ();
    void freeObjEntries ();
    /** navigation
     */
    int toElement ( int direction );
    int getType ();
    string getTypeStr ();
    int getDepth ();
    int getValue ();
    int getGIndex ();
    /** modify
     */
    void insertBeforeElement ();
    void insertAfterElement ();
    void deleteElement ();
    void updateElement ();
    void endEdit();
    GlobalLCEntry* globalLCEntry;
    LevelLCEntry* levelLCEntries[256];

    GroupEntry      	 *groupEntry;
    LayerEntry      	 *layerEntry;
    LineEntry       	 *lineEntry;
    TriangleEntry   	 *triangleEntry;
    QuadEntry       	 *quadEntry;
    ModelEntry      	 *modelEntry;
    AppearanceEntry 	 *appearanceEntry;
    MaterialEntry   	 *materialEntry;
    TextureEntry    	 *textureEntry;
    MeshEntry            *meshEntry;
    VertexEntry          *vertexEntry;
    NormalEntry          *normalEntry;
    TexCoordEntry   	 *texCoordEntry;
    VertexIndexEntry     *vertexIndexEntry;
    NormalIndexEntry     *normalIndexEntry;
    TexCoordIndexEntry   *texCoordIndexEntry;
    TransformEntry       *transformEntry;
protected:
    // navigation
    int cursorDepth;  // base from 0
    int cursor[256];  // store offset for each levelLCEntry
    vector<ModifyCommand> modifyCommands;
};

#define ROOT          0
#define PARENT        1
#define FIRST_CHILD   2
#define LAST_CHILD    3
#define NEXT_SIBLING  4

#define CMD_INSERT_BEFORE_ELEMENT  0
#define CMD_INSERT_AFTER_ELEMENT   1
#define CMD_DELETE_ELEMENT         2

#define SLC_GROUP           0
#define SLC_LAYER           1
#define SLC_PLINE           2
#define SLC_POLY            3
#define SLC_LINE            4
#define SLC_TRIANGLE        5
#define SLC_QUAD            6

#define SLC_MODEL           100
#define SLC_APPEARANCE      110
#define SLC_PHONG_MATERIAL  120
#define SLC_BLINN_MATERIAL  121
#define SLC_TEXTURE         130
#define SLC_MESH            140
#define SLC_VERTEX          150
#define SLC_NORMAL          151
#define SLC_TEXCOORD        152
#define SLC_VERTEXINDEX     153
#define SLC_NORMALINDEX     154
#define SLC_TEXCOORDINDEX   155
#define SLC_TRANSFORM       160

#endif

