#ifndef __SGV_TOOLS_H__
#define __SGV_TOOLS_H__

#include <map>
#include <sgr_vec2.h>
class SGVTool;
class QViewport;

class SGVTools
{
public:
    static const int HAND_TOOL;
    static const int ZOOM_TOOL;
    static const int COORDQUERY_TOOL;

    static SGVTools& getInst();
    SGVTools ();
    void initialize ( QViewport* view );

    SGVTool* selectTool ( int tool );
    SGVTool* currentTool ();
    template<class ToolType>
    ToolType* getTool ( int tool )
    {
	std::map< int, SGVTool* >::iterator pp = _tools.find ( tool );
	if ( pp==_tools.end() )
	    return NULL;
	return dynamic_cast<ToolType*>(pp->second);
    }
private:
    std::map< int, SGVTool* > _tools;
    SGVTool* _currentTool;
};

class SGVTool
{
public:
    virtual void lbuttondown ( float x, float y );
    virtual void lbuttonup ( float x, float y );
    virtual void lbuttonmove ( float x, float y );

    virtual void rbuttondown ( float x, float y );
    virtual void rbuttonup ( float x, float y );
    virtual void rbuttonmove ( float x, float y );

    virtual void mbuttondown ( float x, float y );
    virtual void mbuttonup ( float x, float y );
    virtual void mbuttonmove ( float x, float y );

    virtual void mousemove ( float x, float y );
    virtual void mousewheel ( float x, float y, float delta );

    virtual void keydown ( int key );
    virtual void keyup ( int key );

    void setCtrlPressed ( bool isPressed );
    bool isCtrlPressed ();

    void setAltPressed ( bool isPressed );
    bool isAltPressed ();

    void setShiftPressed ( bool isPressed );
    bool isShiftPressed ();

    void setView ( QViewport* _view );
protected:
    bool _isCtrlPressed, _isAltPressed, _isShiftPressed;
    float _startX, _startY, _lastX, _lastY;

    QViewport* _view;
};

class HandTool : public SGVTool
{
public:
    virtual void lbuttondown ( float x, float y );
    virtual void lbuttonup ( float x, float y );
    virtual void lbuttonmove ( float x, float y );
    virtual void mbuttondown ( float x, float y );
    virtual void mbuttonup ( float x, float y );
    virtual void mbuttonmove ( float x, float y );
};

class ZoomTool : public SGVTool
{
public:
    ZoomTool ();
    virtual void lbuttondown ( float x, float y );
    virtual void lbuttonup ( float x, float y );
    virtual void lbuttonmove ( float x, float y );
    virtual void mousewheel ( float x, float y, float delta );
protected:
    float _scale;
};

class CoordQueryTool : public SGVTool
{
public:
    CoordQueryTool ();
    SGR::vec2f viewportToScene ( SGR::vec2f vpxy );
};

#endif // __SGV_TOOLS_H__
