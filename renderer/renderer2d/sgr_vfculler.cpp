#include "sgr_vfculler.h"
#include "sgr_nodetypes.h"
#include <sgr_math.h>
#include "kdtree.h"
#include <cstring>
#include <IL/ilut.h>

vector<int> vfculler::renderObjects;
BBox2d vfculler::vfbbox2d;
float vfculler::scale = 1;
bool vfculler::doPick = false;
extern LoadTex_Proc loadtex_proc;

//================================================================================

void vfculler::cull ( LC& lc, const BBox2d& vfbbox, float s, bool dopick )
{
    doPick = dopick;
    scale = s;
    vfbbox2d = vfbbox;
//    cout << "(" << vfbbox2d.minvec().x() << "," << vfbbox2d.minvec().y() << "), (" << vfbbox2d.maxvec().x() << "," << vfbbox2d.maxvec().y() << ") " << endl;

    if ( cull_test ( lc.getType(), lc.getGIndex(), lc )==CHILD_NOT_CULLED ) {
        traverse ( lc );
    }
}

//================================================================================

void vfculler::clear ()
{
    renderObjects.clear();
}

//================================================================================

/**
 * return true if object culled, else return false
 */
int vfculler::cull_test ( int type, int gIdx, LC& lc )
{
    stringstream ss;
    switch ( type )
    {
    case SLC_LAYER:
    {
	GlobalLCRecord& grcd = lc.globalLCEntry->LCRecords[gIdx];
	LayerRecord& layer = lc.layerEntry->LCRecords[grcd.value];

        if ( doPick && (false==layer.pickable) )
            return CHILD_CULLED;

	if ( layer.visible ) 
	{
	    const BBox2d* nodeBBox = (const BBox2d*)grcd.minmax;
	    if ( is_outside (*nodeBBox, vfbbox2d ) ) {
		return CHILD_CULLED;
	    } else {
		return CHILD_NOT_CULLED;
	    }
	}
	else
            return CHILD_CULLED;
	
    }
    case SLC_SCENE:
    case SLC_MATERIAL:
    {
        const BBox2d* nodeBBox = (const BBox2d*)(lc.globalLCEntry->LCRecords[gIdx].minmax);
        if ( is_outside (*nodeBBox, vfbbox2d ) )
            return CHILD_CULLED;
        else
            return CHILD_NOT_CULLED;
    }
    case SLC_LOD:
    {
        const BBox2d* nodeBBox = (const BBox2d*)(lc.globalLCEntry->LCRecords[gIdx].minmax);
        if ( is_outside (*nodeBBox, vfbbox2d ) )
            return CHILD_CULLED;
        else
	{
	    LODRecord& lod = lc.lodEntry->LCRecords[lc.getValue()];
	    if ( lod.scalecnt == 0 )
		return CHILD_NOT_CULLED;

	    // skip to correspond node
	    // current Scale;
	    for ( int i=0; i<lod.scalecnt; i++ ) {
		if ( scale < lod.scales[i] ) {
		    traverse ( lc, i );
		    return CHILD_CULLED;
		}
	    }
	    traverse ( lc, lod.scalecnt );
            return CHILD_CULLED;
	}
    }
    case SLC_PLINE:
    case SLC_POLY:
    case SLC_LINE:
    case SLC_TRIANGLE:
    case SLC_RECT:
    case SLC_TEXT:
    {
        const BBox2d* nodeBBox = (const BBox2d*)(lc.globalLCEntry->LCRecords[gIdx].minmax);
        if ( is_outside (*nodeBBox, vfbbox2d ) ) {
            return CHILD_CULLED;
        } else {
            renderObjects.push_back ( gIdx );
            return CHILD_NOT_CULLED;
        }
    }
    case SLC_LODPAGE:
    {
        const BBox2d* nodeBBox = (const BBox2d*)(lc.globalLCEntry->LCRecords[gIdx].minmax);
        if ( is_outside (*nodeBBox, vfbbox2d ) ) {
            return CHILD_CULLED;
        } else {
            int objIdx = lc.globalLCEntry->LCRecords[gIdx].value;
            int kdtreeidx = lc.lodpageEntry->LCRecords[objIdx].kdtree;
            if ( -1 == kdtreeidx )
                return CHILD_NOT_CULLED;
            else
            {
                KdTree<int>* kdt = lc.kdtrees[kdtreeidx];
                vector<int> out;
//		kdt->intersect_cnt = 0;
                bool isHit = kdt->intersect ( &lc, (float*)&vfbbox2d, back_inserter(out) );
//                cout << "isHit = " << isHit << ", size = " << out.size() << endl;
                if ( isHit )
                    copy ( out.begin(), out.end(), back_inserter(renderObjects) );
//		cout << "intersect_cnt : " << kdt->intersect_cnt << endl;
                return CHILDREN_CULLTEST_FINISHED;
            }
        }
    }
    case SLC_SMARTILES:
    {
        // check if bbox outside smartiles
        const BBox2d* nodeBBox = (const BBox2d*)(lc.globalLCEntry->LCRecords[gIdx].minmax);
        if ( is_outside (*nodeBBox, vfbbox2d ) ) {
            return CHILD_CULLED;
        } else {
            // get each tiles
	    SmartTileRecord& smartile = lc.smartTilesEntry->LCRecords[lc.getValue()];
            smartile.tilecnt = GetTiles ( &smartile, (float*)&vfbbox2d, smartile.tiles );

            for ( int i=0; i<smartile.tilecnt; i++ ) {
                Tile* tile = smartile.tiles + i;
                int texid;
                if ( (texid=ilutGLLoadImage ( const_cast<char*>(tile->filename))) == 0 )
                    tile->texid = -1;
                else
                    tile->texid = texid;
            }
            renderObjects.push_back ( gIdx );
            return CHILD_NOT_CULLED;
        }
    }
    default:
        ss << "unsupport type " << type;
        break;
    }
    return NONE;
}

