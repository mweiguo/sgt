#include "sgr_node2lc.h"
#include "sgr_nodes.h"
#include <list>
#include <cstring>
#include <cstdlib>
using namespace std;

SLCNode2LC::SLCNode2LC ( SLCNode* node )
{
    sceneIdx = 0;
    layerIdx = 0;
    lodIdx = 0;
    lodpageIdx = 0;
    lineIdx = 0;
    triIdx = 0;
    rectIdx = 0;
    matIdx = 0;

    collectNodeRecord ( node );
    collectChildrenRecords ( node );
}

void SLCNode2LC::collectNodeRecord ( SLCNode* node )
{
    int type = node->getType();
    GlobalLCRecord grecord;
    grecord.type  = type;
    grecord.depth = node->getDepth();
    int ii=-1;

    switch ( type )
    {
    case SLC_SCENE:
    {
        SLCSceneNode* group = dynamic_cast<SLCSceneNode*>(node);
        scenedata.push_back ( SceneRecord(group->name) );
        ii = sceneIdx++; break;
    }
    case SLC_MATERIAL:
    {
        SLCMaterial* material = dynamic_cast<SLCMaterial*>(node);
        materialMap.insert ( pair<string,int>(material->name, materialdata.size() ));
        materialdata.push_back ( MaterialRecord ( material->name.c_str(), material->foreground_color, material->background_color, material->linewidth, material->linetype ) );
        ii = matIdx++; break;
    }
    case SLC_LAYER:
    {
        SLCLayerNode* layer = dynamic_cast<SLCLayerNode*>(node);
        map<string, int>::iterator pp = materialMap.find ( layer->bindmat->name );
        if ( pp!=materialMap.end() )
            layerdata.push_back ( LayerRecord(layer->name, layer->flags, pp->second ) );
        ii = layerIdx++; break;
    }
    case SLC_LOD:
    {
        SLCLODNode* lodnode = dynamic_cast<SLCLODNode*>(node);
        LODRecord lodrecord;
        int k=0;
        lodrecord.scalecnt = lodnode->scales.size();
        for ( list<float>::iterator pp=lodnode->scales.begin(); pp!=lodnode->scales.end(); ++pp )
            lodrecord.scales[k++] = *pp;
        loddata.push_back ( lodrecord );
        ii = lodIdx++; break;
    }
    case SLC_LODPAGE:
    {
        SLCLODPageNode* lodpagenode = dynamic_cast<SLCLODPageNode*>(node);
        lodpagedata.push_back ( LODPageRecord ( lodpagenode->kdtree.c_str(), lodpagenode->delayloading, lodpagenode->imposter ) );
        ii = lodpageIdx++; break;
    }
    case SLC_LINE:
    {
        SLCLineNode* line = dynamic_cast<SLCLineNode*>(node);
        linedata.push_back ( LineRecord( line->pnts[0], line->pnts[1] ) );
        ii = lineIdx++; break;
    }
    case SLC_TRIANGLE:
    {
        SLCTriNode* tri = dynamic_cast<SLCTriNode*>(node);
        tridata.push_back ( TriangleRecord( tri->pnts[0], tri->pnts[1], tri->pnts[2] ) );
        ii = triIdx++; break;
    }
    case SLC_RECT:
    {
        SLCRectNode* rc = dynamic_cast<SLCRectNode*>(node);
        quaddata.push_back ( RectRecord(rc->pnts[0], rc->pnts[1], rc->pnts[2], rc->pnts[3] ) );
        ii = rectIdx++; break;
    }
    }
    grecord.value  = ii;
    globalRecords.push_back ( grecord );
}

void SLCNode2LC::collectChildrenRecords ( SLCNode* node )
{
    typedef list<SLCNode*>::iterator NodeIter;
    for ( NodeIter pp=node->children.begin(); pp!=node->children.end(); ++pp )
    {
        collectNodeRecord ( *pp );
        collectChildrenRecords ( *pp );
    }
}

