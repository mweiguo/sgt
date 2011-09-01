#include "sgr_lc.h"


int main ( int argc, char* argv[] )
{
    LC lc;
    lc.load ( argv[1] );
    LC2KDT lc2kdt ( lc );
    const KdTree<int>& kdtree = lc2kdt.buildKDT ();
    kdtree.save ( "filename.kdt" );
    lc.free ();
    return 0;
}
