#ifndef _SGR_SMARTILES_H_
#define _SGR_SMARTILES_H_

struct Tile
{
    float minmax[4];
    unsigned int id;
    unsigned short level;
    int texid;
    char filename[256];
};

struct SmartTileRecord;
/*
struct SmartTiles
{
    int levelcount;
    double minmax[4];
};

void InitSmartTiles ();
void SetupTiles ( int levelcnt, float size );
*/
int GetTiles ( SmartTileRecord* curSmartTiles, float* minmax, Tile* tiles );

#endif// _SGR_SMARTILES_H_

