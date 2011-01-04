#include "sgr_culling.h"
#include "sgr_crosspicker.h"

using namespace std;
namespace SGR
{

    Culling culling;

    Culling& Culling::getInst() 
    {
	return culling;
    }

    Culling::Culling ()
    {
    }

    void Culling::operator() ( int camid, const BBox& box, RenderOption* opt, const std::list<SGNode*>& nodes ) 
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

	//CrossPicker crosspicker ( box, camid );
	//crosspicker.doAction (*node);
	//std::copy ( crosspicker.pickedNodes().begin(), crosspicker.pickedNodes().end(), back_inserter(*renderlist) );
	for ( std::list<SGNode*>::const_iterator pp=nodes.begin(); pp!=nodes.end(); ++pp )
	{
	    SGNode* node = *pp;
	    VolumePicker<back_insert_iterator<RenderList> > picker ( box, camid, opt, back_inserter(*renderlist) );
	    picker (*node);
	}
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

}

