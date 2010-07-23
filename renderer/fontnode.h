#ifndef _FONTNODE_H_
#define _FONTNODE_H_

#include "sgnode.h"

#include <string>
using namespace std;
#include "agef_global.h"

class AGEF_EXPORT FontNode : public SGNode
{
public:
    enum StyleFlag
    {
        NORMAL      = 0x0001,
        ITALIC      = 0x0002,
        UNDERSTRIKE = 0x0004
    };
public:
    static FontNode& defaultNode ()
    {
        static FontNode inst;
        return inst;
    }
    FontNode ( const string& family="Courier New", int pointSize=12, StyleFlag style=FontNode::NORMAL ) : 
    _family(family), _pointSize(pointSize)
    {
    }
    FontNode ( const FontNode& rhs ) : SGNode(rhs)
    {
        _family     = rhs._family;
        _pointSize  = rhs._pointSize;
        _style      = rhs._style;
        //_color      = rhs._color;
    }

    // set font interface
    void setFont ( const string& family="Courier New", int pointSize=12, bool isItalic=false );

    void family ( const string& f ) { _family = f; }
    const string& family () const { return _family; }

    void size ( int psize ) { _pointSize = psize; }
    int size () const { return _pointSize; }

    void weight ( int w ) { _weight = w; }
    int weight () { return _weight; }

    void isItalic ( bool isItalic ) { _style &= ITALIC; }
    bool italic () const { return (_style & ITALIC); }

    void defName (const string& defName) { _defName = defName; }
    const string& defName () const { return _defName; }

    //// set color
    //void color ( const string& color );
    //const string& color () { return _color; }
private:
    string _family;
    int _pointSize;
    int _weight;
    // NORMAL, ITALIC, UNDERSTRIKE
    int _style;
    string _defName;
    ////
    //string _color;
};

//typedef NodeMgr<FontNode>        FontNodeMgr;

#endif
