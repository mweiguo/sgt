#include "coordquery.h"
#include <sgr_interface.h>
#include <sgr_vec4.h>
#include <sgr_mat4.h>
#include "view.h"

CoordQueryTool::CoordQueryTool ()
{
}

SGR::vec2f CoordQueryTool::viewportToScene ( SGR::vec2f vpxy )
{
    float imv[16], ipr[16], ivp[16];
    get_camerainversematrix ( _pview->camid(), imv );
    get_projectioninversematrix ( _pview->projid(), ipr );
    get_viewportinversematrix ( _pview->vpid(), ivp );
    SGR::mat4f imvmat(imv), iprmat(ipr), ivpmat(ivp);
    SGR::mat4f mat = imvmat * iprmat * ivpmat;

    SGR::vec4f scenepos = mat * SGR::vec4f ( vpxy );
    return scenepos.xy();
}

