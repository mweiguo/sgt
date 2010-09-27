#ifndef _ATTRSET_H_
#define _ATTRSET_H_
namespace SGR
{

class GColor;
class AttrSet : public SGNode
{
public:
    AttrSet (int renderOrder=0/*this parameter is obsolete */) : _fgColor(0), _bgColor(0), _font(0), _renderOrder(renderOrder), refcnt(1) {}
    void incRef () { ++refcnt; }
    void decRef ()
    {
        if ( 0 >= --refcnt )
            delete this;
    }

    void setFgColor ( GColor* color ) { _fgColor = color; }
    inline GColor* getFgColor () { return _fgColor; }

    void setBgColor ( GColor* color ) { _bgColor = color; }
    inline GColor* getBgColor () { return _bgColor; }

    void setFont ( FontNode* font ) { _font = font; }
    inline FontNode* getFont () { return _font; }

    void setRenderOrder ( int order ) { _renderOrder = order; }
    inline int getRenderOrder () { return _renderOrder; }

    inline int getRefCnt () { return refcnt; }

private:
    GColor* _fgColor;
    GColor* _bgColor;
    FontNode* _font;
    // -1 is for internal use
    // >0 is for render use
    int _renderOrder;

    unsigned int refcnt;
};


}
#endif // _ATTRSET_H_