//================================================================================

void vfculler::traverse ( LC& lc )
{
    if ( lc.toElement ( FIRST_CHILD ) >= 0 ) {
        if ( cull_test ( lc.getType(), lc.getGIndex(), lc )==CHILD_NOT_CULLED )
            traverse ( lc );
        while ( lc.toElement ( NEXT_SIBLING )>=0 ) {
            if ( cull_test ( lc.getType(), lc.getGIndex(), lc )==CHILD_NOT_CULLED )
                traverse ( lc );
        }
        lc.toElement ( PARENT );
    }
}

//================================================================================
/** for LOD node
 */
void vfculler::traverse ( LC& lc, int i )
{
    if ( i<0 )
	return;

    int ii=0;
    if ( lc.toElement ( FIRST_CHILD ) >= 0 ) {
	if ( i == ii++ ) {
	    if ( cull_test ( lc.getType(), lc.getGIndex(), lc )==CHILD_NOT_CULLED )
		traverse ( lc );
	}
        while ( lc.toElement ( NEXT_SIBLING )>=0 ) {
	    if ( i == ii++ ) {
		if ( cull_test ( lc.getType(), lc.getGIndex(), lc )==CHILD_NOT_CULLED )
		    traverse ( lc );
	    }
        }
        lc.toElement ( PARENT );
    }

    if ( i>=ii )
	return;
}

//================================================================================

void vfculler::cull2 ( LC& lc, const BBox2d& vfbbox, float s, bool dopick )
{
    doPick = dopick;
    scale = s;
    vfbbox2d = vfbbox;
    
    if ( cull_test2 ( lc.getType(), lc.getGIndex(), lc )==CHILD_NOT_CULLED ) {
        traverse2 ( lc );
    }
}

//================================================================================

/**
 * return true if object culled, else return false
 */
