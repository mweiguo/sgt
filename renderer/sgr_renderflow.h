#ifndef _RENDERFLOW_H_
#define _RENDERFLOW_H_

//#include "sgr_renderlist.h"
#include "sgr_quickvector.h"
//#include "sgr_global.h"
namespace SGR
{
    class DrawableNode;

typedef quickvector<DrawableNode*> RenderList;

struct RenderOption;
class /* SGR_DLL */ Rendering
{
public:
    Rendering ( RenderList& renderlist, RenderOption& opt ); 
};


class Viewport;

class /* SGR_DLL */ RenderFlow
{
    static const int END               = 0;
    static const int CAMERACHECKING    = 1;
    static const int VIEWPORTCHECKING  = 2;
//    static const int LODING            = 3;
    static const int CULLING           = 4;
    static const int RENDERING         = 5;
public:
    RenderFlow ( Viewport& vp, RenderOption& opt );
};

}
#endif
