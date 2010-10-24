#ifndef _CULLING_H_
#define _CULLING_H_

#include "nodevisitor.h"
#include "renderlist.h"
#include "volumepicker.h"
#include "attrset.h"

#include <map>
#include <iterator>
#include <iostream>

using namespace std;

class AGEF_EXPORT Culling : public map<int, RenderList*>
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
            rect->setRect ( box.min().x(), box.min().y(), 
                box.max().x() - box.min().x(),
                box.max().y() - box.min().y() );
            AttrSet* pAttrSet = new AttrSet(100);
            pAttrSet->setBgColor ( new GColor(0x33000033) );
            rect->setAttrSet ( pAttrSet );

	    TextNode *textnode = new TextNode();
	    stringstream ss;
	    ss << box.min().x() << ' '
	       << box.min().y() << ' '
	       << box.max().x() - box.min().x() << ' '
	       << box.max().y() - box.min().y();
	    textnode->text ( ss.str() );

            renderlist->push_back ( rect );
	    renderlist->push_back ( textnode );
        }
        
    }
private:
    Culling () {}
};

#endif
