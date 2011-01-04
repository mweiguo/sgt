#include "sgr_renderflow.h"

#include <string>
#include <map>
#include <algorithm>

#include "sgr_projection.h"
#include "sgr_scenemgr.h"
#include "sgr_cameraortho.h"
#include "sgr_viewport.h"
#include "sgr_mat4.h"
#include "sgr_renderfunctor.h"
#include "sgr_kdtree.h"
//#include "nodevisible.h"
#include "sgr_culling.h"
#include "sgr_nodemgr.h"

#include "sgr_nodedumper.h"
#include "sgr_global.h"

#include "sgr_renderlist.h"

using namespace std; 
namespace SGR
{
    Rendering::Rendering ( RenderList& renderlist, RenderOption& opt ) 
    {
	QtRenderVisitor func ( &opt );

	const AttrSet* lastAttrset = NULL;
	for ( RenderList::iterator pp=renderlist.begin(); pp!=renderlist.end(); ++pp )
	{
	    if ( (*pp)->getAttrSet() != lastAttrset )
	    {
		// switch all state in AttrSet
		QtStateChanger changeState ( &opt, (*pp)->getAttrSet() );
		lastAttrset = (*pp)->getAttrSet();
	    }
	    (const_cast<DrawableNode*>(*pp))->accept ( func );
	}

	// clean up renderlist
	for ( RenderList::const_iterator pp=renderlist.begin(); pp!=renderlist.end(); ++pp )
	    delete *pp;
	renderlist.reset ();
    }

    RenderFlow::RenderFlow ( Viewport& vp, RenderOption& opt, std::list<SGNode*> scenes )
    {
	CameraOrtho* cam = vp.camera();
	Projection* proj = vp.projection();
	int camid = vp.cameraid();

	int state = CAMERACHECKING;
	int cullingstart, renderstart;
	int cullingend, renderend;

	// fill data into RenderOption
	mat4f old = opt.matrix;
	if ( NULL == proj )
	{
	    mat4f projmat;
	    opt.matrix = vp.vpmatrix() * projmat * cam->mvmatrix();
	    opt.reverse_mvpw = cam->inversematrix() * projmat * vp.inversematrix();
	}
	else
	{
	    opt.matrix = vp.vpmatrix() * proj->projmatrix() * cam->mvmatrix();
	    opt.reverse_mvpw = cam->inversematrix() * proj->inversematrix() * vp.inversematrix();
	}
	opt.camera = cam;
	opt.painter->setMatrix ( QMatrix( opt.matrix.m00(), opt.matrix.m10(), opt.matrix.m01(), opt.matrix.m11(), opt.matrix.dx(), opt.matrix.dy() ) );

	// begin pipeline
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
		if ( scenes.empty() )
		    scenes.push_back (NodeMgr::getInst().root());
		Culling::getInst()( camid, cam->viewvolume(), &opt, scenes );
		cam->dirty( false );
		state = RENDERING;
		cullingend = clock ();
		qDebug ( "culling: %d clock, %d objects culled", cullingend-cullingstart, (Culling::getInst()[camid])->size() );
		break;
	    case RENDERING:
            {
                renderstart = clock();
                Rendering ( *(Culling::getInst()[camid]), opt );
                renderend = clock();

		int renderObjCnt = (Culling::getInst()[camid])->size();
                qDebug ( "rendering: %d clock;  render objects: %d", renderend-renderstart, renderObjCnt );
            }
            state = END;
            break;
	    }
	}
	// restore matrix in RenderOption
	opt.matrix = old;
    }

}
