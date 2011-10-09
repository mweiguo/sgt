#include "sgr_vfculler.h"
#include "sgr_nodetypes.h"
#include <sgr_math.h>
#include "kdtree.h"
#include <cstring>

list<int> vfculler::renderObjects;
BBox2d vfculler::vfbbox2d;
float vfculler::scale = 1;

//================================================================================

void vfculler::cull ( LC& lc, const BBox2d& vfbbox, float s )
{
    scale = s;
    vfbbox2d = vfbbox;
    
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
	    cout << "++++++++++++++++ in lod " << endl;
	    for ( int i=0; i<lod.scalecnt; i++ ) {
		if ( scale < lod.scales[i] ) {
		    traverse ( lc, i );
		    return CHILD_CULLED;
		}
	    }
	    traverse ( lc, lod.scalecnt-1 );
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
                copy ( out.begin(), out.end(), back_inserter(renderObjects) );
//		cout << "intersect_cnt : " << kdt->intersect_cnt << endl;
                return CHILDREN_CULLTEST_FINISHED;
            }
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

