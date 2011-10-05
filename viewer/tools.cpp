#include "tools.h"
#include <utility>
#include "mainwindow.h"
#include <sgr_render2d.h>
#include <iostream>
using namespace std;

//--------------------------------------------------------------------------------

Tool::Tool( Tools* tools )
{
    _tools = tools;
}

//================================================================================

Tool::~Tool()
{
}

//================================================================================

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

//================================================================================

void Tool::OnKeyPress ( int key, int modifiers )
{
}

//================================================================================

void Tool::OnKeyRelease ( int key, int modifiers )
{
}

//--------------------------------------------------------------------------------

NoneTool::NoneTool ( Tools* tools ) : Tool ( tools )
{
}

//================================================================================

void NoneTool::OnLButtonDown ( int x, int y )
{
}

//================================================================================

void NoneTool::OnLButtonUp ( int x, int y )
{
}

//================================================================================

void NoneTool::OnLMouseMove ( int x, int y )
{
}

//--------------------------------------------------------------------------------

ZoomInTool::ZoomInTool ( Tools* tools ) : Tool ( tools )
{
}

//================================================================================

void ZoomInTool::OnLButtonDown ( int x, int y )
{
}

//================================================================================

void ZoomInTool::OnLButtonUp ( int x, int y )
{
}

//================================================================================

void ZoomInTool::OnLMouseMove ( int x, int y )
{
}

//--------------------------------------------------------------------------------

ZoomOutTool::ZoomOutTool ( Tools* tools ) : Tool ( tools )
{
}

//================================================================================

void ZoomOutTool::OnLButtonDown ( int x, int y )
{
}

//================================================================================

void ZoomOutTool::OnLButtonUp ( int x, int y )
{
}

//================================================================================

void ZoomOutTool::OnLMouseMove ( int x, int y )
{
}

//--------------------------------------------------------------------------------

HandTool::HandTool ( Tools* tools ) : Tool ( tools )
{
}

//================================================================================

void HandTool::OnLButtonDown ( int x, int y )
{
    _tools->context->displayer->setCursor ( Qt::ClosedHandCursor );
    r2d_get_scene_position ( x, y, startPos[0], startPos[1] );
}

//================================================================================

void HandTool::OnLButtonUp ( int x, int y )
{    
    _tools->context->displayer->setCursor ( Qt::OpenHandCursor );
    OnLMouseMove ( x, y );
}

//================================================================================

void HandTool::OnLMouseMove ( int x, int y )
{
    float ScenePos[2];
    r2d_get_scene_position ( x, y, ScenePos[0], ScenePos[1] );
    float dx = ScenePos[0] - startPos[0];
    float dy = ScenePos[1] - startPos[1];
    _tools->context->_translate[0] += dx;
    _tools->context->_translate[1] += dy;
    r2d_loadidentity ();
    r2d_scale ( _tools->context->_scale );
    r2d_translate ( _tools->context->_translate[0], _tools->context->_translate[1] );
    _tools->context->displayer->update ();
}

void HandTool::OnMButtonDown ( int x, int y )
{
    OnLButtonDown ( x, y );
}

void HandTool::OnMButtonUp ( int x, int y )
{
    OnLButtonUp ( x, y );
}

void HandTool::OnMMouseMove ( int x, int y )
{
    OnLMouseMove ( x, y );
}

//--------------------------------------------------------------------------------

Tools::Tools ( MainWindow* cont)
{
    context = cont;
    currentTool = NULL;
    currentToolType = NONE_TOOL;
}

//================================================================================

Tools::~Tools ()
{
    for ( map<int, Tool*>::iterator pp=tools.begin();
	  pp!=tools.end();
	  ++pp )
	delete pp->second;
}

//================================================================================

void Tools::setTools ( ToolType toolType )
{
    if ( toolType & NONE_TOOL )
	tools.insert ( pair<int,Tool*>(NONE_TOOL,new NoneTool(this)) );
    if ( toolType & ZOOMIN_TOOL )
	tools.insert ( pair<int,Tool*>(ZOOMIN_TOOL,new ZoomInTool(this)) );
    if ( toolType & ZOOMOUT_TOOL )
	tools.insert ( pair<int,Tool*>(ZOOMOUT_TOOL,new ZoomOutTool(this)) );
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
	context->displayer->setCursor ( Qt::ArrowCursor );
	currentToolType = NONE_TOOL;
	break;
    case HAND_TOOL:
	context->displayer->setCursor ( Qt::OpenHandCursor );
	currentToolType = HAND_TOOL;
	break;
    default:
	break;
    }

    currentTool = pp->second;
    return oldType;
}

//================================================================================