int vfculler::cull_test2 ( int type, int gIdx, LC& lc )
{
    stringstream ss;
    switch ( type )
    {
    case SLC_LAYER:
    {
	GlobalLCRecord& grcd = lc.globalLCEntry->LCRecords[gIdx];
	LayerRecord& layer = lc.layerEntry->LCRecords[grcd.value];
	if ( layer.visible )
	{
	    const BBox2d* nodeBBox = (const BBox2d*)grcd.minmax;
	    if ( true == is_outside ( vfbbox2d, *nodeBBox ) ) {
		return CHILD_CULLED;
	    } else {
		return CHILD_NOT_CULLED;
	    }
	}
	else
            return CHILD_CULLED;
	
    }
    case SLC_SCENE:
    case SLC_MATERIAL:
    {
        const BBox2d* nodeBBox = (const BBox2d*)(lc.globalLCEntry->LCRecords[gIdx].minmax);
        if ( true == is_outside (vfbbox2d, *nodeBBox ) ) {
            return CHILD_CULLED;
        } else {
            return CHILD_NOT_CULLED;
        }
    }
    case SLC_LOD:
    {
        const BBox2d* nodeBBox = (const BBox2d*)(lc.globalLCEntry->LCRecords[gIdx].minmax);
        if ( true == is_outside (vfbbox2d, *nodeBBox ) )
            return CHILD_CULLED;
        else
	{
	    LODRecord& lod = lc.lodEntry->LCRecords[lc.getValue()];
	    if ( lod.scalecnt == 0 )
		return CHILD_NOT_CULLED;

	    // skip to correspond node
	    // current Scale;
	    for ( int i=0; i<lod.scalecnt; i++ ) {
		if ( scale < lod.scales[i] ) {
		    traverse2 ( lc, i );
		    return CHILD_CULLED;
		}
	    }
	    traverse2 ( lc, lod.scalecnt );
            return CHILD_CULLED;
	}
    }
    case SLC_PLINE:
    case SLC_POLY:
    case SLC_LINE:
    case SLC_TRIANGLE:
    case SLC_RECT:
    case SLC_TEXT:
    {
        const BBox2d* nodeBBox = (const BBox2d*)(lc.globalLCEntry->LCRecords[gIdx].minmax);
        if ( false == is_inside (vfbbox2d, *nodeBBox) ) {
            return CHILD_CULLED;
        } else {
            renderObjects.push_back ( gIdx );
            return CHILD_NOT_CULLED;
        }
    }
    case SLC_LODPAGE:
    {
        const BBox2d* nodeBBox = (const BBox2d*)(lc.globalLCEntry->LCRecords[gIdx].minmax);
        if ( true == is_outside (vfbbox2d, *nodeBBox) ) {
            return CHILD_CULLED;
        } else {
            int objIdx = lc.globalLCEntry->LCRecords[gIdx].value;
            int kdtreeidx = lc.lodpageEntry->LCRecords[objIdx].kdtree;
            if ( -1 == kdtreeidx )
                return CHILD_NOT_CULLED;
            else
            {
                KdTree<int>* kdt = lc.kdtrees[kdtreeidx];
                vector<int> out;
//		kdt->intersect_cnt = 0;
                bool isHit = kdt->contain ( &lc, (float*)&vfbbox2d, back_inserter(out) );
                if ( isHit )
                    copy ( out.begin(), out.end(), back_inserter(renderObjects) );
//		cout << "intersect_cnt : " << kdt->intersect_cnt << endl;
                return CHILDREN_CULLTEST_FINISHED;
            }
        }
    }
    case SLC_SMARTILES:
    {
        // check if bbox outside smartiles
        const BBox2d* nodeBBox = (const BBox2d*)(lc.globalLCEntry->LCRecords[gIdx].minmax);
        if ( false == is_inside (vfbbox2d, *nodeBBox) ) {
            return CHILD_CULLED;
        } else {
            // get each tiles
	    SmartTileRecord& smartile = lc.smartTilesEntry->LCRecords[lc.getValue()];
            smartile.tilecnt = GetTiles ( &smartile, (float*)&vfbbox2d, smartile.tiles );

            for ( int i=0; i<smartile.tilecnt; i++ ) {
                Tile* tile = smartile.tiles + i;
                int texid;
                if ( (texid=ilutGLLoadImage ( const_cast<char*>(tile->filename))) == 0 )
                    tile->texid = -1;
                else
                    tile->texid = texid;
            }
            renderObjects.push_back ( gIdx );
            return CHILD_NOT_CULLED;
        }
    }
    default:
        ss << "unsupport type " << type;
        break;
    }
    return NONE;
}

//================================================================================

void vfculler::traverse2 ( LC& lc )
{
    if ( lc.toElement ( FIRST_CHILD ) >= 0 ) {
        if ( cull_test2 ( lc.getType(), lc.getGIndex(), lc )==CHILD_NOT_CULLED )
            traverse2 ( lc );
        while ( lc.toElement ( NEXT_SIBLING )>=0 ) {
            if ( cull_test2 ( lc.getType(), lc.getGIndex(), lc )==CHILD_NOT_CULLED )
                traverse2 ( lc );
        }
        lc.toElement ( PARENT );
    }
}

//================================================================================
/** for LOD node
 */
void vfculler::traverse2 ( LC& lc, int i )
{
    if ( i<0 )
	return;

    int ii=0;
    if ( lc.toElement ( FIRST_CHILD ) >= 0 ) {
	if ( i == ii++ ) {
	    if ( cull_test2 ( lc.getType(), lc.getGIndex(), lc )==CHILD_NOT_CULLED )
		traverse2 ( lc );
	}
        while ( lc.toElement ( NEXT_SIBLING )>=0 ) {
	    if ( i == ii++ ) {
		if ( cull_test2 ( lc.getType(), lc.getGIndex(), lc )==CHILD_NOT_CULLED )
		    traverse2 ( lc );
	    }
        }
        lc.toElement ( PARENT );
    }

    if ( i>=ii )
	return;
}
