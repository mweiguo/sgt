#ifndef __SGV_TOOLS_H__
#define __SGV_TOOLS_H__

#include <map>
#include <sgr_vec2.h>
class SGVTool;
class View;
class SGVTools
{
public:
    static const int HAND_TOOL;
    static const int ZOOM_TOOL;
    static const int COORDQUERY_TOOL;
    static const int LOCATE_TOOL;

    static SGVTools& getInst();
    SGVTools ();
    ~SGVTools ();
    void initialize ( View* pview );

    SGVTool* selectTool ( int tool );
    SGVTool* currentTool ();
    template<class ToolType>
    ToolType* getTool ( const int tool )
    {
	std::map< int, SGVTool* >::iterator pp = _tools.find ( tool );
	if ( pp==_tools.end() )
	    return NULL;
	return dynamic_cast<ToolType*>(pp->second);
    }

    void addTool ( int );
    void removeTool ( int );
private:
    std::map< int, SGVTool* > _tools;
    SGVTool* _currentTool;
    View* _pview;
};

class SGVTool
{
public:
    SGVTool ( SGVTools* tools );

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

    void initialize ( View* pview );
protected:
    bool _isCtrlPressed, _isAltPressed, _isShiftPressed;
    float _startX, _startY, _lastX, _lastY;

    View* _pview;
    SGVTools* _tools;
};

#endif // __SGV_TOOLS_H__
