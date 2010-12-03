#include "sgr_renderflow.h"

#include <string>
#include <map>
#include <algorithm>

#include "sgr_projection.h"
#include "sgr_scenemgr.h"
#include "sgr_cameraortho.h"
#include "sgr_viewport.h"
#include "sgr_mat4.h"
#include "sgr_vec2.h"
#include "sgr_renderfunctor.h"
#include "sgr_glrenderfunctor.h"
#include "sgr_kdtree.h"
//#include "nodevisible.h"
#include "sgr_culling.h"
#include "sgr_nodemgr.h"

#include "sgr_nodedumper.h"
#include "sgr_global.h"

#include "sgr_renderlist.h"

#include <tinylog.h>

using namespace std; 
namespace SGR
{

int Rendering::renderlib = Rendering::RENDER_IN_QT;
const int Rendering::RENDER_IN_QT       = 1;
const int Rendering::RENDER_IN_OPENGL   = 2;

Rendering::Rendering ( RenderList& renderlist, RenderOption& opt ) 
{
    if ( renderlib == RENDER_IN_QT )
	QtRenderVisitor func ( renderlist, &opt );
    else if ( renderlib == RENDER_IN_OPENGL )
	GLRenderVisitor func ( &opt, renderlist );
}

RenderFlow::RenderFlow ( Viewport& vp, RenderOption& opt )
{
    CameraOrtho* cam = vp.camera();
    Projection* proj = vp.projection();
    int camid = vp.cameraid();

    int state = CAMERACHECKING;
    int cullingstart, renderstart;
    int cullingend, renderend;

    while ( state != END )
    {
        switch ( state )
        {
        case CAMERACHECKING:
            if ( cam->dirty() )
            {
                state = CULLING;
                cam->dirty(false);
            }
            else
                state = VIEWPORTCHECKING;
            break;
        case VIEWPORTCHECKING:
            if ( vp.dirty() )
            {
                state = RENDERING;
                vp.dirty(false);
            }
            else
                state = END;
            break;
        case CULLING:
            cullingstart = clock ();

            Culling::getInst()( camid, cam->viewvolume(), NodeMgr::getInst().root() );
            cam->dirty( false );
            state = RENDERING;

            cullingend = clock ();
            LOG_INFO ( "culling: %d clock\n", cullingend-cullingstart );
            break;
        case RENDERING:
            {
                renderstart = clock();

                mat4f old = opt.matrix;
                opt.matrix = vp.vpmatrix() * proj->projmatrix() * cam->mvmatrix();
                opt.reverse_mvpw = cam->inversematrix() * proj->inversematrix() * vp.inversematrix();

		opt.vpXYWH[0] = vp.position().x();
		opt.vpXYWH[1] = vp.position().y();
		opt.vpXYWH[2] = vp.viewportSize().x();
		opt.vpXYWH[3] = vp.viewportSize().y();

		opt.mvmat = cam->mvmatrix();
		opt.projmat = proj->projmatrix();
                Rendering ( *(Culling::getInst()[camid]), opt );

                opt.matrix = old;

                renderend = clock();
                LOG_INFO ( "rendering: %d clock;  render objects: %d\n", renderend-renderstart, (Culling::getInst()[camid])->size() );
            }
            state = END;
            break;
        }

    }
}

}
