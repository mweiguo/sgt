#include "tools.h"
#include <utility>
#include "mainwindow.h"
#include <sgr_render2d.h>
#include <iostream>
#include <cmath>
using namespace std;

//--------------------------------------------------------------------------------

Tool::Tool( Tools* tools )
{
    _tools = tools;
}

Tool::~Tool()
{
}

void Tool::OnLButtonDown ( int x, int y )
{
}

void Tool::OnLButtonUp ( int x, int y )
{
}

void Tool::OnLMouseMove ( int x, int y )
{
}

void Tool::OnMButtonDown ( int x, int y )
{
}

void Tool::OnMButtonUp ( int x, int y )
{
}

void Tool::OnMMouseMove ( int x, int y )
{
}

void Tool::OnKeyPress ( int key, int modifiers )
{
}

void Tool::OnKeyRelease ( int key, int modifiers )
{
}

//--------------------------------------------------------------------------------

NoneTool::NoneTool ( Tools* tools ) : Tool ( tools )
{
}

void NoneTool::OnLButtonDown ( int x, int y )
{
}

void NoneTool::OnLButtonUp ( int x, int y )
{
}

void NoneTool::OnLMouseMove ( int x, int y )
{
}

//--------------------------------------------------------------------------------

MouseTool::MouseTool ( Tools* tools ) : Tool(tools)
{
}

void MouseTool::OnLButtonDown ( int x, int y )
{
    r2d_get_scene_position ( x, y, startPos[0], startPos[1] );
}

void MouseTool::OnLButtonUp ( int x, int y )
{
    r2d_get_scene_position ( x, y, lastPos[0], lastPos[1] );
}

void MouseTool::OnLMouseMove ( int x, int y )
{
    r2d_get_scene_position ( x, y, lastPos[0], lastPos[1] );
}

//--------------------------------------------------------------------------------

RubberBoxTool::RubberBoxTool ( Tools* tools ) : MouseTool ( tools )
{
    // get rectangle from misc scene
    if ( _tools->context->doc->miscsceneid != -1 )
    {
	int sid = _tools->context->doc->miscsceneid;
	r2d_to_element ( sid, R2D_ROOT );
	r2d_to_element ( sid, R2D_FIRST_CHILD ); // layer
	r2d_to_element ( sid, R2D_FIRST_CHILD ); // lod
	r2d_to_element ( sid, R2D_FIRST_CHILD ); // lodpage
	rectid = r2d_to_element ( sid, R2D_FIRST_CHILD ); // rect
    }
}

void RubberBoxTool::OnLButtonDown ( int x, int y )
{
    MouseTool::OnLButtonDown ( x, y );
}

void RubberBoxTool::OnLButtonUp ( int x, int y )
{
    MouseTool::OnLButtonUp ( x, y );
    float minmax[6] = { 0, 0, 100, 0, 0, 100 };
    int sid = _tools->context->doc->miscsceneid;
    if ( sid != -1 )
    {
	sid = _tools->context->doc->miscsceneid;
	r2d_to_element ( sid, R2D_ROOT );
	r2d_to_element ( sid, R2D_FIRST_CHILD ); // layer
	r2d_to_element ( sid, R2D_FIRST_CHILD ); // lod
	r2d_to_element ( sid, R2D_FIRST_CHILD ); // lodpage
	rectid = r2d_to_element ( sid, R2D_FIRST_CHILD ); // rect
	r2d_rect_points ( sid, rectid, minmax );
    }
}

void RubberBoxTool::OnLMouseMove ( int x, int y )
{
    MouseTool::OnLMouseMove ( x, y );
    float minmax[6] = { 0, 0, 100, 0, 0, 100 };
    minmax[0] = startPos[0] < lastPos[0] ? startPos[0] : lastPos[0];
    minmax[1] = startPos[1] < lastPos[1] ? startPos[1] : lastPos[1];
    minmax[3] = startPos[0] > lastPos[0] ? startPos[0] : lastPos[0];
    minmax[4] = startPos[1] > lastPos[1] ? startPos[1] : lastPos[1];

    int sid = _tools->context->doc->miscsceneid;
    if ( sid != -1 )
    {
	sid = _tools->context->doc->miscsceneid;
	r2d_to_element ( sid, R2D_ROOT );
	r2d_to_element ( sid, R2D_FIRST_CHILD ); // layer
	r2d_to_element ( sid, R2D_FIRST_CHILD ); // lod
	r2d_to_element ( sid, R2D_FIRST_CHILD ); // lodpage
	rectid = r2d_to_element ( sid, R2D_FIRST_CHILD ); // rect
	r2d_rect_points ( sid, rectid, minmax );
    }
}

//--------------------------------------------------------------------------------

