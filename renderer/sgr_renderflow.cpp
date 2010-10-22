#include "sgr_renderflow.h"

#include <string>
#include <map>
#include <algorithm>

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
#include "sgr_projection.h"

using namespace std; 
namespace SGR
{

// bool lessAttrSet ( DrawableNode* lhs, DrawableNode* rhs )
// {
//     AttrSet *plhs = lhs->getAttrSet();
//     AttrSet *prhs = rhs->getAttrSet();
//     if ( plhs && prhs )
//     {
//         if ( plhs->getRenderOrder() < prhs->getRenderOrder() )
//             return true;
//         else if ( plhs->getRenderOrder() == prhs->getRenderOrder() )
//         {
//             if ( plhs < prhs )
//                 return true;
//         }
//     }
//     else
//         return plhs < prhs;

//     return false;
// }

// bool equalAttrSet ( DrawableNode* lhs, DrawableNode* rhs )
// {
//     return lhs->getAttrSet() == rhs->getAttrSet();
// }

Rendering::Rendering ( RenderList& renderlist, RenderOption& opt ) 
{
    QtRenderVisitor func ( &opt );

    AttrSet* lastAttrset = NULL;
    for ( RenderList::iterator pp=renderlist.begin(); pp!=renderlist.end(); ++pp )
    {
        if ( (*pp)->getAttrSet() != lastAttrset )
        {
            // switch all state in AttrSet
            QtStateChanger changeState ( &opt, (*pp)->getAttrSet() );
            lastAttrset = (*pp)->getAttrSet();
        }
        (*pp)->accept ( func );
    }
/*
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
*/
}

RenderFlow::RenderFlow ( Viewport& vp, RenderOption& opt )
{
    CameraOrtho* cam = vp.camera();
    Projection& proj = vp.projection();
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
            qDebug ( "culling: %d clock", cullingend-cullingstart );
            break;
        case RENDERING:
            {
                renderstart = clock();

                mat4f old = opt.matrix;
                opt.matrix = vp.vpmatrix() * proj.projmatrix() * cam->mvmatrix();
                opt.reverse_mvpw = cam->inversematrix() * proj.inversematrix() * vp.inversematrix();
                //opt.matrix = cam->mvmatrix();
                //opt.scale = cam->mvmatrix().sx();
                opt.painter->setMatrix ( QMatrix( opt.matrix.m00(), opt.matrix.m10(), opt.matrix.m01(), opt.matrix.m11(), opt.matrix.dx(), opt.matrix.dy() ) );
                Rendering ( *(Culling::getInst()[camid]), opt );
                opt.matrix = old;

                renderend = clock();
                qDebug ( "rendering: %d clock;  render objects: %d", renderend-renderstart, (Culling::getInst()[camid])->size() );
            }
            state = END;
            break;
        }

    }
}

}
