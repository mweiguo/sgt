#ifndef _COORDQUERYTOOL_H_
#define _COORDQUERYTOOL_H_
#include "sgv_tools.h"
#include <sgr_vec2.h>
#include <sgr_mat4.h>

class CoordQueryTool : public SGVTool
{
public:
    CoordQueryTool ( SGVTools* tools );

    SGR::mat4f getVPM () const;
    SGR::mat4f getIMPV () const;
    
    SGR::vec2f viewportToScene ( SGR::vec2f vpxy ) const;
};

#endif // _COORDQUERYTOOL_H_
