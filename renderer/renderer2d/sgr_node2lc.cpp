#include "sgr_node2lc.h"
#include "sgr_nodes.h"
#include "sgr_lc2kdtree.h"
#include "sgr_bboxupdater.h"
#include "sgr_font.h"
#include "sgr_triangulate.h"
#include "mat4.h"

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
    plineIdx = 0;
    polyIdx = 0;
    textIdx = 0;
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
        scenedata.push_back ( SceneRecord(group->name.c_str()) );
        ii = sceneIdx++; break;
    }
    case SLC_MATERIAL:
    {
        SLCMaterial* material = dynamic_cast<SLCMaterial*>(node);
        materialMap.insert ( pair<string,int>(material->name, materialdata.size() ));
        materialdata.push_back ( MaterialRecord ( material->name.c_str(), material->background_color, material->foreground_color, material->linewidth, material->linetype, material->fontfilename.c_str(), material->texturefilename.c_str() ) );
        ii = matIdx++; break;
    }
    case SLC_LAYER:
    {
        SLCLayerNode* layer = dynamic_cast<SLCLayerNode*>(node);
        map<string, int>::iterator pp = materialMap.find ( layer->bindmat->name );
        if ( pp!=materialMap.end() )
            layerdata.push_back ( LayerRecord(layer->name.c_str(), layer->flags, pp->second ) );
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
        map<string, int>::iterator pp = materialMap.find ( rc->bindmat->name );
        if ( pp!=materialMap.end() )
	{
	    if ( rc->filltexture )
		quaddata.push_back ( RectRecord(rc->pnts[0], rc->pnts[1], rc->pnts[2], rc->pnts[3], true, rc->textureAngle, rc->textureScale, pp->second ) );
	    else
		quaddata.push_back ( RectRecord(rc->pnts[0], rc->pnts[1], rc->pnts[2], rc->pnts[3], false, rc->textureAngle, rc->textureScale, pp->second ) );
	}
        ii = rectIdx++; break;
    }
    case SLC_PLINE:
    {
        SLCPLineNode* pline = dynamic_cast<SLCPLineNode*>(node);
        map<string, int>::iterator pp = materialMap.find ( pline->bindmat->name );
        if ( pp!=materialMap.end() ) {
	    size_t istart = plinebuffer.size();
	    copy ( pline->pnts.begin(), pline->pnts.end(), back_inserter(plinebuffer) );
	    plinedata.push_back ( PLineRecord(istart, pline->pnts.size()+istart, pp->second) );
	}
        ii = plineIdx++; break;
    }
    case SLC_POLY:
    {
        SLCPolyNode* poly = dynamic_cast<SLCPolyNode*>(node);
        map<string, int>::iterator pp = materialMap.find ( poly->bindmat->name );
        if ( pp!=materialMap.end() ) {
	    size_t istart = plinebuffer.size();
	    copy ( poly->pnts.begin(), poly->pnts.end(), back_inserter(plinebuffer) );
	    // calculate tessellation
	    int tessStart = polytessellationbuffer.size();
	    Triangulate::Process ( poly->pnts, back_inserter(polytessellationbuffer) );
	    int tessEnd = polytessellationbuffer.size();
	    if ( poly->filltexture )
	    {
		double topi = M_PI / 180;
		// calculate texture coordinate
		float cosalpha = cos ( topi * poly->textureAngle );
		float sinalpha = sin ( topi * poly->textureAngle );
		int texcoordStart = texturecoordbuffer.size();
		for ( int i=tessStart; i!=tessEnd; i++ )
		{
		    vec2f& t = polytessellationbuffer[i];
		    float x = t.x() * cosalpha - t.y() * sinalpha;
		    float y = t.x() * sinalpha + t.y() * cosalpha;
		    vec2f texcoord;
		    texcoord.x ( x / (200 * poly->textureScale ) );
		    texcoord.y ( y / (200 * poly->textureScale ) );
		    texturecoordbuffer.push_back ( texcoord );
		}
		int texcoordEnd = texturecoordbuffer.size();
		polydata.push_back ( PolyRecord(istart, poly->pnts.size()+istart, texcoordStart, texcoordEnd, tessStart, tessEnd, true, poly->textureAngle, poly->textureScale, pp->second) );
	    }
	    else
		polydata.push_back ( PolyRecord(istart, poly->pnts.size()+istart, -1, -1, tessStart, tessEnd, false, 0, 1, pp->second) );
	}
        ii = polyIdx++; break;
    }
    case SLC_TEXT:
    {
        SLCTextNode* tc = dynamic_cast<SLCTextNode*>(node);
        map<string, int>::iterator pp = materialMap.find ( tc->bindmat->name );
        if ( pp!=materialMap.end() ) {
	    size_t istart = textbuffer.size();
	    copy ( tc->text.begin(), tc->text.end(), back_inserter(textbuffer) );
	    textbuffer.push_back ( 0 );
	    int sstart = textsilhouettebuffer.size();
	    textsilhouettebuffer.resize ( textsilhouettebuffer.size() + 4 );
	    int send = textsilhouettebuffer.size();
	    textdata.push_back ( TextRecord(istart, tc->pos, tc->scale, tc->rotz, sstart, send, pp->second ) );
	}
	
        ii = textIdx++; break;
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
    lc->globalLCEntry 	 = (GlobalLCEntry*)    malloc( sizeof(int) + ( globalRecords.empty() ? sizeof(GlobalLCRecord) : sizeof(GlobalLCRecord) * globalRecords.size() ));
    lc->sceneEntry    	 = (SceneEntry*)       malloc( sizeof(int) + ( scenedata.empty() ? sizeof(SceneRecord) : sizeof(SceneRecord) * scenedata.size() ));
    lc->materialEntry 	 = (MaterialEntry*)    malloc( sizeof(int) + ( materialdata.empty() ? sizeof(MaterialRecord) : sizeof(MaterialRecord) * materialdata.size() ));
    lc->layerEntry    	 = (LayerEntry*)       malloc( sizeof(int) + ( layerdata.empty() ? sizeof(LayerRecord) : sizeof(LayerRecord) * layerdata.size() ));
    lc->lodEntry      	 = (LODEntry*)         malloc( sizeof(int) + ( loddata.empty() ? sizeof(LODRecord) : sizeof(LODRecord) * loddata.size() ));
    lc->lodpageEntry  	 = (LODPageEntry*)     malloc( sizeof(int) + ( lodpagedata.empty() ? sizeof(LODPageRecord) : sizeof(LODPageRecord) * lodpagedata.size() ));
    lc->lineEntry     	 = (LineEntry*)        malloc( sizeof(int) + ( linedata.empty() ? sizeof(LineRecord) : sizeof(LineRecord) * linedata.size() ));
    lc->triangleEntry 	 = (TriangleEntry*)    malloc( sizeof(int) + ( tridata.empty() ? sizeof(TriangleRecord) : sizeof(TriangleRecord) * tridata.size() ));
    lc->rectEntry     	 = (RectEntry*)        malloc( sizeof(int) + ( quaddata.empty() ? sizeof(RectRecord) : sizeof(RectRecord) * quaddata.size() ));
    lc->plineEntry     	 = (PLineEntry*)       malloc( sizeof(int) + ( plinedata.empty() ? sizeof(PLineRecord) : sizeof(PLineRecord) * plinedata.size() ));
    lc->polyEntry     	 = (PolyEntry*)        malloc( sizeof(int) + ( polydata.empty() ? sizeof(PolyRecord) : sizeof(PolyRecord) * polydata.size() ));
    lc->textEntry     	 = (TextEntry*)        malloc( sizeof(int) + ( textdata.empty() ? sizeof(TextRecord) : sizeof(TextRecord) * textdata.size() ));
    lc->textBufferEntry  = (TextBufferEntry*)  malloc( sizeof(int) + ( textbuffer.empty() ? sizeof(char) : sizeof(char) * textbuffer.size() ));
    lc->textSilhouetteBufferEntry  = (TextSilhouetteBufferEntry*)  malloc( sizeof(int) + ( textsilhouettebuffer.empty() ? sizeof(vec2f) : sizeof(vec2f) * textsilhouettebuffer.size() ));
    lc->plineBufferEntry = (PLineBufferEntry*) malloc( sizeof(int) + ( plinebuffer.empty() ? sizeof(vec2f) : sizeof(vec2f) * plinebuffer.size() ));
    lc->polyTessellationBufferEntry = (PolyTessellationBufferEntry*) malloc( sizeof(int) + ( polytessellationbuffer.empty() ? sizeof(vec2f) : sizeof(vec2f) * polytessellationbuffer.size() ));
    lc->texCoordBufferEntry = (TexCoordBufferEntry*) malloc( sizeof(int) + ( texturecoordbuffer.empty() ? sizeof(vec2f) : sizeof(vec2f) * texturecoordbuffer.size() ));

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

    // fonts
    for ( int ii=0; ii<lc->materialEntry->LCLen; ii++ )
    {
	Font* ft = 0;
	MaterialRecord& mr = lc->materialEntry->LCRecords[ii];
	for ( size_t jj=0; jj<lc->fonts.size(); jj++ )
	{
	    if ( lc->fonts[jj]->fontfilename == mr.fontfile ) {
		ft = lc->fonts[jj];
		mr.fontIdx = jj;
		break;
	    }
	}

	if ( NULL == ft ) {
	    ft = new Font ( mr.fontfile );
	    mr.fontIdx = lc->fonts.size();
	    lc->fonts.push_back ( ft );
	}
    }


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

    lc->plineEntry->LCLen = plinedata.size();
    if ( lc->plineEntry->LCLen > 0 )
        memcpy ( lc->plineEntry->LCRecords, &(plinedata[0]), sizeof(PLineRecord) * plinedata.size() );

    lc->polyEntry->LCLen = polydata.size();
    if ( lc->polyEntry->LCLen > 0 )
        memcpy ( lc->polyEntry->LCRecords, &(polydata[0]), sizeof(PolyRecord) * polydata.size() );

    lc->textEntry->LCLen = textdata.size();
    if ( lc->textEntry->LCLen > 0 )
        memcpy ( lc->textEntry->LCRecords, &(textdata[0]), sizeof(TextRecord) * textdata.size() );

    lc->textBufferEntry->LCLen = textbuffer.size();
    if ( lc->textBufferEntry->LCLen > 0 )
        memcpy ( lc->textBufferEntry->LCRecords, &(textbuffer[0]), sizeof(char) * textbuffer.size() );

    lc->textSilhouetteBufferEntry->LCLen = textsilhouettebuffer.size();
    if ( lc->textSilhouetteBufferEntry->LCLen > 0 )
        memcpy ( lc->textSilhouetteBufferEntry->LCRecords, &(textsilhouettebuffer[0]), sizeof(vec2f) * textsilhouettebuffer.size() );

    lc->plineBufferEntry->LCLen = plinebuffer.size();
    if ( lc->plineBufferEntry->LCLen > 0 )
        memcpy ( lc->plineBufferEntry->LCRecords, &(plinebuffer[0]), sizeof(vec2f) * plinebuffer.size() );

    lc->polyTessellationBufferEntry->LCLen = polytessellationbuffer.size();
    if ( lc->polyTessellationBufferEntry->LCLen > 0 )
        memcpy ( lc->polyTessellationBufferEntry->LCRecords, &(polytessellationbuffer[0]), sizeof(vec2f) * polytessellationbuffer.size() );

    lc->texCoordBufferEntry->LCLen = texturecoordbuffer.size();
    if ( lc->texCoordBufferEntry->LCLen > 0 )
        memcpy ( lc->texCoordBufferEntry->LCRecords, &(texturecoordbuffer[0]), sizeof(vec2f) * texturecoordbuffer.size() );

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
    free ( lc->plineEntry );
    free ( lc->polyEntry );
    free ( lc->textEntry );
    free ( lc->textBufferEntry );
    free ( lc->textSilhouetteBufferEntry );
    free ( lc->plineBufferEntry );
    free ( lc->polyTessellationBufferEntry );
    free ( lc->texCoordBufferEntry );

    lc->freeLevelLC ();
    delete lc;
}

// #include "sgr_lcreport.h"

void SLCNode2LC::convert ( const char* filename )
{
    LC* lc = generateLC();
    // update bbox
    BBox2dUpdater::forward_update ( *lc );

    lc->save ( filename );
//     LCReport rpt ( *lc, 1 );
//     rpt.printCounter ();

    // save kdt, if there have
    LC2KDT::buildKDTs ( *lc );
    
    freeLC ( lc );
}
