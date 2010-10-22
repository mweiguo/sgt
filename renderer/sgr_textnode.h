#ifndef _TEXTNODE_H_
#define _TEXTNODE_H_

//#include "sgr_drawablenode.h"
//#include "sgr_fontnode.h"
//#include "sgr_layernode.h"
//#include "sgr_scenenode.h"
//#include "sgr_kdtreenode.h"
//#include <string>
//#include <sgr_fontmetric.h>
//#include <sgr_vec4.h>
//#include "fbcolor.h"
//#include "sgr_parentfinder.h"
//
//using namespace std;
#include <sgr_vec4.h>
#include <sgr_vec2.h>
#include "sgr_drawablenode.h"
//#include "fbcolor.h"
namespace SGR
{

class SGR_DLL TextNode : public DrawableNode
{
public:
    static const int TXTSIZEMODE_SCENE;
    static const int TXTSIZEMODE_SCREEN;
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
    //FontNode* fontnode();
    //void fontnode( FontNode* p );

    
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    //void useLayerColor ();
    //void setTextColor ( GColor color );
    //GColor getTextColor ();

    void setSizeMode ( int );
    int getSizeMode ();

    // the coordinate frame is depend on size mode, if sizemode = screenmode, the coord is in screen frame
    // if sizemode = scenemode, the coord is in scene frame
    void setSize ( float x, float y );
    const vec2f& getSize () const ;
    void width ( float w );
    void height ( float h );
    float width() const;
    float height() const;

    float renderScale () const;
    vec2f anchorPos () const;
private:
    string _text;
    short _anchor;
    short _alignFlag;
    //FontNode* _fontnode;

    vec2f _size;
    int _sizeMode;

    float _renderScale;
    vec2f _anchorPos;
//private:
//    FgBgColorComponent _color;
};

}
//typedef NodeMgr<TextNode>        TextNodeMgr;
#endif
