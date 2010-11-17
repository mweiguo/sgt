#include "coordquery.h"
#include <sgr_interface.h>
#include <sgr_vec4.h>
#include <sgr_mat4.h>
#include "view.h"

CoordQueryTool::CoordQueryTool ( SGVTools* tools ) : SGVTool ( tools )
{
}

SGR::mat4f CoordQueryTool::getVPM () const
{
    SGR::mat4f mat4camera, mat4proj, mat4vp;
    get_cameramatrix ( _pview->camid(), &(mat4camera.m00()) );
    get_projectionmatrix ( _pview->projid(), &(mat4proj.m00()) );
    get_viewportmatrix ( _pview->vpid(), &(mat4vp.m00()) );

    return mat4vp * mat4proj * mat4camera;
}

SGR::mat4f CoordQueryTool::getIMPV () const
{
    SGR::mat4f imvmat, iprmat, ivpmat;
    get_camerainversematrix ( _pview->camid(), &(imvmat.m00()) );
    get_projectioninversematrix ( _pview->projid(), &(iprmat.m00()) );
    get_viewportinversematrix ( _pview->vpid(), &(ivpmat.m00()) );
    return imvmat * iprmat * ivpmat;
}


SGR::vec2f CoordQueryTool::viewportToScene ( SGR::vec2f vpxy ) const
{
    SGR::mat4f mat = getIMPV ();
    SGR::vec4f scenepos = mat * SGR::vec4f ( vpxy );
    return scenepos.xy();
}

