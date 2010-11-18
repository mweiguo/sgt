#ifndef _LOCATE_TOOL_H_
#define _LOCATE_TOOL_H_
#include "sgv_tools.h"
#include <sgr_vec3.h>


class CameraTool : public SGVTool
{
public:
    CameraTool ( SGVTools* tools );
    void locateAll ();
    void locate_object ( int id );

    float find_view ( const SGR::vec3f& minvec, const SGR::vec3f& maxvec, float percentOfView );
    void left ();
    void right ();
    void up ();
    void down ();
    void zoomin ();
    void zoomout ();

    void setCameraConstraint ( int nodeid, float percentOfView );
    void setCameraConstraint ( SGR::vec3f minTranslate, SGR::vec3f maxTranslate, float minScale, float maxScale );
protected:
    float calcScale ( const SGR::vec3f& minvec, const SGR::vec3f& maxvec );
private:
    float _currentScale;
    SGR::vec3f _currentTranslate;

    // camera constraint
    SGR::vec3f _minTranslate, _maxTranslate;
    float _minScale, _maxScale;
    bool _isInConstraintMode;
};


#endif // _LOCATE_TOOL_H_
