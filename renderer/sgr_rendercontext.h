#ifndef _RENDER_CONTEXT_H_
#define _RENDER_CONTEXT_H_

#include "sgr_mat4.h"
#include "sgr_bbox.h"

class QPainter;
class QPaintDevice;

namespace SGR
{
    class CameraOrtho;
    struct SGR_DLL RenderOption
    {
	BBox bb;
	QPainter* painter;
	QPaintDevice* orgpaintdevice;
	//mat4f* mvmatrix;
	// matrix = viewpmapping * model-view
	mat4f matrix, reverse_mvpw;
	float scale;
	CameraOrtho* camera;
	RenderOption() : painter(0) {}
    };
}
#endif // _RENDER_CONTEXT_H_
