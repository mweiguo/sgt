#include "sgr_node2lc.h"
#include "sgr_nodes.h"
#include <list>
#include <cstring>
using namespace std;

SLCNode2LC::SLCNode2LC ()
{
    groupdIdx = 0;
    layerIdx = 0;
    lineIdx = 0;
    triIdx = 0;
    quadIdx = 0;
    modelIdx = 0;
    appIdx = 0;
    matIdx = 0;
    texIdx = 0;
    meshIdx = 0;
    vertexIdx = 0;
    normalIdx = 0;
    texCoordIdx = 0;
    vertexIndexIdx = 0;
    normalIndexIdx = 0;
    texcoordIndexIdx = 0;
    transformIdx = 0;
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
    case SLC_GROUP:
    {
        SLCGroupNode* group = dynamic_cast<SLCGroupNode*>(node);
        groupdata.push_back ( GroupRecord(group->name) );
        ii = groupdIdx++;
    }
    case SLC_LAYER:
    {
        SLCLayerNode* layer = dynamic_cast<SLCLayerNode*>(node);
        layerdata.push_back ( LayerRecord(layer->name, layer->flags) );
        ii = layerIdx++;
    }
    case SLC_LINE:
    {
        SLCLineNode* line = dynamic_cast<SLCLineNode*>(node);
        linedata.push_back ( LineRecord( line->pnts[0], line->pnts[1] ) );
        ii = lineIdx++;
    }
    case SLC_TRIANGLE:
    {
        SLCTriNode* tri = dynamic_cast<SLCTriNode*>(node);
        tridata.push_back ( TriangleRecord( tri->pnts[0], tri->pnts[1], tri->pnts[2] ) );
        ii = triIdx++;
    }
    case SLC_QUAD:
    {
        SLCQuadNode* quad = dynamic_cast<SLCQuadNode*>(node);
        quaddata.push_back ( QuadRecord(quad->pnts[0], quad->pnts[1], quad->pnts[2], quad->pnts[3] ) );
        ii = quadIdx++;
    }
    case SLC_MODEL:
    {
        SLCModel* model = dynamic_cast<SLCModel*>(node);
        modeldata.push_back ( ModelRecord ( model->name ) );
        ii = modelIdx++;
    }
    case SLC_APPEARANCE:
    {
        SLCAppearance* appearance = dynamic_cast<SLCAppearance*>(node);
        appearancedata.push_back ( AppearanceRecord() );
        ii = appIdx++;
    }
    case SLC_PHONG_MATERIAL:
    {
        SLCPhongMaterial* material = dynamic_cast<SLCPhongMaterial*>(node);
        materialdata.push_back ( MaterialRecord ( material->diffuse, material->ambient, material->specular ) );
        ii = matIdx++;
    }
    case SLC_TEXTURE:
    {
        SLCTexture* texture = dynamic_cast<SLCTexture*>(node);
        texturedata.push_back ( TextureRecord ( texture->name, texture->filename ) );
        ii = texIdx++;
    }
    case SLC_MESH:
    {
        SLCMesh* mesh = dynamic_cast<SLCMesh*>(node);
        meshdata.push_back ( MeshRecord() );
        ii = meshIdx++;
    }
    case SLC_VERTEX:
    {
        SLCVertex* vertex = dynamic_cast<SLCVertex*>(node);
        SLCMesh* mesh = dynamic_cast<SLCMesh*>(vertex->parent);
        MeshRecord& mr = meshdata[meshIdx-1];
        mr.vertexOffset = vertexdata.size();
        mr.vertexLength = vertex->data.size();
        mesh->vertexOffset = vertexdata.size();
        for ( int i=0; i<vertex->data.size(); i++ )
            vertexdata.push_back ( VertexRecord ( vertex->data[i] ) );
        ii = vertexIdx++;
    }
    case SLC_NORMAL:
    {
        SLCNormal* normal = dynamic_cast<SLCNormal*>(node);
        SLCMesh* mesh = dynamic_cast<SLCMesh*>(normal->parent);
        MeshRecord& mr = meshdata[meshIdx-1];
        mr.normalOffset = normaldata.size();
        mr.normalLength = normal->data.size();
        mesh->normalOffset = normaldata.size();
        for ( int i=0; i<normal->data.size(); i++ )
            normaldata.push_back ( NormalRecord ( normal->data[i] ) );
        ii = normalIdx++;
    }
    case SLC_TEXCOORD:
    {
        SLCTexCoord* texcoord = dynamic_cast<SLCTexCoord*>(node);
        SLCMesh* mesh        = dynamic_cast<SLCMesh*>(texcoord->parent);
        MeshRecord& mr       = meshdata[meshIdx-1];
        mr.texCoordOffset    = texcoorddata.size();
        mr.texCoordLength    = texcoord->data.size();
        mesh->texcoordOffset = texcoorddata.size();
        for ( int i=0; i<texcoord->data.size(); i++ )
            texcoorddata.push_back ( TexCoordRecord ( texcoord->data[i] ) );
        ii = texCoordIdx++;
    }
    case SLC_VERTEXINDEX:
    {
        SLCVertexIndex* vertexIndex = dynamic_cast<SLCVertexIndex*>(node);
        SLCMesh* mesh = dynamic_cast<SLCMesh*>(vertexIndex->parent);
        MeshRecord& mr       = meshdata[meshIdx-1];
        mr.vertexIndexOffset = vertexidxdata.size();
        mr.vertexIndexLength = vertexIndex->data.size();
        for ( int i=0; i<vertexIndex->data.size(); i++ )
            vertexidxdata.push_back ( VertexIndexRecord ( vertexIndex->data[i] + mesh->vertexOffset ) );
        ii = vertexIndexIdx++;
    }
    case SLC_NORMALINDEX:
    {
        SLCNormalIndex* normalIndex = dynamic_cast<SLCNormalIndex*>(node);
        SLCMesh* mesh = dynamic_cast<SLCMesh*>(normalIndex->parent);
        MeshRecord& mr       = meshdata[meshIdx-1];
        mr.normalIndexOffset = normalidxdata.size();
        mr.normalIndexLength = normalIndex->data.size();
        for ( int i=0; i<normalIndex->data.size(); i++ )
            normalidxdata.push_back ( NormalIndexRecord ( normalIndex->data[i] + mesh->normalOffset ) );
        ii = normalIndexIdx++;
    }
    case SLC_TEXCOORDINDEX:
    {
        SLCTexCoordIndex* texCoordIndex = dynamic_cast<SLCTexCoordIndex*>(node);
        SLCMesh* mesh = dynamic_cast<SLCMesh*>(texCoordIndex->parent);
        MeshRecord& mr         = meshdata[meshIdx-1];
        mr.texCoordIndexOffset = texcoordidxdata.size();
        mr.texCoordIndexLength = texCoordIndex->data.size();
        for ( int i=0; i<texCoordIndex->data.size(); i++ )
            texcoordidxdata.push_back ( TexCoordIndexRecord ( texCoordIndex->data[i] + mesh->texcoordOffset ) );
        ii = texcoordIndexIdx++;
    }
    case SLC_TRANSFORM:
    {
        SLCTransform* transform = dynamic_cast<SLCTransform*>(node);
        transformdata.push_back ( TransformRecord ( transform->mat ) );
        ii = transformIdx++;
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

void SLCNode2LC::prepareGlobalEntryData ( SLCNode* node )
{
    collectNodeRecord ( node );
    collectChildrenRecords ( node );
}

void SLCNode2LC::fillGlobalObjEntry ( LC& lc )
{
    lc.globalLCEntry->LCLen = globalRecords.size();
    if ( lc.globalLCEntry->LCLen > 0 )
        memcpy ( lc.globalLCEntry->LCRecords, &(globalRecords[0]), sizeof(GlobalLCRecord) * globalRecords.size() );

    lc.groupEntry->LCLen = groupdata.size();
    if ( lc.groupEntry->LCLen > 0 )
        memcpy ( lc.groupEntry->LCRecords, &(groupdata[0]), sizeof(GroupRecord) * groupdata.size() );

    lc.layerEntry->LCLen = layerdata.size();
    if ( lc.layerEntry->LCLen > 0 )
        memcpy ( lc.layerEntry->LCRecords, &(layerdata[0]), sizeof(LayerRecord) * layerdata.size() );

    lc.lineEntry->LCLen = linedata.size();
    if ( lc.lineEntry->LCLen > 0 )
        memcpy ( lc.lineEntry->LCRecords, &(linedata[0]), sizeof(LineRecord) * linedata.size() );

    lc.triangleEntry->LCLen = tridata.size();
    if ( lc.triangleEntry->LCLen > 0 )
        memcpy ( lc.triangleEntry->LCRecords, &(tridata[0]), sizeof(TriangleRecord) * tridata.size() );

    lc.quadEntry->LCLen = quaddata.size();
    if ( lc.quadEntry->LCLen > 0 )
        memcpy ( lc.quadEntry->LCRecords, &(quaddata[0]), sizeof(QuadRecord) * quaddata.size() );

    lc.modelEntry->LCLen = modeldata.size();
    if ( lc.modelEntry->LCLen > 0 )
        memcpy ( lc.modelEntry->LCRecords, &(modeldata[0]), sizeof(ModelRecord) * modeldata.size() );

    lc.appearanceEntry->LCLen = appearancedata.size();
    if ( lc.appearanceEntry->LCLen > 0 )
        memcpy ( lc.appearanceEntry->LCRecords, &(appearancedata[0]), sizeof(AppearanceRecord) * appearancedata.size() );

    lc.materialEntry->LCLen = materialdata.size();
    if ( lc.materialEntry->LCLen > 0 )
        memcpy ( lc.materialEntry->LCRecords, &(materialdata[0]), sizeof(MaterialRecord) * materialdata.size() );

    lc.textureEntry->LCLen = texturedata.size();
    if ( lc.textureEntry->LCLen > 0 )
        memcpy ( lc.textureEntry->LCRecords, &(texturedata[0]), sizeof(TextureRecord) * texturedata.size() );

    lc.meshEntry->LCLen = meshdata.size();
    if ( lc.meshEntry->LCLen > 0 )
        memcpy ( lc.meshEntry->LCRecords, &(meshdata[0]), sizeof(MeshRecord) * meshdata.size() );

    lc.vertexEntry->LCLen = vertexdata.size();
    if ( lc.vertexEntry->LCLen > 0 )
        memcpy ( lc.vertexEntry->LCRecords, &(vertexdata[0]), sizeof(VertexRecord) * vertexdata.size() );

    lc.normalEntry->LCLen = normaldata.size();
    if ( lc.normalEntry->LCLen > 0 )
        memcpy ( lc.normalEntry->LCRecords, &(normaldata[0]), sizeof(NormalRecord) * normaldata.size() );

    lc.texCoordEntry->LCLen = texcoorddata.size();
    if ( lc.texCoordEntry->LCLen > 0 )
        memcpy ( lc.texCoordEntry->LCRecords, &(texcoorddata[0]), sizeof(TexCoordRecord) * texcoorddata.size() );

    lc.vertexIndexEntry->LCLen = vertexidxdata.size();
    if ( lc.vertexIndexEntry->LCLen > 0 )
        memcpy ( lc.vertexIndexEntry->LCRecords, &(vertexidxdata[0]), sizeof(VertexIndexRecord) * vertexidxdata.size() );

    lc.normalIndexEntry->LCLen = normalidxdata.size();
    if ( lc.normalIndexEntry->LCLen > 0 )
        memcpy ( lc.normalIndexEntry->LCRecords, &(normalidxdata[0]), sizeof(NormalIndexRecord) * normalidxdata.size() );

    lc.texCoordIndexEntry->LCLen = texcoordidxdata.size();
    if ( lc.texCoordIndexEntry->LCLen > 0 )
        memcpy ( lc.texCoordIndexEntry->LCRecords, &(texcoordidxdata[0]), sizeof(TexCoordIndexRecord) * texcoordidxdata.size() );

    lc.transformEntry->LCLen = transformdata.size();
    if ( lc.transformEntry->LCLen > 0 )
        memcpy ( lc.transformEntry->LCRecords, &(transformdata[0]), sizeof(TransformRecord) * transformdata.size() );
}
