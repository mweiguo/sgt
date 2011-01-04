#ifndef _RENDERLIST_H_
#define _RENDERLIST_H_

#include "sgr_quickvector.h"
#include <map>
namespace SGR
{

class DrawableNode;
typedef quickvector<const DrawableNode*> RenderList;
typedef std::map<int, RenderList*> RenderListMgr;
}
#endif
