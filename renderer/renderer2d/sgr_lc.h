#ifndef _SLC_LC_H_
#define _SLC_LC_H_

#include <vec2.h>
#include <vec3.h>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
using namespace std;

template < class T>
class KdTree;
// types
struct GlobalLCRecord
{
    GlobalLCRecord();
    int type;
    int depth;
    int value;
    float minmax[4];
};

struct LevelLCRecord
{
    int gOffset;
    int firstChildIdx;
};

struct SceneRecord
{
    SceneRecord ( const char* nm="scene" );
    char name[32];
};

struct MaterialRecord
{
    enum LINETYPE {
        LINETYPE_SOLID,
        LINETYPE_DASH
    };
    MaterialRecord ();
    MaterialRecord ( const char* nm, const vec3i& bg, const vec3i& fg, float lw, int linetype );
    char name[32];
    vec3i backgroud_color;
    vec3i foregroud_color;
    float linewidth;
    int linetype;
};

struct LayerRecord
{
    LayerRecord ( const char* nm="layer", int f=0, int matIdx=-1 );
    char name[32];
    int  flags;
    int  materialIdx;
};

struct LODRecord
{
    int scalecnt;
    float scales[4];
};

struct LODPageRecord
{
    LODPageRecord ();
    LODPageRecord ( const char* kdtree, bool delay, bool imp );
    char kdtreepath[32];
    bool delayloading;
    bool imposter;
    int kdtree;
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

typedef DataGroup2<vec2f> LineRecord;
typedef DataGroup3<vec2f> TriangleRecord;
typedef DataGroup4<vec2f> RectRecord;

typedef DataGroup1<vec3f> VertexRecord;

template < class RecType >
struct LCEntry 
{
    LCEntry () : LCLen(0) {}
    int LCLen;
    RecType LCRecords[1];
};

typedef LCEntry<GlobalLCRecord>      GlobalLCEntry;
typedef LCEntry<LevelLCRecord>       LevelLCEntry;
typedef LCEntry<SceneRecord>         SceneEntry;
typedef LCEntry<MaterialRecord>      MaterialEntry;
typedef LCEntry<LayerRecord>         LayerEntry;
typedef LCEntry<LODRecord>           LODEntry;
typedef LCEntry<LODPageRecord>       LODPageEntry;
typedef LCEntry<LineRecord>          LineEntry;
typedef LCEntry<TriangleRecord>      TriangleEntry;
typedef LCEntry<RectRecord>          RectEntry;
typedef LCEntry<VertexRecord>        VertexEntry;

// modify
struct ModifyCommand
{
    int commandMask;
    int globalIndex;
};

class LC
{
public:
    LC ();
    ~LC ();
    void save ( const char* filename );
    bool load ( const char* filename );
    void free ();

    void buildLevelLC ();
    void freeLevelLC ();
    /** navigation
     */
    int toElement ( int direction );
    int getType ();
    string getTypeStr ();
    int getDepth ();
    int getValue ();
    int getGIndex ();
    float* getMinMax ();
    void setMinMax ( float minx, float miny, float maxx, float maxy );
    void updateMinMax ();
    /** modify
     */
    void insertBeforeElement ();
    void insertAfterElement ();
    void deleteElement ();
    void updateElement ();
    void endEdit();
    GlobalLCEntry* globalLCEntry;
    LevelLCEntry* levelLCEntries[256];

    SceneEntry           *sceneEntry;
    MaterialEntry        *materialEntry;
    LayerEntry           *layerEntry;
    LODEntry             *lodEntry;
    LODPageEntry         *lodpageEntry;
    LineEntry            *lineEntry;
    TriangleEntry        *triangleEntry;
    RectEntry            *rectEntry;

    // kdtree support
    vector< KdTree<int>* > kdtrees;
protected:
    // navigation
    int cursorDepth;  // base from 0
    int cursor[256];  // store offset for each levelLCEntry
    vector<ModifyCommand> modifyCommands;
};

struct GetPrimitiveCenter
{
    void init ( LC* lc )
    {
        _lc = lc;
    }
    vec2f operator() ( int gidx ) const
    {
        const GlobalLCRecord& grcd = _lc->globalLCEntry->LCRecords[gidx];
        vec2f rtn ( (grcd.minmax[0] + grcd.minmax[2])/2.0, 
                    (grcd.minmax[1] + grcd.minmax[3])/2.0 );
        return rtn;
    }
private:
    LC* _lc;
};

struct GetPrimitiveMinMax
{
    void init ( LC* lc )
    {
        _lc = lc;
    }
    void operator() ( int gidx, float* minmax ) const
    {
        memcpy ( minmax, _lc->globalLCEntry->LCRecords[gidx].minmax, sizeof(float)*4 );
    }
private:
    LC* _lc;
};

#define ROOT          0
#define PARENT        1
#define FIRST_CHILD   2
#define LAST_CHILD    3
#define NEXT_SIBLING  4

#define CMD_INSERT_BEFORE_ELEMENT  0
#define CMD_INSERT_AFTER_ELEMENT   1
#define CMD_DELETE_ELEMENT         2


#endif

