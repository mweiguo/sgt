#ifndef _COORDQUERYTOOL_H_
#define _COORDQUERYTOOL_H_
#include "sgv_tools.h"
#include <sgr_vec2.h>

class CoordQueryTool : public SGVTool
{
public:
    CoordQueryTool ();
    SGR::vec2f viewportToScene ( SGR::vec2f vpxy );
};

#endif // _COORDQUERYTOOL_H_
