#ifndef _FONTNODE_H_
#define _FONTNODE_H_

#include "sgr_sgnode.h"

#include <string>
using namespace std;
#include "sgr_global.h"
namespace SGR
{

class SGR_DLL FontNode : public SGNode
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
	_family(family), _pointSize(pointSize), _style(style)
    {
    }
    FontNode ( const FontNode& rhs ) : SGNode(rhs)
    {
        _family     = rhs._family;
        _pointSize  = rhs._pointSize;
        _style      = rhs._style;
        //_color      = rhs._color;
    }

    virtual SGNode* clone ()
    {
        return new FontNode(*this);
    }

    // set font interface
    void setFont ( const string& family="Courier New", int pointSize=12, bool isItalic=false );

    void family ( const string& f ) { _family = f; }
    const string& family () const { return _family; }

    void pointSize ( int psize ) { _pointSize = psize; }
    int pointSize () const { return _pointSize; }

    void weight ( int w ) { _weight = w; }
    int weight () { return _weight; }

    void isItalic ( bool isItalic )
    {
	if (isItalic)
	    _style &= ITALIC; 
	else
	    _style &= ~ITALIC; 
    }
    bool italic () const { return (_style & ITALIC); }

    void defName (const string& defName) { _defName = defName; }
    const string& defName () const { return _defName; }

    string getDesc ()
    {
        stringstream ss;
        ss << _family << '|' << _pointSize << "|" << _weight << "|" << _style;
        return ss.str();
    }
    void setDesc ( const string& /*desc*/ )
    {
    }

    
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
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
}
#endif
