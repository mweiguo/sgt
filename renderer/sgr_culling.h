#ifndef _CULLING_H_
#define _CULLING_H_

#include "sgr_nodevisitor.h"
#include "sgr_renderlist.h"
#include "sgr_scenenode.h"
#include "sgr_attrset.h"
#include "sgr_volumepicker.h"

#include <map>
#include <iterator>
using namespace std;
namespace SGR
{

class SGR_DLL Culling : public map<int, RenderList*>
{
public:
    typedef RenderListMgr::iterator iterator;
    static Culling& getInst() 
    {
        static Culling inst;
        return inst;
    }

    void operator() ( int camid, const BBox& box, SGNode* node ) 
    {
        //        static Rectanglef rect;
        typedef vector<SpatialObjectMgr*> SpatialObjectMgrs;
        RenderListMgr::iterator pp = find ( camid );
        RenderList* renderlist=0;
        if ( pp == end() )
            (*this)[camid] = renderlist = new RenderList();
        else
            renderlist = (*pp).second;
        renderlist->reset();

        VolumePicker<back_insert_iterator<RenderList> > picker ( box, camid, back_inserter(*renderlist) );
        picker (*node);
        //#ifdef _USESTATISTIC_
        //            qDebug ( "the culling volumn min=(%f,%f,%f), max=(%f,%f,%f)", 
        //                box.min().x(), box.min().y(), box.min().z(),
        //                box.max().x(), box.max().y(), box.max().z() );
        //            qDebug ( "%s", (*pp)->intersectstatistic ().c_str() );
        //#endif
        //        insert scene bbox to renderlist
        
        CameraOrtho* cam = NodeMgr::getInst().getNodePtr<CameraOrtho>(camid);
        if ( cam ) 
        {
            BBox box = cam->viewvolume();
            RectangleNodef *rect = new RectangleNodef();
            rect->setRect ( box.minvec().x(), box.minvec().y(), 
                box.maxvec().x() - box.minvec().x(),
                box.maxvec().y() - box.minvec().y() );
            AttrSet* pAttrSet = new AttrSet(100);
            pAttrSet->setBgColor ( new GColor(0) );
            rect->setAttrSet ( pAttrSet );
            renderlist->push_back ( rect );
        }
        
    }
private:
    Culling () {}
};
}
#endif
