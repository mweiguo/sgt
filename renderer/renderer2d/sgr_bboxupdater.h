#ifndef _BBOX_UPDATEER_H_
#define _BBOX_UPDATEER_H_

#include "sgr_lc.h"
#include <bbox2d.h>

class BBox2dUpdater
{
public:
    static void forward_update ( LC& lc );
    static void lazy_update ( LC& lc );
private:
    static BBox2d traverse ( LC& lc );
};

#endif // _BBOX_UPDATEER_H_
