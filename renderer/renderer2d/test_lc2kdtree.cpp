#include "sgr_lc.h"
#include "sgr_lc2kdtree.h"

int main ( int argc, char* argv[] )
{
    LC lc;
    lc.load ( argv[1] );

    KdTree<int> kdtree;
    LC2KDT::buildKDT (lc, kdtree);

    kdtree.save ( "filename.kdt" );
    lc.free ();
    return 0;
}
