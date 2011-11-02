#include "gtest/gtest.h"

#include <sgr_smartiles.h>

#include <iostream>


// ********************************************************************************

class SmartTilesTestCase : public ::testing::Test
{
protected:
    SmartTilesTestCase ()
    {
        InitSmartTiles ();
        SetupTiles ( 5, 25600 );
    }
    ~SmartTilesTestCase ()
    {
    }
};

// ================================================================================

TEST_F ( SmartTilesTestCase, GetTiles_level0 ) {
    double minmax[4] = { 0, 0, 25600, 25600 };
    Tile tiles[256];
    int tilecnt = GetTiles ( minmax, tiles );

    EXPECT_EQ ( 1,     tilecnt );
    EXPECT_EQ ( 0u,    tiles[0].level );
    EXPECT_EQ ( 0u,    tiles[0].id );

    EXPECT_DOUBLE_EQ ( 0,     tiles[0].minmax[0] );
    EXPECT_DOUBLE_EQ ( 0,     tiles[0].minmax[1] );
    EXPECT_DOUBLE_EQ ( 25600, tiles[0].minmax[2] );
    EXPECT_DOUBLE_EQ ( 25600, tiles[0].minmax[3] );
}

// ================================================================================

TEST_F ( SmartTilesTestCase, GetTiles_level1 ) {
    double minmax[4] = { 0, 0, 25000, 25000 };
    Tile tiles[256];
    int tilecnt = GetTiles ( minmax, tiles );

    EXPECT_EQ ( 4,     tilecnt );
    EXPECT_EQ ( 1u,    tiles[0].level );
    EXPECT_EQ ( 0u,    tiles[0].id );
    EXPECT_EQ ( 1u,    tiles[1].id );
    EXPECT_EQ ( 2u,    tiles[2].id );
    EXPECT_EQ ( 3u,    tiles[3].id );

    EXPECT_DOUBLE_EQ ( 0,     tiles[0].minmax[0] );
    EXPECT_DOUBLE_EQ ( 0,     tiles[0].minmax[1] );
    EXPECT_DOUBLE_EQ ( 12800, tiles[0].minmax[2] );
    EXPECT_DOUBLE_EQ ( 12800, tiles[0].minmax[3] );

    EXPECT_DOUBLE_EQ ( 12800, tiles[1].minmax[0] );
    EXPECT_DOUBLE_EQ ( 0,     tiles[1].minmax[1] );
    EXPECT_DOUBLE_EQ ( 25600, tiles[1].minmax[2] );
    EXPECT_DOUBLE_EQ ( 12800, tiles[1].minmax[3] );

    EXPECT_DOUBLE_EQ ( 0,     tiles[2].minmax[0] );
    EXPECT_DOUBLE_EQ ( 12800, tiles[2].minmax[1] );
    EXPECT_DOUBLE_EQ ( 12800, tiles[2].minmax[2] );
    EXPECT_DOUBLE_EQ ( 25600, tiles[2].minmax[3] );

    EXPECT_DOUBLE_EQ ( 12800, tiles[3].minmax[0] );
    EXPECT_DOUBLE_EQ ( 12800, tiles[3].minmax[1] );
    EXPECT_DOUBLE_EQ ( 25600, tiles[3].minmax[2] );
    EXPECT_DOUBLE_EQ ( 25600, tiles[3].minmax[3] );
}

// ================================================================================

TEST_F ( SmartTilesTestCase, GetTiles_level2 ) {
    double minmax[4] = { 0, 0, 12800, 12800 };
    Tile tiles[256];
    int tilecnt = GetTiles ( minmax, tiles );

    EXPECT_EQ ( 4,     tilecnt );
    EXPECT_EQ ( 2u,    tiles[0].level );
    EXPECT_EQ ( 0u,    tiles[0].id );
    EXPECT_EQ ( 1u,    tiles[1].id );
    EXPECT_EQ ( 4u,    tiles[2].id );
    EXPECT_EQ ( 5u,    tiles[3].id );

    EXPECT_DOUBLE_EQ ( 0,     tiles[0].minmax[0] );
    EXPECT_DOUBLE_EQ ( 0,     tiles[0].minmax[1] );
    EXPECT_DOUBLE_EQ ( 6400,  tiles[0].minmax[2] );
    EXPECT_DOUBLE_EQ ( 6400,  tiles[0].minmax[3] );

    EXPECT_DOUBLE_EQ ( 6400,  tiles[1].minmax[0] );
    EXPECT_DOUBLE_EQ ( 0,     tiles[1].minmax[1] );
    EXPECT_DOUBLE_EQ ( 12800, tiles[1].minmax[2] );
    EXPECT_DOUBLE_EQ ( 6400,  tiles[1].minmax[3] );

    EXPECT_DOUBLE_EQ ( 0,     tiles[2].minmax[0] );
    EXPECT_DOUBLE_EQ ( 6400,  tiles[2].minmax[1] );
    EXPECT_DOUBLE_EQ ( 6400,  tiles[2].minmax[2] );
    EXPECT_DOUBLE_EQ ( 12800, tiles[2].minmax[3] );

    EXPECT_DOUBLE_EQ ( 6400,  tiles[3].minmax[0] );
    EXPECT_DOUBLE_EQ ( 6400,  tiles[3].minmax[1] );
    EXPECT_DOUBLE_EQ ( 12800, tiles[3].minmax[2] );
    EXPECT_DOUBLE_EQ ( 12800, tiles[3].minmax[3] );
}

