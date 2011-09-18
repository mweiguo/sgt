#ifndef _VFC_CULLER_H_
#define _VFC_CULLER_H_
#include <list>
using namespace std;

#include <bbox2d.h>
#include "sgr_lc.h"

class vfculler
{
public:
    enum CullTestResult {
	CHILD_CULLED,
	CHILD_NOT_CULLED,
	CHILDREN_CULLTEST_FINISHED,
	NONE
    };
    static void cull ( LC& lc, const BBox2d& vfbbox );
    static void clear ();
    static list<int> renderObjects;
private:
    static int cull_test ( int type, int idx, LC& lc );
    static void traverse ( LC& lc );
    static BBox2d vfbbox2d;
};


#endif // _VFC_CULLER_H_
