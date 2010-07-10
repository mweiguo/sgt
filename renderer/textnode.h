#ifndef _TEXTNODE_H_
#define _TEXTNODE_H_

//#include "drawablenode.h"
//#include "fontnode.h"
//#include "layernode.h"
//#include "meshnode.h"
//#include "kdtreenode.h"
//#include <string>
//#include <fontmetric.h>
//#include <vec4.h>
//#include "fbcolor.h"
//#include "parentfinder.h"
//
//using namespace std;
#include <vec4.h>
#include "fbcolor.h"

class AGEF_EXPORT TextNode : public DrawableNode
{
public:
    enum AnchorFlag
    {
        AnchorLEFT    = 1,
        AnchorTOP     = 2,
        AnchorRIGHT   = 4,
        AnchorBOTTOM  = 8,
        AnchorHCENTER  = 16,
        AnchorVCENTER  = 32,
        AnchorCENTER  = AnchorHCENTER | AnchorVCENTER
    };
    enum AlignFlag
    {
        AlignLeft     = 0x1, 
        AlignRight    = 0x2,
        AlignHCenter  = 0x4,
        AlignJustify  = 0x8,
        AlignTop      = 0x20,
        AlignBottom   = 0x40,
        AlignVCenter  = 0x80,
        AlignCenter   = AlignHCenter | AlignVCenter
    };
    TextNode ();
    TextNode ( const string& content );
    TextNode ( const TextNode& rhs );
    virtual ~TextNode ();

    virtual void updateBBox( const mat4f& mat=mat4f() );
    void text ( const string& content );
    const string& text ();

    // anchor interface
    // anchor have 9 options
    //  1     2     3
    //  4     5     6
    //  7     8     9
    void setAnchor ( int anchorFlag );
    bool isAnchorLeft ();
    bool isAnchorHCenter ();
    bool isAnchorRight ();
    bool isAnchorTop ();
    bool isAnchorVCenter ();
    bool isAnchorBottom ();
    short anchorValue ();
    void anchorValue ( short v );

    // set align
    void setAlignFlag ( short alignFlag );
    short alignFlag ();

    // bounding box display control
    //void showBoundingBox ( bool isShow );
    FontNode* fontnode();
    void fontnode( FontNode* p );

    virtual void accept ( NodeVisitor& pvisitor ) const { pvisitor.apply ( *this ); }
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    void useLayerColor ();
    void setTextColor ( GColor color );
    GColor getTextColor ();

private:
    string _text;
    short _anchor;
    short _alignFlag;
    FontNode* _fontnode;

    vec3f _dimension;
private:
    FgBgColorComponent _color;
};

//typedef NodeMgr<TextNode>        TextNodeMgr;
#endif
