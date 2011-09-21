#include "sgr_vfculler.h"
#include "sgr_nodetypes.h"
#include <sgr_math.h>
#include "kdtree.h"
#include <cstring>

list<int> vfculler::renderObjects;
BBox2d vfculler::vfbbox2d;

//================================================================================

void vfculler::cull ( LC& lc, const BBox2d& vfbbox )
{
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
    case SLC_SCENE:
    case SLC_MATERIAL:
    case SLC_LAYER:
    case SLC_LOD:
    {
        const BBox2d* nodeBBox = (const BBox2d*)(lc.globalLCEntry->LCRecords[gIdx].minmax);
        if ( is_outside (*nodeBBox, vfbbox2d ) )
            return CHILD_CULLED;
        else
            return CHILD_NOT_CULLED;
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
        if ( is_outside (*nodeBBox, vfbbox2d ) )
            return CHILD_CULLED;
        else
        {
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

