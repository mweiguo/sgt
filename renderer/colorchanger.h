#ifndef _COLORCHANGER_H_
#define _COLORCHANGER_H_

#include "nodevisitor.h"
#include "layernode.h"
#include "rectitem.h"
#include "linenode.h"
#include <string>
using namespace std;
template < class T >
class AGEF_EXPORT FillColorChanger: public NodeVisitor 
{
public:
    FillColorChanger ( unsigned color, bool isByGroup ) : _color(color), _isByGroup(isByGroup) {}
    FillColorChanger ( const string& color, bool isByGroup ) : _color(color), _isByGroup(isByGroup) {}
    virtual void apply ( LayerNode& /*node*/ );
    virtual void apply ( RectangleNodef& /*node*/ );
private:
    T _color;
    bool _isByGroup;
};

template < class T >
void FillColorChanger<T>::apply ( LayerNode& node )
{
    node.setColor ( _color );
}

template < class T >
void FillColorChanger<T>::apply ( RectangleNodef& node )
{
    if ( _isByGroup )
        node.useLayerFillColor();
    else
        node.setFillColor ( _color );
}

//template <>
//void FillColorChanger<unsigned int>::apply ( RectangleNodef& node )
//{
//    if ( _isByGroup )
//        node.useLayerFillColor();
//    else
//        node.setFillColor ( _color );
//}

template < class T >
class AGEF_EXPORT BorderColorChanger: public NodeVisitor 
{
public:
    BorderColorChanger ( unsigned int color, bool isByGroup ) : _color(color), _isByGroup(isByGroup) {}
    BorderColorChanger ( const string& color, bool isByGroup ) : _color(color), _isByGroup(isByGroup) {}
    virtual void apply ( LineNodef& /*node*/ );
    virtual void apply ( RectangleNodef& /*node*/ );
private:
    T _color;
    bool _isByGroup;
};

template < class T >
void BorderColorChanger<T>::apply ( LineNodef& node )
{
    if ( _isByGroup )
        node.useLayerColor ();
    else
        node.setLineColor ( _color );
}

//template <>
//void BorderColorChanger<unsigned int>::apply ( LineNodef& node )
//{
//    if ( _isByGroup )
//        node.useLayerColor ();
//    else
//        node.setLineColor ( _color );
//}

template < class T >
void BorderColorChanger<T>::apply ( RectangleNodef& node )
{
    if ( _isByGroup )
        node.useLayerBorderColor ();
    else
        node.setBorderColor ( _color );
}

//template <>
//void BorderColorChanger<unsigned int>::apply ( RectangleNodef& node )
//{
//    if ( _isByGroup )
//        node.useLayerBorderColor ();
//    else
//        node.setBorderColor ( _color );
//}

class AGEF_EXPORT FrontColorChanger: public NodeVisitor 
{
public:
    FrontColorChanger ( unsigned int color, bool isByGroup ) : _color(color), _isByGroup(isByGroup) {}
    virtual void apply ( const TextNode& /*node*/ ) {}
    virtual void apply ( TextNode& /*node*/ );
private:
    unsigned int _color;
    bool _isByGroup;
};

void FrontColorChanger::apply ( TextNode& node )
{
    if ( _isByGroup )
        node.useLayerColor ();
    else
        node.setTextColor ( _color );
}



#endif // _COLORCHANGER_H_
