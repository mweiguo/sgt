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
    /** intersect each primitive with view frustum
     */
    static void cull ( LC& lc, const BBox2d& vfbbox, float scale=1, bool dopick=false );
    /** do include test for each primitive with view frustum
     */
    static void cull2 ( LC& lc, const BBox2d& vfbbox, float scale=1, bool dopick=false );
    static void clear ();
    static vector<int> renderObjects;
private:
    static int cull_test ( int type, int idx, LC& lc );
    static void traverse ( LC& lc );
    static void traverse ( LC& lc, int i );

    static int cull_test2 ( int type, int idx, LC& lc );
    static void traverse2 ( LC& lc );
    static void traverse2 ( LC& lc, int i );


    static BBox2d vfbbox2d;
    static float scale;
    static bool doPick;
};

#endif // _VFC_CULLER_H_
