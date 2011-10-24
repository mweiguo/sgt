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

class MouseTool : public Tool
{
public:
    MouseTool ( Tools* tools );
    virtual void OnLButtonDown ( int x, int y );
    virtual void OnLButtonUp ( int x, int y );
    virtual void OnLMouseMove ( int x, int y );
protected:
    float startPos[2], lastPos[2];
};

class RubberBoxTool : public MouseTool
{
public:
    RubberBoxTool ( Tools* tools );
    virtual void OnLButtonDown ( int x, int y );
    virtual void OnLButtonUp ( int x, int y );
    virtual void OnLMouseMove ( int x, int y );
protected:
    int rectid;
};

class ZoomTool : public RubberBoxTool
{
public:
    ZoomTool ( Tools* tools );
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
class GLScrollWidget;
class Tools
{
public:
    enum ToolType
    {
	NONE_TOOL     = 1,
	ZOOM_TOOL     = 2,
	HAND_TOOL     = 4
    };
    Tools ( MainWindow* cont, GLScrollWidget* parent );
    ~Tools ();
    void setTools ( int toolType );
    int selectTool ( int tooltype );
    void clearTools ();
    std::map<int, Tool*> tools;
    Tool* currentTool;
    int currentToolType;
    MainWindow* context;
    GLScrollWidget* parent;
};

#endif // _TOOLS_H_

