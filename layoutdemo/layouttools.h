#ifndef _LAYOUT_TOOLS_H_
#define _LAYOUT_TOOLS_H_

#include <tools.h>
#include <string>

class KeyboardTool : public Tool
{
public:
    KeyboardTool ( Tools* tools );
    virtual void OnKeyPress ( int key, int modifiers );
    virtual void OnKeyRelease ( int key, int modifiers );
    void parseAndDoCommand ();
    std::string cmd;
};

#endif // _LAYOUT_TOOLS_H_
