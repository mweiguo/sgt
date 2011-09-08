#include "tools.h"
#include <utility>
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

//================================================================================

void Tool::OnLButtonUp ( int x, int y )
{
}

//================================================================================

void Tool::OnLMouseMove ( int x, int y )
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
}

//================================================================================

void HandTool::OnLButtonUp ( int x, int y )
{
}

//================================================================================

void HandTool::OnLMouseMove ( int x, int y )
{
}

//--------------------------------------------------------------------------------

Tools::Tools ()
{
    currentTool = NULL;
}

//================================================================================

void Tools::setTools ( ToolType toolType )
{
    if ( toolType & ZOOMIN_TOOL )
	tools.insert ( pair<int,Tool>(ZOOMIN_TOOL,ZoomInTool(this)) );
    if ( toolType & ZOOMOUT_TOOL )
	tools.insert ( pair<int,Tool>(ZOOMOUT_TOOL,ZoomOutTool(this)) );
    if ( toolType & HAND_TOOL )
	tools.insert ( pair<int,Tool>(HAND_TOOL,HandTool(this)) );
}

//================================================================================

void Tools::selectTool ( ToolType tooltype )
{
    map<int, Tool>::iterator pp = tools.find ( tooltype );
    if ( pp == tools.end() )
	return;
    currentTool = &pp->second;
}

//================================================================================
