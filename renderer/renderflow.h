#ifndef _RENDERFLOW_H_
#define _RENDERFLOW_H_

#include "renderlist.h"
#include "agef_global.h"

struct RenderOption;
class AGEF_EXPORT Rendering
{
public:
    Rendering ( RenderList& renderlist, RenderOption& opt ); 
};


class Viewport;

class AGEF_EXPORT RenderFlow
{
    static const int END               = 0;
    static const int CAMERACHECKING    = 1;
    static const int VIEWPORTCHECKING  = 2;
//    static const int LODING            = 3;
    static const int CULLING           = 4;
    static const int RENDERING         = 5;
public:
    RenderFlow::RenderFlow ( Viewport& vp, RenderOption& opt );
};

#endif
