#ifndef _NODEDUMPER_H_
#define _NODEDUMPER_H_

#include "sgr_childvisitor.h"
#include <string>
#include <sstream>
using namespace std;
namespace SGR
{

class SGR_DLL NodeDumper : public ChildVisitor
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
    virtual void apply ( SceneNode& node );
    virtual void apply ( FontNode& node );
    virtual void apply ( TextNode& node );
    virtual void apply ( GroupNode& node );
    virtual void apply ( SwitchNode& node );
    virtual void apply ( LineNodef& node );
    virtual void apply ( AttrSet& node );
    virtual void apply ( MeshNode3f& node );
    virtual void apply ( MeshLineNode& node );
    virtual void apply ( PolylineNode2Df& node );
    virtual void apply ( PolyNode2Df& node );
    virtual void apply ( PointNode& node );
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
    _dump << _ident << "SGNode: id=" << node.getID() << " " <<
        "bbox.min=(" << node.getBBox().minvec().x() << ", " << node.getBBox().minvec().y() << ", " << node.getBBox().minvec().z() << 
        ") bbox.max=(" << node.getBBox().maxvec().x() << ", " << node.getBBox().maxvec().y() << ", " << node.getBBox().maxvec().z() << ")" << endl;
    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( LayerNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "LayerNode: id=" << node.getID() << " name=" << node.name() << " visible=" << node.isVisible() <<
        " bbox.min=(" << node.getBBox().minvec().x() << ", " << node.getBBox().minvec().y() << ", " << node.getBBox().minvec().z() << 
        ") bbox.max=(" << node.getBBox().maxvec().x() << ", " << node.getBBox().maxvec().y() << ", " << node.getBBox().maxvec().z() << ") layerfgcolor=" <<
        node.getFgColor().toString() << ") layerbgcolor=" << node.getBgColor().toString() << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( PickableGroup& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "PickableGroup: id=" << node.getID() << " name=" << node.name() << 
        " bbox.min=(" << node.getBBox().minvec().x() << ", " << node.getBBox().minvec().y() << ", " << node.getBBox().minvec().z() << 
        ") bbox.max=(" << node.getBBox().maxvec().x() << ", " << node.getBBox().maxvec().y() << ", " << node.getBBox().maxvec().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( RectangleNodef& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    float x1 = node.getBBox().minvec().x();
    float y1 = node.getBBox().minvec().y();
    float z1 = node.getBBox().minvec().z();
    float x2 = node.getBBox().maxvec().x();
    float y2 = node.getBBox().maxvec().y();
    float z2 = node.getBBox().maxvec().z();

    _dump << _ident << "RectangleNodef: id=" << node.getID() << " w=" << node.w() << " h=" << node.h() <<
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

    _dump << _ident << "TransformNode: id=" << node.getID() << " tx=" << dx << 
        " ty=" << dy <<
        " tz=" << dz <<
        " bbox.min=(" << node.getBBox().minvec().x() << ", " << node.getBBox().minvec().y() << ", " << node.getBBox().minvec().z() << 
        ") bbox.max=(" << node.getBBox().maxvec().x() << ", " << node.getBBox().maxvec().y() << ", " << node.getBBox().maxvec().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( ArrayNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "ArrayNode: id=" << node.getID() << 
        " bbox.min=(" << node.getBBox().minvec().x() << ", " << node.getBBox().minvec().y() << ", " << node.getBBox().minvec().z() << 
        ") bbox.max=(" << node.getBBox().maxvec().x() << ", " << node.getBBox().maxvec().y() << ", " << node.getBBox().maxvec().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( LODNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "LODNode: id=" << node.getID() <<
        " bbox.min=(" << node.getBBox().minvec().x() << ", " << node.getBBox().minvec().y() << ", " << node.getBBox().minvec().z() << 
        ") bbox.max=(" << node.getBBox().maxvec().x() << ", " << node.getBBox().maxvec().y() << ", " << node.getBBox().maxvec().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( KdTreeNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "KdTreeNode: id=" << node.getID() <<
        " bbox.min=(" << node.getBBox().minvec().x() << ", " << node.getBBox().minvec().y() << ", " << node.getBBox().minvec().z() << 
        ") bbox.max=(" << node.getBBox().maxvec().x() << ", " << node.getBBox().maxvec().y() << ", " << node.getBBox().maxvec().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( SceneNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "SceneNode: id=" << node.getID() <<
        " bbox.min=(" << node.getBBox().minvec().x() << ", " << node.getBBox().minvec().y() << ", " << node.getBBox().minvec().z() << 
        ") bbox.max=(" << node.getBBox().maxvec().x() << ", " << node.getBBox().maxvec().y() << ", " << node.getBBox().maxvec().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( FontNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "FontNode: id=" << node.getID() << " family=" << node.family() <<
        " pointsize=" << node.size() <<
        " bbox.min=(" << node.getBBox().minvec().x() << ", " << node.getBBox().minvec().y() << ", " << node.getBBox().minvec().z() << 
        ") bbox.max=(" << node.getBBox().maxvec().x() << ", " << node.getBBox().maxvec().y() << ", " << node.getBBox().maxvec().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( TextNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "TextNode: id=" << node.getID() << " string=" << node.text() <<
        " bbox.min=(" << node.getBBox().minvec().x() << ", " << node.getBBox().minvec().y() << ", " << node.getBBox().minvec().z() << 
        ") bbox.max=(" << node.getBBox().maxvec().x() << ", " << node.getBBox().maxvec().y() << ", " << node.getBBox().maxvec().z() << ") textcolor=" <<
        node.getAttrSet()->getFgColor()->toString() << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( GroupNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "GroupNode: id=" << node.getID() << " name=" << node.name() <<
        " bbox.min=(" << node.getBBox().minvec().x() << ", " << node.getBBox().minvec().y() << ", " << node.getBBox().minvec().z() << 
        ") bbox.max=(" << node.getBBox().maxvec().x() << ", " << node.getBBox().maxvec().y() << ", " << node.getBBox().maxvec().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( SwitchNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "SwitchNode: id=" << node.getID() << " isVisible=" << node.isVisible() <<
        " bbox.min=(" << node.getBBox().minvec().x() << ", " << node.getBBox().minvec().y() << ", " << node.getBBox().minvec().z() << 
        ") bbox.max=(" << node.getBBox().maxvec().x() << ", " << node.getBBox().maxvec().y() << ", " << node.getBBox().maxvec().z() << ")" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( LineNodef& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "LineNodef: id=" << node.getID() << " x1=" << node.x1() << " y1=" << node.y1() << " x2=" << node.x2() << " y2=" << node.y2() <<
        " bbox.min=(" << node.getBBox().minvec().x() << ", " << node.getBBox().minvec().y() << ", " << node.getBBox().minvec().z() << 
        ") bbox.max=(" << node.getBBox().maxvec().x() << ", " << node.getBBox().maxvec().y() << ", " << node.getBBox().maxvec().z() << ") ";

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
    _dump << _ident << "AttrSet: id=" << node.getID() << " renderorder=" << node.getRenderOrder() << endl;
    if ( node.getFgColor() )
        _dump << " bordercolor=" << node.getFgColor()->toString(); 
    if ( node.getBgColor() )
        _dump << " fillcolor=" << node.getBgColor()->toString();

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( MeshNode3f& node )
{
    stringstream ss1;
    MeshNode3f::coorditerator pp, end=node.coordend();
    for ( pp=node.coordbegin(); pp!=end; ++pp )
        ss1 << pp->x() << " " << pp->y() << " " << pp->z() << " ";

    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "<mesh: id=" << node.getID() << " coords='" << ss1.str() << "'>" << endl;


    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( MeshLineNode& node )
{
    stringstream ss1;
    MeshLineNode::pntiterator pp, end=node.pntend();
    for ( pp=node.pntbegin(); pp!=end; ++pp )
        ss1 << *pp << " ";

    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "<meshline: id=" << node.getID() << " type='" << node.type() << "'>" << ss1.str() << "</meshline>" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( PolylineNode2Df& node )
{
    stringstream ss1;
    PolylineNode2Df::pointiterator pp, end=node.pointend();
    for ( pp=node.pointbegin(); pp!=end; ++pp )
        ss1 << pp->x() << " " << pp->y() << " ";

    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "<polyline: id=" << node.getID() << ">" << ss1.str() << "</polyline>" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( PolyNode2Df& node )
{
    stringstream ss1;
    PolyNode2Df::pointiterator pp, end=node.pointend();
    for ( pp=node.pointbegin(); pp!=end; ++pp )
        ss1 << pp->x() << " " << pp->y() << " ";

    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "<poly: id=" << node.getID() << ">" << ss1.str() << "</poly>" << endl;

    ChildVisitor::apply ( node );
    _ident.erase ( 0, 2 );
}

inline void NodeDumper::apply ( PointNode& node )
{
    stringstream ss;
    _ident.insert ( 0, "  " );
    _dump << _ident << "PointNode: id=" << node.getID() << " x=" << node.x() << " y=" << node.y() << " size=" << node.pointSize() << 
        " bbox.min=(" << node.getBBox().minvec().x() << ", " << node.getBBox().minvec().y() << ", " << node.getBBox().minvec().z() << 
        ") bbox.max=(" << node.getBBox().maxvec().x() << ", " << node.getBBox().maxvec().y() << ", " << node.getBBox().maxvec().z() << ") " << endl;

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

}
#endif
