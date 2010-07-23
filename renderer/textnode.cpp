#include "drawablenode.h"
#include <string>
#include <fontmetric.h>
#include "parentfinder.h"

#include "sgl_includes.h"

using namespace std;

#include "textnode.h"

TextNode::TextNode ()
{
    _fontnode = &FontNode::defaultNode(); 
}

TextNode::TextNode ( const string& content ) : _text(content) 
{ 
    _fontnode = &FontNode::defaultNode(); 
}

TextNode::TextNode ( const TextNode& rhs ) : DrawableNode ( rhs )
{
    _text         = rhs._text;
    _anchor       = rhs._anchor;
    _alignFlag    = rhs._alignFlag;
    _fontnode     = rhs._fontnode;
}

TextNode::~TextNode ()
{
}

void TextNode::updateBBox( const mat4f& mat )
{
    if ( NULL == _fontnode )
        return;

    _bb.init ( vec3f(0.f, 0.f, 0.f) );
    _bb.expandby ( _dimension );

    if ( isAnchorHCenter () )
        _bb.translate ( vec3f(-_dimension.x()/2.f, 0.f, 0.f ) );
    else if ( isAnchorRight () )
        _bb.translate ( vec3f(-_dimension.x(), 0.f, 0.f) );

    if ( isAnchorVCenter () )
        _bb.translate ( vec3f(0, _dimension.y()/2.f, 0.f) );
    else if ( isAnchorTop () )
        _bb.translate ( vec3f(0, _dimension.y(), 0.f) );

    vec4f min = mat * vec4f ( _bb.min() );
    vec4f max = mat * vec4f ( _bb.max() );

    _bb.setminmax ( min.xyz(), max.xyz() );

    for ( iterator pp=begin(); pp!=end(); ++pp )
    {
        (*pp)->updateBBox();
        _bb = _bb.unionbox ( (*pp)->getBBox() );
    }
}

void TextNode::text ( const string& content )
{
    _text = content; 
    _dimension = FontMetric::getInst().getBBox ( *_fontnode, _text ).dimension();
}

const string& TextNode::text ()
{ 
    return _text; 
}

// anchor interface
// anchor have 9 options
//  1     2     3
//  4     5     6
//  7     8     9
void TextNode::setAnchor ( int anchorFlag ) 
{
    _anchor = anchorFlag; 
}

bool TextNode::isAnchorLeft () 
{
    return ( (AnchorLEFT & _anchor) != 0 ); 
}

bool TextNode::isAnchorHCenter () 
{ 
    return ( (AnchorHCENTER & _anchor) != 0 ); 
}

bool TextNode::isAnchorRight () 
{ 
    return ( (AnchorRIGHT & _anchor) != 0 ); 
}

bool TextNode::isAnchorTop () 
{ 
    return ( (AnchorTOP & _anchor) != 0 ); 
}

bool TextNode::isAnchorVCenter () 
{ 
    return ( (AnchorVCENTER & _anchor) != 0 ); 
}

bool TextNode::isAnchorBottom () 
{ 
    return ( (AnchorBOTTOM & _anchor) != 0 ); 
}

short TextNode::anchorValue ()
{
    int value = 0;
    if ( isAnchorTop() )
    {
        if ( isAnchorLeft() )
            value = 1;
        else if ( isAnchorHCenter() )
            value = 2;
        else if ( isAnchorRight() )
            value = 3;
    }
    else if ( isAnchorVCenter() )
    {
        if ( isAnchorLeft() )
            value = 4;
        else if ( isAnchorHCenter() )
            value = 5;
        else if ( isAnchorRight() )
            value = 6;
    }
    else if ( isAnchorBottom() )
    {
        if ( isAnchorLeft() )
            value = 7;
        else if ( isAnchorHCenter() )
            value = 8;
        else if ( isAnchorRight() )
            value = 9;
    }
    return value;
}

void TextNode::anchorValue ( short v )
{
    switch ( v)
    {
    case 1:
        setAnchor ( AnchorLEFT | AnchorTOP );
        break;
    case 2:
        setAnchor ( AnchorHCENTER | AnchorTOP );
        break;
    case 3:
        setAnchor ( AnchorRIGHT | AnchorTOP );
        break;
    case 4:
        setAnchor ( AnchorLEFT | AnchorVCENTER );
        break;
    case 5:
        setAnchor ( AnchorHCENTER | AnchorVCENTER );
        break;
    case 6:
        setAnchor ( AnchorRIGHT | AnchorVCENTER );
        break;
    case 7:
        setAnchor ( AnchorLEFT | AnchorBOTTOM );
        break;
    case 8:
        setAnchor ( AnchorHCENTER | AnchorBOTTOM );
        break;
    case 9:
        setAnchor ( AnchorRIGHT | AnchorBOTTOM );
        break;

    }
}

// set align
void TextNode::setAlignFlag ( short alignFlag ) 
{ 
    _alignFlag = alignFlag; 
}

short TextNode::alignFlag () 
{ 
    return _alignFlag; 
}

// bounding box display control
//void showBoundingBox ( bool isShow );
FontNode* TextNode::fontnode()
{
    return _fontnode; 
}

void TextNode::fontnode( FontNode* p ) 
{ 
    _fontnode = p; 
}

//void TextNode::useLayerColor ()
//{
//    ParentFinder<LayerNode> finder ( this );
//    if ( finder.target() )
//        _color.useLayerFgColor(finder.target()); 
//}
//void TextNode::setTextColor ( GColor color )
//{
//    _color.setFgColor(color);
//}
//
//GColor TextNode::getTextColor ()
//{ 
//    return _color.getFgColor(); 
//}
//
