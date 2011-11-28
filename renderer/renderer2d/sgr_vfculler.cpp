#include <GL/glew.h>
#include "sgr_vfculler.h"
#include "sgr_nodetypes.h"
#include <sgr_math.h>
#include "kdtree.h"
#include <cstring>
#include <IL/ilut.h>
#include "teximage.h"
#include <sqlite3.h>

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

void free_image_blobdata ( void* data )
{
    free ( data );
}

//================================================================================

//const unsigned char* get_image_blobdata ( sqlite3* pDB, int level, int id, int* length )
int get_image_blobdata ( sqlite3* pDB, int level, int id, unsigned char** data )
{
//    cout << "---------------get_image_blobdata " << pDB << ", " << length << endl;
    int length = 0;
    *data = 0;
    sqlite3_stmt *pStmt;
    char stmt[256];
    sprintf ( stmt, "select b from ttt where level=%d and id=%d;", level, id );
    int rc = sqlite3_prepare ( pDB, stmt, -1, &pStmt, 0);
    if (rc == SQLITE_OK && pStmt != NULL) {
        if ( sqlite3_step(pStmt) == SQLITE_ROW ) {
            if ( sqlite3_column_count ( pStmt ) == 1 ) {
                if ( SQLITE_BLOB==sqlite3_column_type ( pStmt, 0) ) {
                    length = sqlite3_column_bytes(pStmt, 0);
                    *data = (unsigned char*)malloc ( length * sizeof(char) );
                    unsigned char* tmp = (unsigned char*)sqlite3_column_blob(pStmt, 0);
                    memcpy ( *data, tmp, length*sizeof(char) );
//                    sqlite3_column_bytes(pStmt, 0);
                } else {
                    printf ( "error: %s\n", sqlite3_errmsg (pDB ) );
                }
            } else {
                printf ( "error: %s\n", sqlite3_errmsg (pDB ) );
            }
        } else {
            printf ( "error: %s\n", sqlite3_errmsg (pDB ) );
        }
//        cout << 6 << endl;
        rc = sqlite3_finalize(pStmt);
    } else {
        printf ( "error: %s\n", sqlite3_errmsg (pDB ) );
    }
//    cout << 7 << endl;
    return length;
}

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

            cout << "...........tiles cache have " << lc.smartile_textures.size() << " tiles" << endl;
            for ( int i=0; i<smartile.tilecnt; i++ ) {
                Tile* tile = smartile.tiles + i;
                map<string,unsigned int>::iterator pp = lc.smartile_textures.find ( tile->filename );
                if ( pp == lc.smartile_textures.end() ) {
                    int texid = 0;

                    cout << "...... before load texture " << tile->filename << endl;
                    unsigned char* memdata;
                    int len = get_image_blobdata ( (sqlite3*)smartile.pDB, tile->level, tile->id, &memdata );
//                    printf ( "memdata = %x, len=%d\n", memdata, len );
                    if ( memdata != 0 ) {
                        texid = create_texture_from_jpgmem ( memdata, len );
                        cout << "...... create_texture_from_jpgmem finished" << endl;
                    }
                    free_image_blobdata ( memdata );

//                     if ( (texid=ilutGLLoadImage ( const_cast<char*>(tile->filename))) == 0 ) {
                    if ( texid == 0 ) {
                        tile->texid = -1;
                        cout << "---------- load texture " << tile->filename << " failed" << endl;
                    } else {
                        if (GLEW_VERSION_1_3) {
                            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
                            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
                        }
                        tile->texid = texid;
                        lc.smartile_textures.insert ( pair<string,unsigned int>(tile->filename,texid) );
                        cout << "---------- load texture " << tile->filename << " ok. " << tile->texid << endl;
                    }
                } else {
                    tile->texid = pp->second;
                }
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
        cout << "---------- load texture " << endl;
        // check if bbox outside smartiles
        const BBox2d* nodeBBox = (const BBox2d*)(lc.globalLCEntry->LCRecords[gIdx].minmax);
        if ( false == is_inside (vfbbox2d, *nodeBBox) ) {
            return CHILD_CULLED;
        } else {
            // get each tiles
	    SmartTileRecord& smartile = lc.smartTilesEntry->LCRecords[lc.getValue()];
            if ( smartile.pDB == 0 ) {
                cerr << "invalid smartile, dbname = " << smartile.dbname << endl;
                return CHILD_NOT_CULLED;
            }
            smartile.tilecnt = GetTiles ( &smartile, (float*)&vfbbox2d, smartile.tiles );

            for ( int i=0; i<smartile.tilecnt; i++ ) {
                Tile* tile = smartile.tiles + i;
                int texid = 0;
                // get imagedata
                
                unsigned char* jpgfile_mem;
                int memsize = get_image_blobdata ( (sqlite3*)smartile.pDB, tile->level, tile->id, &jpgfile_mem );
                if ( jpgfile_mem != NULL )
                    texid = create_texture_from_jpgmem ( jpgfile_mem, memsize );
                free_image_blobdata ( jpgfile_mem );


                if ( texid == 0 ) {
                    tile->texid = -1;
                    cout << "---------- load texture " << tile->filename << " failed" << endl;
                } else {
                    tile->texid = texid;
                    cout << "---------- load texture " << tile->filename << " ok" << endl;
                }

//                 if ( (texid=ilutGLLoadImage ( const_cast<char*>(tile->filename))) == 0 ) {
//                     tile->texid = -1;
//                     cout << "---------- load texture " << tile->filename << " failed" << endl;
//                 } else {
//                     tile->texid = texid;
//                     cout << "---------- load texture " << tile->filename << " ok" << endl;
//                 }
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
