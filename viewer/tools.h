#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <map>

class Tools;
class Tool
{
public:
    enum KeyboardModifiers
    {
	KB_NoModifier           = 0,
        KB_ShiftModifier        = 1,
        KB_ControlModifier      = 2,
        KB_AltModifier          = 4,
        KB_MetaModifier         = 8,
        KB_KeypadModifier       = 16,
        KB_GroupSwitchModifier  = 32
    };

    Tool ( Tools* tools );
    virtual ~Tool();

    virtual void OnLButtonDown ( int x, int y );
    virtual void OnLButtonUp ( int x, int y );
    virtual void OnLMouseMove ( int x, int y );
    virtual void OnKeyPress ( int key, int modifiers );
    virtual void OnKeyRelease ( int key, int modifiers );

private:
    Tools* _tools;
};

class ZoomInTool : public Tool
{
public:
    ZoomInTool ( Tools* tools );
    virtual void OnLButtonDown ( int x, int y );
    virtual void OnLButtonUp ( int x, int y );
    virtual void OnLMouseMove ( int x, int y );
};

class ZoomOutTool : public Tool
{
public:
    ZoomOutTool ( Tools* tools );
    virtual void OnLButtonDown ( int x, int y );
    virtual void OnLButtonUp ( int x, int y );
    virtual void OnLMouseMove ( int x, int y );
};

class HandTool : public Tool
{
public:
    HandTool ( Tools* tools );
    virtual void OnLButtonDown ( int x, int y );
    virtual void OnLButtonUp ( int x, int y );
    virtual void OnLMouseMove ( int x, int y );
};

class FullContentTool : public Tool
{
public:
    
};

class Tools
{
public:
    enum ToolType
    {
	ZOOMIN_TOOL   = 1,
	ZOOMOUT_TOOL  = 2,
	HAND_TOOL     = 4
    };
    Tools ();
    void setTools ( ToolType toolType );
    void selectTool ( ToolType tooltype );
    std::map<int, Tool> tools;
    Tool* currentTool;
};

#endif // _TOOLS_H_

