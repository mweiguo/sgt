#include "sgr_smartiles.h"

#include <math.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

SmartTiles* curSmartTiles;

void InitSmartTiles ()
{
    curSmartTiles = (SmartTiles*)calloc (1,sizeof(SmartTiles));
}

void SetupTiles ( int levelcnt, double size )
{
    curSmartTiles->levelcount = levelcnt;
    curSmartTiles->minmax[0] = 0;
    curSmartTiles->minmax[1] = 0;
    curSmartTiles->minmax[2] = size;
    curSmartTiles->minmax[3] = size;
}

int GetTiles ( double* minmax, Tile* tiles )
{
    // get level
    double w = minmax[2] - minmax[0];
    double h = minmax[3] - minmax[1];
    double size = w > h ? w : h;
    double gsize = curSmartTiles->minmax[2] - curSmartTiles->minmax[0];
    int level = 0;
    if ( size >= gsize )
        level = 0;
    else {
        double t = gsize / size;
        while ( t >= 1  ) {
            t /= 2.0;
            level ++;
        }
    }
    if ( level >= curSmartTiles->levelcount )
        level = curSmartTiles->levelcount-1;

    int xtilecnt = pow(2,level);
    double delta = curSmartTiles->minmax[2] / xtilecnt;
/*     cout << "--------level     = " << level    << endl; */
/*     cout << "--------deltasize = " << delta    << endl; */
/*     cout << "--------xtilecnt  = " << xtilecnt << endl; */

    Tile* bak = tiles;

    // get tiles
    int xidx_min = (int)(minmax[0] / delta);
    int yidx_min = (int)(minmax[1] / delta);
    int xidx_max = (int)(minmax[2] / delta - 0.0001);
    int yidx_max = (int)(minmax[3] / delta - 0.0001);
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
            tiles++;
        }
    }

    return (tiles - bak);
}

