#ifndef _CULLING_H_
#define _CULLING_H_

#include "sgr_nodevisitor.h"
#include "sgr_renderlist.h"
#include "sgr_scenenode.h"
#include "sgr_attrset.h"
#include "sgr_volumepicker.h"

#include <map>
#include <list>
#include <iterator>
using namespace std;
namespace SGR
{

    class SGR_DLL Culling : public map<int, RenderList*>
    {
    public:
	typedef RenderListMgr::iterator iterator;
	static Culling& getInst();
	Culling ();

	void operator() ( int camid, const BBox& box, RenderOption* opt, const std::list<SGNode*>& nodes );
    };
}
#endif
