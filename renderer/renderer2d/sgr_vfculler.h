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
    void cull ( LC& lc, const BBox2d& vfbbox );
    list<int> renderObjects;
private:
    int cull_test ( int type, int idx, LC& lc );
    void traverse ( LC& lc );
    BBox2d vfbbox2d;
};


#endif // _VFC_CULLER_H_
