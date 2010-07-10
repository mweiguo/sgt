#ifndef _ATTRSET_H_
#define _ATTRSET_H_

class AttrSet
{
public:
    void setFgColor ( GColor* color ) { _fgColor = color; }
    GColor* getFgColor () { return _fgColor; }

    void setBgColor ( GColor* color ) { _bgColor = color; }
    GColor* getBgColor () { return _bgColor; }

private:
    GColor* _fgColor;
    GColor* _bgColor;
};



#endif // _ATTRSET_H_