// ================================================================================

TEST_F ( SmartTilesTestCase, GetTiles_level3 ) {
    double minmax[4] = { 0, 0, 6400, 6400 };
    Tile tiles[256];
    int tilecnt = GetTiles ( minmax, tiles );

    EXPECT_EQ ( 4,     tilecnt );
    EXPECT_EQ ( 3u,    tiles[0].level );
    EXPECT_EQ ( 0u,    tiles[0].id );
    EXPECT_EQ ( 1u,    tiles[1].id );
    EXPECT_EQ ( 8u,    tiles[2].id );
    EXPECT_EQ ( 9u,    tiles[3].id );

    EXPECT_DOUBLE_EQ ( 0,     tiles[0].minmax[0] );
    EXPECT_DOUBLE_EQ ( 0,     tiles[0].minmax[1] );
    EXPECT_DOUBLE_EQ ( 3200,  tiles[0].minmax[2] );
    EXPECT_DOUBLE_EQ ( 3200,  tiles[0].minmax[3] );

    EXPECT_DOUBLE_EQ ( 3200,  tiles[1].minmax[0] );
    EXPECT_DOUBLE_EQ ( 0,     tiles[1].minmax[1] );
    EXPECT_DOUBLE_EQ ( 6400,  tiles[1].minmax[2] );
    EXPECT_DOUBLE_EQ ( 3200,  tiles[1].minmax[3] );

    EXPECT_DOUBLE_EQ ( 0,     tiles[2].minmax[0] );
    EXPECT_DOUBLE_EQ ( 3200,  tiles[2].minmax[1] );
    EXPECT_DOUBLE_EQ ( 3200,  tiles[2].minmax[2] );
    EXPECT_DOUBLE_EQ ( 6400,  tiles[2].minmax[3] );

    EXPECT_DOUBLE_EQ ( 3200,  tiles[3].minmax[0] );
    EXPECT_DOUBLE_EQ ( 3200,  tiles[3].minmax[1] );
    EXPECT_DOUBLE_EQ ( 6400,  tiles[3].minmax[2] );
    EXPECT_DOUBLE_EQ ( 6400,  tiles[3].minmax[3] );
}

// ================================================================================

TEST_F ( SmartTilesTestCase, GetTiles_level4 ) {
    double minmax[4] = { 0, 0, 3200, 3200 };
    Tile tiles[256];
    int tilecnt = GetTiles ( minmax, tiles );

    EXPECT_EQ ( 4,     tilecnt );
    EXPECT_EQ ( 4u,    tiles[0].level );
    EXPECT_EQ ( 0u,    tiles[0].id );
    EXPECT_EQ ( 1u,    tiles[1].id );
    EXPECT_EQ ( 16u,   tiles[2].id );
    EXPECT_EQ ( 17u,   tiles[3].id );

    EXPECT_DOUBLE_EQ ( 0,     tiles[0].minmax[0] );
    EXPECT_DOUBLE_EQ ( 0,     tiles[0].minmax[1] );
    EXPECT_DOUBLE_EQ ( 1600,  tiles[0].minmax[2] );
    EXPECT_DOUBLE_EQ ( 1600,  tiles[0].minmax[3] );

    EXPECT_DOUBLE_EQ ( 1600,  tiles[1].minmax[0] );
    EXPECT_DOUBLE_EQ ( 0,     tiles[1].minmax[1] );
    EXPECT_DOUBLE_EQ ( 3200,  tiles[1].minmax[2] );
    EXPECT_DOUBLE_EQ ( 1600,  tiles[1].minmax[3] );

    EXPECT_DOUBLE_EQ ( 0,     tiles[2].minmax[0] );
    EXPECT_DOUBLE_EQ ( 1600,  tiles[2].minmax[1] );
    EXPECT_DOUBLE_EQ ( 1600,  tiles[2].minmax[2] );
    EXPECT_DOUBLE_EQ ( 3200,  tiles[2].minmax[3] );

    EXPECT_DOUBLE_EQ ( 1600,  tiles[3].minmax[0] );
    EXPECT_DOUBLE_EQ ( 1600,  tiles[3].minmax[1] );
    EXPECT_DOUBLE_EQ ( 3200,  tiles[3].minmax[2] );
    EXPECT_DOUBLE_EQ ( 3200,  tiles[3].minmax[3] );
}

// ================================================================================

TEST_F ( SmartTilesTestCase, GetTiles_level5 ) {
    double minmax[4] = { 0, 0, 1600, 1600 };
    Tile tiles[256];
    int tilecnt = GetTiles ( minmax, tiles );

    EXPECT_EQ ( 1,     tilecnt );
    EXPECT_EQ ( 4u,    tiles[0].level );
    EXPECT_EQ ( 0u,    tiles[0].id );

    EXPECT_DOUBLE_EQ ( 0,     tiles[0].minmax[0] );
    EXPECT_DOUBLE_EQ ( 0,     tiles[0].minmax[1] );
    EXPECT_DOUBLE_EQ ( 1600,  tiles[0].minmax[2] );
    EXPECT_DOUBLE_EQ ( 1600,  tiles[0].minmax[3] );
}

// ================================================================================
