#ifndef _ATTRSET_H_
#define _ATTRSET_H_

class GColor;
class AttrSet : public SGNode
{
public:
    AttrSet (int renderOrder) : _fgColor(0), _bgColor(0), _renderOrder(renderOrder), refcnt(0) {}
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

    void setRenderOrder ( int order ) { _renderOrder = order; }
    inline int getRenderOrder () { return _renderOrder; }

    inline int getRefCnt () { return refcnt; }

private:
    GColor* _fgColor;
    GColor* _bgColor;
    int _renderOrder;

    unsigned int refcnt;
};



#endif // _ATTRSET_H_
