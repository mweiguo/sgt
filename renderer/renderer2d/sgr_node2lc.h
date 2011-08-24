#ifndef _NODE2LC_H_
#define _NODE2LC_H_

#include "sgr_lc.h"
#include <map>
using namespace std;

class SLCNode;
class SLCNode2LC
{
public:
    SLCNode2LC ( SLCNode* node );
    LC* generateLC();
    void freeLC ( LC* lc );

    vector<GlobalLCRecord>      globalRecords;
    vector<SceneRecord>    	scenedata;
    vector<MaterialRecord> 	materialdata;
    vector<LayerRecord>    	layerdata;
    vector<LODRecord>    	loddata;
    vector<LODPageRecord>    	lodpagedata;
    vector<LineRecord>     	linedata;
    vector<TriangleRecord> 	tridata;
    vector<RectRecord>     	quaddata;
    map<string, int>            materialMap;

private:
    void collectNodeRecord ( SLCNode* node );
    void collectChildrenRecords ( SLCNode* node );

    int sceneIdx;
    int layerIdx;
    int lodIdx;
    int lodpageIdx;
    int lineIdx;
    int triIdx;
    int rectIdx;
    int matIdx;
};
#endif// _NODE2LC_H_
