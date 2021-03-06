#ifndef _NODE2LC_H_
#define _NODE2LC_H_

#include "sgr_lc.h"
#include <map>
#include <list>
using namespace std;

struct Mat4f
{
    float mat[16];
};

class LC;
class SLCNode;
class SLCNode2LC
{
public:
    SLCNode2LC ( SLCNode* node );
    LC* generateLC();
    void freeLC ( LC* lc );
    void convert ( const char* filename );

    vector<GlobalLCRecord>      globalRecords;
    vector<SceneRecord>    	scenedata;
    vector<MaterialRecord> 	materialdata;
    vector<LayerRecord>    	layerdata;
    vector<LODRecord>    	loddata;
    vector<LODPageRecord>    	lodpagedata;
    vector<LineRecord>     	linedata;
    vector<TriangleRecord> 	tridata;
    vector<RectRecord>     	quaddata;
    vector<PLineRecord>     	plinedata;
    vector<PolyRecord>     	polydata;
    vector<TextRecord>     	textdata;
    vector<char>                textbuffer;
    vector<vec3f>               textsilhouettebuffer;
    vector<vec3f>               plinebuffer;
    vector<vec3f>               polytessellationbuffer;
    vector<vec2f>               texturecoordbuffer;
    map<string, int>            materialMap;

private:
    void collectNodeRecord ( SLCNode* node );
    void collectChildrenRecords ( SLCNode* node );
    void calcCurrentMatrix ();

    int sceneIdx;
    int layerIdx;
    int lodIdx;
    int lodpageIdx;
    int lineIdx;
    int triIdx;
    int rectIdx;
    int plineIdx;
    int polyIdx;
    int textIdx;
    int matIdx;

    list<Mat4f> _current_matrix_stack;
    float _current_matrix[16];
};
#endif// _NODE2LC_H_
