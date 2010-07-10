#ifndef _RENDERLIST_H_
#define _RENDERLIST_H_

#include "quickvector.h"
#include <map>

class DrawableNode;
typedef quickvector<DrawableNode*> RenderList;
typedef std::map<int, RenderList*> RenderListMgr;

#endif
