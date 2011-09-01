#ifndef _SGR_LC2KDT_H_
#define _SGR_LC2KDT_H_

#include "sgr_lc.h"
#include <kdtree.h>
#include <map>
using namespace std;

class LC2KDT
{
public:
    // convient interface
    static void buildKDT ( LC& lc, KdTree<int>& kdtree );
    // detailed interface
    static void traverse ( LC& lc, KdTree<int>& kdtree );
    static void collectPrimitive ( int type, int gidx, KdTree<int>& kdtree );
    // more practicle interface
    static void buildKDTs ( LC& lc );
private:
    static void traverse2 ( LC& lc );
    static bool collectPrimitive2 ( LC& lc );

    static KdTree<int>* curKDT;
    static map<string, KdTree<int> > kdtreemap;
};


#endif// _SGR_NODE2KDT_H_

