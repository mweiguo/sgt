#ifndef _KEYBOARD_MANIPULATER_H_
#define _KEYBOARD_MANIPULATER_H_

class KeyboardManipulater : public SGVTool
{
public:
    KeyboardManipulater ( SGVTools* tools );

    virtual void keydown ( int key );
    virtual void keyup ( int key );
};

#endif // _KEYBOARD_MANIPULATER_H_
