#ifndef _NODEDUMPER_H_
#define _NODEDUMPER_H_

#include "childvisitor.h"
#include <string>
#include <sstream>
using namespace std;

class AGEF_EXPORT NodeDumper : public ChildVisitor
{
public:
    virtual void apply ( SGNode& node );
    virtual void apply ( LayerNode& node );
    virtual void apply ( RectangleNodef& node );
    virtual void apply ( TransformNode& node );
    virtual void apply ( ArrayNode& node );
    virtual void apply ( LODNode& node );
    virtual void apply ( PickableGroup& node );
    virtual void apply ( KdTreeNode& node );
    virtual void apply ( MeshNode& node );
    virtual void apply ( FontNode& node );
    virtual void apply ( TextNode& node );
    virtual void apply ( GroupNode& node );
    virtual void apply ( SwitchNode& node );
    virtual void apply ( LineNodef& node );
    virtual void apply ( AttrSet& node );
    void operator() ( SGNode* node )
    {
        _dump.str("");
        _ident = "";
        node->accept ( *this );
    }
    string dumpstring () { return _dump.str(); }
private:
    stringstream _dump;
    string _ident;
};

inline void NodeDumper::apply ( SGNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "sgnode " <<
        "bbox.min=(" << node.getBBox().min().x() << ", " << node.getBBox().min().y() << ", " << node.getBBox().min().z() << 
        ") bbox.max=(" << node.getBBox().max().x() << ", " << node.getBBox().max().y() << ", " << node.getBBox().max().z() << ")" << endl;
    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( LayerNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "LayerNode: name=" << node.name() << " visible=" << node.isVisible() <<
        " bbox.min=(" << node.getBBox().min().x() << ", " << node.getBBox().min().y() << ", " << node.getBBox().min().z() << 
        ") bbox.max=(" << node.getBBox().max().x() << ", " << node.getBBox().max().y() << ", " << node.getBBox().max().z() << ") layerfgcolor=" <<
        node.getFgColor().toString() << ") layerbgcolor=" << node.getBgColor().toString() << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( PickableGroup& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "PickableGroup: name=" << node.name() << 
        " bbox.min=(" << node.getBBox().min().x() << ", " << node.getBBox().min().y() << ", " << node.getBBox().min().z() << 
        ") bbox.max=(" << node.getBBox().max().x() << ", " << node.getBBox().max().y() << ", " << node.getBBox().max().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( RectangleNodef& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    float x1 = node.getBBox().min().x();
    float y1 = node.getBBox().min().y();
    float z1 = node.getBBox().min().z();
    float x2 = node.getBBox().max().x();
    float y2 = node.getBBox().max().y();
    float z2 = node.getBBox().max().z();

    _dump << _ident << "RectangleNodef: w=" << node.w() << " h=" << node.h() <<
        " bbox.min=(" << x1 << ", " << y1 << ", " << z1 << 
        ") bbox.max=(" << x2 << ", " << y2 << ", " << z2 << ") ";

    AttrSet* pAttrSet = node.getAttrSet();
    if ( pAttrSet )
    {
        _dump << " renderOrder=" << pAttrSet->getRenderOrder();

        if ( pAttrSet->getFgColor() )
            _dump << " bordercolor=" << pAttrSet->getFgColor()->toString(); 
        if ( pAttrSet->getBgColor() )
            _dump << " fillcolor=" << pAttrSet->getBgColor()->toString();
    }
    _dump << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( TransformNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    float dx = node.getMatrix().dx();
    float dy = node.getMatrix().dy();
    float dz = node.getMatrix().dz();

    _dump << _ident << "TransformNode tx=" << dx << 
        " ty=" << dy <<
        " tz=" << dz <<
        " bbox.min=(" << node.getBBox().min().x() << ", " << node.getBBox().min().y() << ", " << node.getBBox().min().z() << 
        ") bbox.max=(" << node.getBBox().max().x() << ", " << node.getBBox().max().y() << ", " << node.getBBox().max().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( ArrayNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "ArrayNode" <<
        " bbox.min=(" << node.getBBox().min().x() << ", " << node.getBBox().min().y() << ", " << node.getBBox().min().z() << 
        ") bbox.max=(" << node.getBBox().max().x() << ", " << node.getBBox().max().y() << ", " << node.getBBox().max().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( LODNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "LODNode" <<
        " bbox.min=(" << node.getBBox().min().x() << ", " << node.getBBox().min().y() << ", " << node.getBBox().min().z() << 
        ") bbox.max=(" << node.getBBox().max().x() << ", " << node.getBBox().max().y() << ", " << node.getBBox().max().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( KdTreeNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "KdTreeNode" <<
        " bbox.min=(" << node.getBBox().min().x() << ", " << node.getBBox().min().y() << ", " << node.getBBox().min().z() << 
        ") bbox.max=(" << node.getBBox().max().x() << ", " << node.getBBox().max().y() << ", " << node.getBBox().max().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( MeshNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "MeshNode " <<
        "bbox.min=(" << node.getBBox().min().x() << ", " << node.getBBox().min().y() << ", " << node.getBBox().min().z() << 
        ") bbox.max=(" << node.getBBox().max().x() << ", " << node.getBBox().max().y() << ", " << node.getBBox().max().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( FontNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "FontNode family=" << node.family() <<
        " pointsize=" << node.size() <<
        " bbox.min=(" << node.getBBox().min().x() << ", " << node.getBBox().min().y() << ", " << node.getBBox().min().z() << 
        ") bbox.max=(" << node.getBBox().max().x() << ", " << node.getBBox().max().y() << ", " << node.getBBox().max().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( TextNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "TextNode string=" << node.text() <<
        " bbox.min=(" << node.getBBox().min().x() << ", " << node.getBBox().min().y() << ", " << node.getBBox().min().z() << 
        ") bbox.max=(" << node.getBBox().max().x() << ", " << node.getBBox().max().y() << ", " << node.getBBox().max().z() << ") textcolor=" <<
        node.getAttrSet()->getFgColor()->toString() << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( GroupNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "GroupNode name=" << node.name() <<
        " bbox.min=(" << node.getBBox().min().x() << ", " << node.getBBox().min().y() << ", " << node.getBBox().min().z() << 
        ") bbox.max=(" << node.getBBox().max().x() << ", " << node.getBBox().max().y() << ", " << node.getBBox().max().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( SwitchNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "SwitchNode isVisible=" << node.isVisible() <<
        " bbox.min=(" << node.getBBox().min().x() << ", " << node.getBBox().min().y() << ", " << node.getBBox().min().z() << 
        ") bbox.max=(" << node.getBBox().max().x() << ", " << node.getBBox().max().y() << ", " << node.getBBox().max().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( LineNodef& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "LineNodef: x1=" << node.x1() << " y1=" << node.y1() << " x2=" << node.x2() << " y2=" << node.y2() <<
        " bbox.min=(" << node.getBBox().min().x() << ", " << node.getBBox().min().y() << ", " << node.getBBox().min().z() << 
        ") bbox.max=(" << node.getBBox().max().x() << ", " << node.getBBox().max().y() << ", " << node.getBBox().max().z() << ") ";

    AttrSet* pAttrSet = node.getAttrSet();
    if ( pAttrSet )
    {
        _dump << " renderOrder=" << pAttrSet->getRenderOrder();

        if ( pAttrSet->getFgColor() )
            _dump << " fgrcolor=" << pAttrSet->getFgColor()->toString(); 
        if ( pAttrSet->getBgColor() )
            _dump << " bgcolor=" << pAttrSet->getBgColor()->toString();
    }
    _dump << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( AttrSet& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "AttrSet: renderorder=" << node.getRenderOrder() << endl;
    if ( node.getFgColor() )
        _dump << " bordercolor=" << node.getFgColor()->toString(); 
    if ( node.getBgColor() )
        _dump << " fillcolor=" << node.getBgColor()->toString();

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

#endif
