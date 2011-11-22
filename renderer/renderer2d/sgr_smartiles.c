#include "sgr_smartiles.h"
#include "sgr_lc.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <sstream>
using namespace std;

#include <sqlite3.h>
#include <math.h>
//SmartTiles* curSmartTiles;

/*
void InitSmartTiles ()
{
    curSmartTiles = (SmartTiles*)calloc (1,sizeof(SmartTiles));
}

void SetupTiles ( int levelcnt, float size )
{
    curSmartTiles->levelcount = levelcnt;
    curSmartTiles->minmax[0] = 0;
    curSmartTiles->minmax[1] = 0;
    curSmartTiles->minmax[2] = size;
    curSmartTiles->minmax[3] = size;
}
*/

int GetTiles ( SmartTileRecord* curSmartTiles, float* minmax, Tile* tiles )
{
    // normalize minmax
/*     cout << "------minmax[3] = " << minmax[3] << endl; */
/*     cout << "------curSmartTiles->data[2].y() = " << curSmartTiles->data[2].y() << endl; */
    minmax[0] = minmax[0] > curSmartTiles->data[0].x() ? minmax[0] : curSmartTiles->data[0].x();
    minmax[1] = minmax[1] > curSmartTiles->data[0].y() ? minmax[1] : curSmartTiles->data[0].y();
    minmax[2] = minmax[2] < curSmartTiles->data[2].x() ? minmax[2] : curSmartTiles->data[2].x();
    minmax[3] = minmax[3] < curSmartTiles->data[2].y() ? minmax[3] : curSmartTiles->data[2].y();
    // get level
    float w = minmax[2] - minmax[0];
    float h = minmax[3] - minmax[1];
    float size = w < h ? w : h;
    float gsize = curSmartTiles->data[1].x() - curSmartTiles->data[0].x();
    int level = 0;
    if ( size >= gsize ) {
        tiles->id        = 0;
        tiles->level     = 0;
        tiles->minmax[0] = curSmartTiles->data[0].x();
        tiles->minmax[1] = curSmartTiles->data[0].y();
        tiles->minmax[2] = tiles->minmax[0] + gsize;
        tiles->minmax[3] = tiles->minmax[1] + gsize;
//        tiles->texid     = -1;

        stringstream ss;
        ss << level << "/0/0/0/0.png";
        strcpy ( tiles->filename, ss.str().c_str() );
        return 1;
    } else {
        float t = gsize / size;
        while ( t >= 1  ) {
            t /= 2.0;
            level ++;
        }
    }
    if ( level >= curSmartTiles->levelcnt )
        level = curSmartTiles->levelcnt-1;

    int xtilecnt = pow(2,level);
    float delta = ( curSmartTiles->data[1].x() - curSmartTiles->data[0].x()) / xtilecnt;
/*     cout << "--------level     = " << level    << endl; */
/*     cout << "--------deltasize = " << delta    << endl; */
/*     cout << "--------xtilecnt  = " << xtilecnt << endl; */

    Tile* bak = tiles;

    // get tiles
    int xidx_min = (int)(minmax[0] / delta);
    int yidx_min = (int)(minmax[1] / delta);
    int xidx_max = (int)(minmax[2] / delta - 0.0001);
    int yidx_max = (int)(minmax[3] / delta - 0.0001);
/*     cout << "--------minmax[0] = " << minmax[0] << endl; */
/*     cout << "--------minmax[1] = " << minmax[1] << endl; */
/*     cout << "--------minmax[2] = " << minmax[2] << endl; */
/*     cout << "--------minmax[3] = " << minmax[3] << endl; */
/*     cout << "--------xidx_min = " << xidx_min << endl; */
/*     cout << "--------yidx_min = " << yidx_min << endl; */
/*     cout << "--------xidx_max = " << xidx_max << endl; */
/*     cout << "--------yidx_max = " << yidx_max << endl; */

    // output to tiles
    for ( int j=yidx_min; j<=yidx_max; j++ )
    {
        for ( int i=xidx_min; i<=xidx_max; i++ )
        {
            tiles->id        = j * xtilecnt + i;
            tiles->level     = level;
            tiles->minmax[0] = i * delta;
            tiles->minmax[1] = j * delta;
            tiles->minmax[2] = tiles->minmax[0] + delta;
            tiles->minmax[3] = tiles->minmax[1] + delta;
//            tiles->texid     = -1;

            stringstream ss;
            ss << level << "/" << i/1024 << "/" << i%1024 << "/" << (xtilecnt-j-1)/1024 << "/" << (xtilecnt-j-1)%1024 << ".png";
            strcpy ( tiles->filename, ss.str().c_str() );

            tiles++;
        }
    }

    return (tiles - bak);
}

const unsigned char* get_image_blobdata ( void* db, int level, int id, int* length )
{
    sqlite3* pDB = (sqlite3*)db;
    const unsigned char* rtn = 0;
    *length = 0;
    sqlite3_stmt *pStmt;
    char stmt[256];
    sprintf ( stmt, "select b from ttt where level=%d and id=%d;", level, id );
    int rc = sqlite3_prepare ( pDB, stmt, -1, &pStmt, 0);
    if (rc == SQLITE_OK && pStmt != NULL) {
        if ( sqlite3_step(pStmt) == SQLITE_ROW ) {
            if ( sqlite3_column_count ( pStmt ) == 1 ) {
                if ( SQLITE_BLOB==sqlite3_column_type ( pStmt, 0) ) {
                    rtn = (const unsigned char*)sqlite3_column_blob(pStmt, 0);
                    *length = sqlite3_column_bytes(pStmt, 0);
/*
                    FILE* fp;
                    fp = fopen ( "test.jpg", "wb" );
                    fwrite ( rtn, length, 1, fp );
                    fclose ( fp );
*/
                }
            }
        }
        rc = sqlite3_finalize(pStmt);
    }
    return rtn;
}

