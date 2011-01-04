#ifndef _RENDER_H_
#define _RENDER_H_

#include "sgr_interface.h"
// node
#include "sgr_sgnode.h"
#include "sgr_arraynode.h"
#include "sgr_drawablenode.h"
#include "sgr_switchnode.h"
#include "sgr_transformnode.h"
#include "sgr_lodnode.h"
#include "sgr_groupnode.h"
#include "sgr_layernode.h"
#include "sgr_rectitem.h"
#include "sgr_pickablegroup.h"
#include "sgr_kdtreenode.h"
#include "sgr_nodemgr.h"

// camera & viewport
#include "sgr_cameraortho.h"
#include "sgr_viewport.h"

// util
#include "sgr_bbox.h"
#include "sgr_vec2.h"
#include "sgr_vec3.h"
#include "sgr_vec4.h"
#include "mat3.h"
#include "sgr_mat4.h"
#include "sgr_quickvector.h"
#include "sgr_kdtree.h"
#include "sgr_scenemgr.h"

// functors
#include "sgr_nodevisitor.h"
#include "sgr_childvisitor.h"
#include "sgr_parentvisitor.h"
#include "sgr_renderfunctor.h"
#include "sombuilder.h"
#include "sgr_rendernodecollector.h"
//#include "nodevisible.h"
#include "sgr_scenenode.h"
//#include "copier.h"

// io
#include "sgr_loadscene.h"
#include "sgr_savemesh.h"
//#include "savepickable.h"

// render process
#include "sgr_renderflow.h"
#include "sgr_renderlist.h"


#endif