LC* SLCNode2LC::generateLC ()
{
    // alloc memory
    LC *lc = new LC();
    lc->globalLCEntry = (GlobalLCEntry*) malloc( sizeof(int) + ( globalRecords.empty() ? sizeof(GlobalLCRecord) : sizeof(GlobalLCRecord) * globalRecords.size() ));
    lc->sceneEntry    = (SceneEntry*)    malloc( sizeof(int) + ( scenedata.empty() ? sizeof(SceneRecord) : sizeof(SceneRecord) * scenedata.size() ));
    lc->materialEntry = (MaterialEntry*) malloc( sizeof(int) + ( materialdata.empty() ? sizeof(MaterialRecord) : sizeof(MaterialRecord) * materialdata.size() ));
    lc->layerEntry    = (LayerEntry*)    malloc( sizeof(int) + ( layerdata.empty() ? sizeof(LayerRecord) : sizeof(LayerRecord) * layerdata.size() ));
    lc->lodEntry      = (LODEntry*)      malloc( sizeof(int) + ( loddata.empty() ? sizeof(LODRecord) : sizeof(LODRecord) * loddata.size() ));
    lc->lodpageEntry  = (LODPageEntry*)  malloc( sizeof(int) + ( lodpagedata.empty() ? sizeof(LODPageRecord) : sizeof(LODPageRecord) * lodpagedata.size() ));
    lc->lineEntry     = (LineEntry*)     malloc( sizeof(int) + ( linedata.empty() ? sizeof(LineRecord) : sizeof(LineRecord) * linedata.size() ));
    lc->triangleEntry = (TriangleEntry*) malloc( sizeof(int) + ( tridata.empty() ? sizeof(TriangleRecord) : sizeof(TriangleRecord) * tridata.size() ));
    lc->rectEntry     = (RectEntry*)     malloc( sizeof(int) + ( quaddata.empty() ? sizeof(RectRecord) : sizeof(RectRecord) * quaddata.size() ));

    // fill data
    lc->globalLCEntry->LCLen = globalRecords.size();
    if ( lc->globalLCEntry->LCLen > 0 )
        memcpy ( lc->globalLCEntry->LCRecords, &(globalRecords[0]), sizeof(GlobalLCRecord) * globalRecords.size() );

	lc->sceneEntry->LCLen = scenedata.size();
    if ( lc->sceneEntry->LCLen > 0 )
        memcpy ( lc->sceneEntry->LCRecords, &(scenedata[0]), sizeof(SceneRecord) * scenedata.size() );

    lc->materialEntry->LCLen = materialdata.size();
    if ( lc->materialEntry->LCLen > 0 )
        memcpy ( lc->materialEntry->LCRecords, &(materialdata[0]), sizeof(MaterialRecord) * materialdata.size() );

    lc->layerEntry->LCLen = layerdata.size();
    if ( lc->layerEntry->LCLen > 0 )
        memcpy ( lc->layerEntry->LCRecords, &(layerdata[0]), sizeof(LayerRecord) * layerdata.size() );

    lc->lodEntry->LCLen = loddata.size();
    if ( lc->lodEntry->LCLen > 0 )
        memcpy ( lc->lodEntry->LCRecords, &(loddata[0]), sizeof(LODRecord) * loddata.size() );
    
    lc->lodpageEntry->LCLen = lodpagedata.size();
    if ( lc->lodpageEntry->LCLen > 0 )
        memcpy ( lc->lodpageEntry->LCRecords, &(lodpagedata[0]), sizeof(LODPageRecord) * lodpagedata.size() );
    
    lc->lineEntry->LCLen = linedata.size();
    if ( lc->lineEntry->LCLen > 0 )
        memcpy ( lc->lineEntry->LCRecords, &(linedata[0]), sizeof(LineRecord) * linedata.size() );

    lc->triangleEntry->LCLen = tridata.size();
    if ( lc->triangleEntry->LCLen > 0 )
        memcpy ( lc->triangleEntry->LCRecords, &(tridata[0]), sizeof(TriangleRecord) * tridata.size() );

    lc->rectEntry->LCLen = quaddata.size();
    if ( lc->rectEntry->LCLen > 0 )
        memcpy ( lc->rectEntry->LCRecords, &(quaddata[0]), sizeof(RectRecord) * quaddata.size() );

    // build level data
    lc->buildLevelLC ();

    return lc;
}

void SLCNode2LC::freeLC ( LC* lc )
{
    free ( lc->globalLCEntry );
    free ( lc->sceneEntry );
    free ( lc->materialEntry );
    free ( lc->layerEntry );
    free ( lc->lodEntry );
    free ( lc->lodpageEntry );
    free ( lc->lineEntry );
    free ( lc->triangleEntry );
    free ( lc->rectEntry );
    lc->freeLevelLC ();
    delete lc;
}
