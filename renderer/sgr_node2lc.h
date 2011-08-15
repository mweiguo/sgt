#ifndef _NODE2LC_H_
#define _NODE2LC_H_

#include "sgr_lc.h"

class SLCNode;
class SLCNode2LC
{
public:
    SLCNode2LC ();
    void prepareGlobalEntryData ( SLCNode* node );
    void fillGlobalObjEntry ( LC& lc );

    vector<GlobalLCRecord>      globalRecords;
    vector<GroupRecord>    	groupdata;
    vector<LayerRecord>    	layerdata;
    vector<LineRecord>     	linedata;
    vector<TriangleRecord> 	tridata;
    vector<QuadRecord>     	quaddata;
    vector<ModelRecord>    	modeldata;
    vector<AppearanceRecord>    appearancedata;
    vector<MaterialRecord> 	materialdata;
    vector<TextureRecord>  	texturedata;
    vector<MeshRecord>          meshdata;
    vector<VertexRecord>        vertexdata;
    vector<NormalRecord>        normaldata;
    vector<TexCoordRecord>      texcoorddata;
    vector<VertexIndexRecord>   vertexidxdata;
    vector<NormalIndexRecord>   normalidxdata;
    vector<TexCoordIndexRecord> texcoordidxdata;
    vector<TransformRecord>     transformdata;

private:
    void collectNodeRecord ( SLCNode* node );
    void collectChildrenRecords ( SLCNode* node );

    int groupdIdx;
    int layerIdx;
    int lineIdx;
    int triIdx;
    int quadIdx;
    int modelIdx;
    int appIdx;
    int matIdx;
    int texIdx;
    int meshIdx;
    int vertexIdx;
    int normalIdx;
    int texCoordIdx;
    int vertexIndexIdx;
    int normalIndexIdx;
    int texcoordIndexIdx;
    int transformIdx;
};
#endif// _NODE2LC_H_
