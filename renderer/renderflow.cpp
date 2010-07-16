#include "renderflow.h"

#include <string>
#include <map>
#include <algorithm>

#include "scenemgr.h"
#include "cameraortho.h"
#include "viewport.h"
#include "mat4.h"
#include "renderfunctor.h"
#include "kdtree.h"
//#include "nodevisible.h"
#include "culling.h"
#include "nodemgr.h"

#include "nodedumper.h"
#include "agef_global.h"
#include "projection.h"

using namespace std; 

bool lessAttrSet ( DrawableNode* lhs, DrawableNode* rhs )
{
    AttrSet *plhs = lhs->getAttrSet();
    AttrSet *prhs = rhs->getAttrSet();
    if ( plhs && prhs )
    {
        if ( plhs->getRenderOrder() < prhs->getRenderOrder() )
            return true;
        else if ( plhs->getRenderOrder() == prhs->getRenderOrder() )
        {
            if ( plhs < prhs )
                return true;
        }
    }
    else
        return plhs < prhs;

    return false;
}

bool equalAttrSet ( DrawableNode* lhs, DrawableNode* rhs )
{
    return lhs->getAttrSet() == rhs->getAttrSet();
}

Rendering::Rendering ( RenderList& renderlist, RenderOption& opt ) 
{
    // sort by attrset
    RenderList::iterator aa =renderlist.begin();
    RenderList::iterator bb =renderlist.end();

    sort ( renderlist.begin(), renderlist.end(), lessAttrSet );
    RenderList copylist;
    copylist.resize ( renderlist.size() );
    RenderList::iterator pp = unique_copy ( renderlist.begin(), renderlist.end(), copylist.begin(), equalAttrSet );
    copylist.resize ( pp - copylist.begin() );
    
    // start render
    QtRenderVisitor func ( &opt );
    for ( RenderList::iterator pp=copylist.begin(); pp!=copylist.end(); ++pp )
    {
        // find range
        pair<RenderList::iterator, RenderList::iterator> bounds = 
            equal_range ( renderlist.begin(), renderlist.end(), *pp, lessAttrSet );

        // switch all state in AttrSet
        QtStateChanger changeState ( &opt, (*pp)->getAttrSet() );

        // render objects
        for ( RenderList::iterator pp1=bounds.first; pp1!=bounds.second; ++pp1 )
            (*pp1)->accept ( func );
    }

    //for ( RenderList::const_iterator pp=renderlist.begin(); pp!=renderlist.end(); ++pp ) {
    //    QtRenderFunctor func ( &opt );
    //    (*pp)->accept ( func );
    //}

    // clean up renderlist
    for ( RenderList::const_iterator pp=renderlist.begin(); pp!=renderlist.end(); ++pp )
        delete *pp;
    renderlist.reset ();
}

RenderFlow::RenderFlow ( Viewport& vp, RenderOption& opt )
{
    CameraOrtho* cam = vp.camera();
    Projection& proj = vp.projection();
    int camid = vp.cameraid();

    int state = CAMERACHECKING;
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
            Culling::getInst()( camid, cam->viewvolume(), NodeMgr::getInst().root() );
            cam->dirty( false );
            state = RENDERING;
            break;
        case RENDERING:
            {
                mat4f old = opt.matrix;
                opt.matrix = vp.vpmatrix() * proj.projmatrix() * cam->mvmatrix();
                //opt.matrix = cam->mvmatrix();
                //opt.scale = cam->mvmatrix().sx();
                opt.painter->setMatrix ( QMatrix( opt.matrix.m00(), opt.matrix.m10(), opt.matrix.m01(), opt.matrix.m11(), opt.matrix.dx(), opt.matrix.dy() ) );
                Rendering ( *(Culling::getInst()[camid]), opt );
                opt.matrix = old;

            }
            state = END;
            break;
        }
    }
}
