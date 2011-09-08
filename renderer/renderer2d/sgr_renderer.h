#ifndef _SGR_RENDERER_H_
#define _SGR_RENDERER_H_

class StateSet;
class LC;
class Renderer
{
public:
    static void render ( StateSet* root, LC* lc );
};


#endif // _SGR_RENDERER_H_
