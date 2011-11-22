#ifndef _SGR_SMARTILES_H_
#define _SGR_SMARTILES_H_

struct Tile
{
    float minmax[4];
    unsigned int id;
    unsigned short level;
    int texid;
    char filename[256];
    Tile ()
    {
        id = 0;
        level = 0;
        texid = -1;
        minmax[0] = minmax[1] = minmax[2] = minmax[3] = 0.0f;
    }
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
const unsigned char* get_image_blobdata ( void* pDB, int level, int id, int* length );

#endif// _SGR_SMARTILES_H_

