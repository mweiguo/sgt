#include "sgr_drawablenode.h"
#include <string>
#include <sgr_fontmetric.h>
#include "sgr_parentfinder.h"

#include "sgr_includes.h"

using namespace std;

#include "sgr_textnode.h"
namespace SGR
{

const int TextNode::TXTSIZEMODE_SCENE   = 1;
const int TextNode::TXTSIZEMODE_SCREEN  = 2;

TextNode::TextNode ()
{
    //_fontnode = &FontNode::defaultNode(); 
    _sizeMode = TXTSIZEMODE_SCENE;
    _size.xy ( 1, 1 );
    _renderScale = 1;
    _anchorPos = vec2f(0,0);
    _isDrawBackground = false;
}

TextNode::TextNode ( const string& content ) : _text(content) 
{ 
    //_fontnode = &FontNode::defaultNode(); 
    _sizeMode = TXTSIZEMODE_SCENE;
    _size.xy ( 1, 1 );
    _renderScale = 1;
    _anchorPos = vec2f(0,0);
    _isDrawBackground = false;
}

TextNode::TextNode ( const TextNode& rhs ) : DrawableNode ( rhs )
{
    _text         = rhs._text;
    _anchor       = rhs._anchor;
    _alignFlag    = rhs._alignFlag;
    //_fontnode     = rhs._fontnode;
    _sizeMode     = rhs._sizeMode;
    _size         = rhs._size;
    _renderScale  = rhs._renderScale;
    _anchorPos    = rhs._anchorPos;
    _isDrawBackground = rhs._isDrawBackground;
}

TextNode::~TextNode ()
{
}

SGNode* TextNode::clone ()
{
    return new TextNode(*this);
}

void TextNode::updateBBox( const mat4f& mat, bool force )
{
    if ( force || isBBoxDirty()  )
    {
        if ( NULL==getAttrSet() || NULL==getAttrSet()->getFont() )
            return;
        _anchorPos = mat * vec4f ( 0, 0, 0, 1 );

        //    qDebug ("TextNode::updateBBox pass");

        if ( TXTSIZEMODE_SCENE == _sizeMode )
        {
            vec3f scenesize = FontMetric::getBBox ( *(getAttrSet()->getFont()), _text.c_str() ).dimension();
            _renderScale = _size.x() / fabs(scenesize.x());
            //	_anchorPos = mat4f::scale_matrix(1.0f/_renderScale, 1.0f/_renderScale, 1.0f/_renderScale) * mat * vec4f ( 0, 0, 0, 1 );

            //	qDebug ("TextNode::updateBBox scene mode %f", _renderScale);
            //     _size = FontMetric::getInst().getBBox ( *getAttrSet()->getFont(), _text ).dimension();
            _bb.init ( vec3f(0.f, 0.f, 0.f) );
            _bb.expandby ( _size );

            if ( isAnchorHCenter () )
                _bb.translate ( vec3f(-_size.x()/2.f, 0.f, 0.f ) );
            else if ( isAnchorRight () )
                _bb.translate ( vec3f(-_size.x(), 0.f, 0.f) );

            if ( isAnchorVCenter () )
                _bb.translate ( vec3f(0, -_size.y()/2.f, 0.f) );
            else if ( isAnchorTop () )
                _bb.translate ( vec3f(0, -_size.y(), 0.f) );

            vec4f min = mat * vec4f ( _bb.minvec() );
            vec4f max = mat * vec4f ( _bb.maxvec() );

            _bb.setminmax ( min.xyz(), max.xyz() );

        }
        else if ( TXTSIZEMODE_SCREEN == _sizeMode )
        {
            // do nothing, because we assume text's bbox is invalid in screen mode.
        }

        for ( iterator pp=begin(); pp!=end(); ++pp )
        {
            if ( force || (*pp)->isBBoxDirty () )
                (*pp)->updateBBox(mat, force);
            _bb = _bb.unionbox ( (*pp)->getBBox() );
        }
        _isBBoxDirty = false;
    }
}

void TextNode::computeBBox( const mat4f* mat ) const
{
    if ( false == _isBBoxDirty )
        return;

    mat4f tmat;
    if ( 0 == mat )
    {
        mat = &tmat;
        tmat = getParentTranMatrix ();
    }
    DrawableNode::computeBBox ( mat );

    BBox bb;
    if ( NULL==getAttrSet() || NULL==getAttrSet()->getFont() )
        return;
    _anchorPos = (*mat) * vec4f ( 0, 0, 0, 1 );

    //    qDebug ("TextNode::updateBBox pass");

    if ( TXTSIZEMODE_SCENE == _sizeMode )
    {
        vec3f scenesize = FontMetric::getBBox ( *(getAttrSet()->getFont()), _text.c_str() ).dimension();
        _renderScale = _size.x() / fabs(scenesize.x());
        //	_anchorPos = mat4f::scale_matrix(1.0f/_renderScale, 1.0f/_renderScale, 1.0f/_renderScale) * mat * vec4f ( 0, 0, 0, 1 );

        //	qDebug ("TextNode::updateBBox scene mode %f", _renderScale);
        //     _size = FontMetric::getInst().getBBox ( *getAttrSet()->getFont(), _text ).dimension();
        bb.init ( vec3f(0.f, 0.f, 0.f) );
        bb.expandby ( _size );

        if ( isAnchorHCenter () )
            bb.translate ( vec3f(-_size.x()/2.f, 0.f, 0.f ) );
        else if ( isAnchorRight () )
            bb.translate ( vec3f(-_size.x(), 0.f, 0.f) );

        if ( isAnchorVCenter () )
            bb.translate ( vec3f(0, -_size.y()/2.f, 0.f) );
        else if ( isAnchorTop () )
            bb.translate ( vec3f(0, -_size.y(), 0.f) );

        vec4f min = (*mat) * vec4f ( bb.minvec() );
        vec4f max = (*mat) * vec4f ( bb.maxvec() );

        bb.setminmax ( min.xyz(), max.xyz() );

    }
    else if ( TXTSIZEMODE_SCREEN == _sizeMode )
    {
        // do nothing, because we assume text's bbox is invalid in screen mode.
    }

    _bb = _bb.unionbox ( bb );
}

void TextNode::text ( const string& content )
{
    _text = content; 
    if ( TXTSIZEMODE_SCENE==getSizeMode() && getAttrSet() && getAttrSet()->getFont() )
    {
	vec3f scenesize = FontMetric::getBBox ( *(getAttrSet()->getFont()), _text.c_str() ).dimension();
	_renderScale = _size.x() / fabs(scenesize.x());
    }

    //_size = FontMetric::getInst().getBBox ( *_fontnode, _text ).dimension();
    setBBoxDirty ();
    //setParentBBoxDirty ( true );

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

bool TextNode::isAnchorLeft () const
{
    return ( (AnchorLEFT & _anchor) != 0 ); 
}

bool TextNode::isAnchorHCenter () const
{ 
    return ( (AnchorHCENTER & _anchor) != 0 ); 
}

bool TextNode::isAnchorRight () const
{ 
    return ( (AnchorRIGHT & _anchor) != 0 ); 
}

bool TextNode::isAnchorTop () const
{ 
    return ( (AnchorTOP & _anchor) != 0 ); 
}

bool TextNode::isAnchorVCenter () const
{ 
    return ( (AnchorVCENTER & _anchor) != 0 ); 
}

bool TextNode::isAnchorBottom () const
{ 
    return ( (AnchorBOTTOM & _anchor) != 0 ); 
}

short TextNode::anchorValue () const
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

void TextNode::setSizeMode ( int sizemode )
{
    _sizeMode = sizemode;
}

int TextNode::getSizeMode ()
{
    return _sizeMode;
}

void TextNode::setSize ( float x, float y )
{
    _size.xy ( x, y );
}

const vec2f& TextNode::getSize () const
{
    return _size;
}

void TextNode::width ( float w )
{
    _size.x ( w );
}

void TextNode::height ( float h )
{
    _size.y ( h );
}

float TextNode::width() const
{
    return _size.x();
}

float TextNode::height() const
{
    return _size.y();
}

float TextNode::renderScale () const
{
    return _renderScale;
}

vec2f TextNode::anchorPos () const
{
    return _anchorPos;
}

void TextNode::setDrawBackground( bool b )
{
    _isDrawBackground = b;
}

bool TextNode::isDrawBackground() const
{
    return _isDrawBackground;
}

// bounding box display control
//void showBoundingBox ( bool isShow );
//FontNode* TextNode::fontnode()
//{
//    return _fontnode; 
//}

//void TextNode::fontnode( FontNode* p ) 
//{ 
//    _fontnode = p; 
//}

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
}
