#ifndef _ATTRSET_H_
#define _ATTRSET_H_
#include "sgr_sgnode.h"
namespace SGR
{

class GColor;
class AttrSet : public SGNode
{
public:
    AttrSet (int renderOrder=0/*this parameter is obsolete */) : _fgColor(0), _bgColor(0), _font(0), _lineWidth(0), _renderOrder(renderOrder), refcnt(1) {}
    void incRef () { ++refcnt; }
    void decRef ()
    {
        if ( 0 >= --refcnt )
            delete this;
    }

    void setFgColor ( GColor* color ) { _fgColor = color; }
    inline GColor* getFgColor () { return _fgColor; }
    inline const GColor* getFgColor () const { return _fgColor; }

    void setBgColor ( GColor* color ) { _bgColor = color; }
    inline GColor* getBgColor () { return _bgColor; }
    inline const GColor* getBgColor () const { return _bgColor; }

    void setFont ( FontNode* font ) { _font = font; }
    inline FontNode* getFont () { return _font; }
    inline const FontNode* getFont () const { return _font; }

    void setRenderOrder ( int order ) { _renderOrder = order; }
    inline int getRenderOrder () const { return _renderOrder; }

    void setLineWidth ( int width ) { _lineWidth = width; }
    inline int getLineWidth () const { return _lineWidth; }

    inline int getRefCnt () const { return refcnt; }

private:
    GColor* _fgColor;
    GColor* _bgColor;
    FontNode* _font;
    int _lineWidth;
    // -1 is for internal use
    // >0 is for render use
    int _renderOrder;

    unsigned int refcnt;
};


}
#endif // _ATTRSET_H_
