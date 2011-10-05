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
    virtual void OnMButtonDown ( int x, int y );
    virtual void OnMButtonUp ( int x, int y );
    virtual void OnMMouseMove ( int x, int y );
    virtual void OnKeyPress ( int key, int modifiers );
    virtual void OnKeyRelease ( int key, int modifiers );

protected:
    Tools* _tools;
};

class NoneTool : public Tool
{
public:
    NoneTool ( Tools* tools );
    virtual void OnLButtonDown ( int x, int y );
    virtual void OnLButtonUp ( int x, int y );
    virtual void OnLMouseMove ( int x, int y );
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
    virtual void OnMButtonDown ( int x, int y );
    virtual void OnMButtonUp ( int x, int y );
    virtual void OnMMouseMove ( int x, int y );
private:
    float startPos[2];
};

class FullContentTool : public Tool
{
public:
    
};

class MainWindow;
class Tools
{
public:
    enum ToolType
    {
	NONE_TOOL     = 1,
	ZOOMIN_TOOL   = 2,
	ZOOMOUT_TOOL  = 4,
	HAND_TOOL     = 8
    };
    Tools ( MainWindow* cont );
    ~Tools ();
    void setTools ( ToolType toolType );
    int selectTool ( int tooltype );
    std::map<int, Tool*> tools;
    Tool* currentTool;
    int currentToolType;
    MainWindow* context;
};

#endif // _TOOLS_H_

