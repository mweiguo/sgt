#ifndef _LOCATE_TOOL_H_
#define _LOCATE_TOOL_H_
#include "sgv_tools.h"
#include <sgr_vec3.h>

class LocateTool : public SGVTool
{
public:
    LocateTool ( SGVTools* tools );
    void locateAll ();
    void locate_object ( int id );
private:
    float _currentScale;
    SGR::vec3f _currentTranslate;
};


#endif // _LOCATE_TOOL_H_