ZoomTool::ZoomTool ( Tools* tools ) : RubberBoxTool ( tools )
{
}

void ZoomTool::OnLButtonDown ( int x, int y )
{
    RubberBoxTool::OnLButtonDown ( x, y );
}

void ZoomTool::OnLButtonUp ( int x, int y )
{
    RubberBoxTool::OnLButtonUp ( x, y );
    float center[2] = { (lastPos[0] + startPos[0]) / 2, (lastPos[1] + startPos[1]) / 2 };
    float size[2]   = { fabs(lastPos[0] - startPos[0]) / 2, fabs(lastPos[1] - startPos[1]) / 2 };
    r2d_loadidentity ();
    _tools->parent->widget->scale = size[0] > size[1] ? 1.0 / size[0] : 1.0 / size[1];
    _tools->parent->widget->translate[0] = -center[0];
    _tools->parent->widget->translate[1] = -center[1];

    _tools->parent->setViewportTransform ( _tools->parent->widget->scale,
					   _tools->parent->widget->translate[0], 
					   _tools->parent->widget->translate[1] );
}

void ZoomTool::OnLMouseMove ( int x, int y )
{
    RubberBoxTool::OnLMouseMove ( x, y );
    _tools->parent->widget->updateGL ();
}

//--------------------------------------------------------------------------------

HandTool::HandTool ( Tools* tools ) : Tool ( tools )
{
}

//================================================================================

void HandTool::OnLButtonDown ( int x, int y )
{
    _tools->parent->widget->setCursor ( Qt::ClosedHandCursor );
    r2d_get_scene_position ( x, y, startPos[0], startPos[1] );
}

//================================================================================

void HandTool::OnLButtonUp ( int x, int y )
{    
    _tools->parent->widget->setCursor ( Qt::OpenHandCursor );
    OnLMouseMove ( x, y );
    _tools->parent->setViewportTransform ( _tools->parent->widget->scale,
					   _tools->parent->widget->translate[0], 
					   _tools->parent->widget->translate[1] );
}

//================================================================================

void HandTool::OnLMouseMove ( int x, int y )
{
    float ScenePos[2];
    r2d_get_scene_position ( x, y, ScenePos[0], ScenePos[1] );
    float dx = ScenePos[0] - startPos[0];
    float dy = ScenePos[1] - startPos[1];
    _tools->parent->widget->translate[0] += dx;
    _tools->parent->widget->translate[1] += dy;
    _tools->parent->widget->update();
}

//================================================================================

void HandTool::OnMButtonDown ( int x, int y )
{
    OnLButtonDown ( x, y );
}

//================================================================================

void HandTool::OnMButtonUp ( int x, int y )
{
    OnLButtonUp ( x, y );
}

//================================================================================

void HandTool::OnMMouseMove ( int x, int y )
{
    OnLMouseMove ( x, y );
}

//--------------------------------------------------------------------------------

Tools::Tools ( MainWindow* cont, GLScrollWidget* p)
{
    context = cont;
    currentTool = NULL;
    currentToolType = NONE_TOOL;
    parent = p;
}

//================================================================================

Tools::~Tools ()
{
    clearTools ();
}

//================================================================================

void Tools::setTools ( int toolType )
{
    if ( toolType & NONE_TOOL )
	tools.insert ( pair<int,Tool*>(NONE_TOOL,new NoneTool(this)) );
    if ( toolType & ZOOM_TOOL )
	tools.insert ( pair<int,Tool*>(ZOOM_TOOL,new ZoomTool(this)) );
    if ( toolType & HAND_TOOL )
	tools.insert ( pair<int,Tool*>(HAND_TOOL,new HandTool(this)) );
}

//================================================================================

int Tools::selectTool ( int tooltype )
{
    map<int, Tool*>::iterator pp = tools.find ( tooltype );
    if ( pp == tools.end() )
	return currentToolType;
    
    int oldType = currentToolType;
    switch ( tooltype )
    {
    case NONE_TOOL:
	parent->widget->setCursor ( Qt::ArrowCursor );
	currentToolType = NONE_TOOL;
	break;
    case HAND_TOOL:
	parent->widget->setCursor ( Qt::OpenHandCursor );
	currentToolType = HAND_TOOL;
	break;
    case ZOOM_TOOL:
	parent->widget->setCursor ( Qt::CrossCursor );
	currentToolType = ZOOM_TOOL;
	break;
    default:
	break;
    }

    currentTool = pp->second;
    return oldType;
}

//================================================================================

void Tools::clearTools ()
{
    for ( map<int, Tool*>::iterator pp=tools.begin();
	  pp!=tools.end();
	  ++pp )
	delete pp->second;
    tools.clear();
}

//================================================================================

