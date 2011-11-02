#ifndef _SGR_SMARTILES_H_
#define _SGR_SMARTILES_H_

struct Tile
{
    double minmax[4];
    unsigned int id;
    unsigned short level;
    int texid;
};

struct SmartTiles
{
    int levelcount;
    double minmax[4];
};

void InitSmartTiles ();
void SetupTiles ( int levelcnt, double size );
int GetTiles ( double* minmax, Tile* tiles );

#endif// _SGR_SMARTILES_H_

