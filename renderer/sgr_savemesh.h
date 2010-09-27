#ifndef _SAVEMESH_H_
#define _SAVEMESH_H_

#include "sgr_childvisitor.h"
#include <sstream>
#include <fstream>
using namespace std;
namespace SGR
{
class XMLCharEscape
{
public:
    static XMLCharEscape& getInst() 
    {
        static XMLCharEscape inst;
        return inst;
    }
    string transcode ( const string& str )
    {
        QString rst ( str.c_str() );
        for ( map < string, QRegExp  >::iterator pp=replaces.begin(); pp!=replaces.end(); ++pp )
            rst = rst.replace ( pp->second, pp->first.c_str() );
        return rst.toStdString();
    }
private:
    XMLCharEscape ()
    {
        replaces[ "&amp;" ] = QRegExp( "&(?!amp;)" );
        replaces[ "&quot;" ] = QRegExp( "\"" );
        replaces[ "&apos;" ] = QRegExp( "\'" );
        replaces[ "&lt;" ] = QRegExp( "<" );
        replaces[ "&gt;" ] = QRegExp( ">" );
    }
    map < string, QRegExp > replaces;
};

class SGR_DLL SaveMesh : public ChildVisitor
{
public:
    SaveMesh( const string& filename, SGNode* node );
    virtual void apply ( LayerNode& node );
    virtual void apply ( RectangleNodef& node );
    virtual void apply ( TransformNode& node );
    virtual void apply ( ArrayNode& node );
    virtual void apply ( LODNode& node );
    virtual void apply ( PickableGroup& node );
    virtual void apply ( KdTreeNode& node );
    virtual void apply ( SceneNode& node );
    virtual void apply ( FontNode& node );
    virtual void apply ( GroupNode& node );
    virtual void apply ( SwitchNode& node );
    virtual void apply ( LineNodef& node );
    virtual void apply ( TextNode& node );
    virtual void apply ( MeshNode3f& node );
    virtual void apply ( MeshLineNode& node );
    virtual void apply ( PolylineNode2Df& node );
    virtual void apply ( PolyNode2Df& node );
    virtual void apply ( PointNode& node );
    virtual void apply ( MeshPointNode& node );
private:
    stringstream _xmlContent;
};

inline SaveMesh::SaveMesh( const string& filename, SGNode* node )
{
    _xmlContent.imbue (std::locale("C"));
    string a = _xmlContent.getloc().name ();
    _xmlContent.str("");
    _xmlContent << std::fixed;
    _xmlContent << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    _xmlContent << "<chiptemplate>";
    node->accept ( *this );
    _xmlContent << "</chiptemplate>" << endl;
    ofstream out;
    out.open ( filename.c_str() );
    out << _xmlContent.str();
    out.close();
}

inline void SaveMesh::apply ( LayerNode& node )
{
    _xmlContent << "<layer id=\"" << node.getID() << "\" name='" << XMLCharEscape::getInst().transcode (node.name()) << "' isVisible='" << (node.isVisible() ? 1 : 0) 
        << "' fgcolor='" << node.getFgColor().toString() <<"' bgcolor='" << node.getBgColor().toString() << "' bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
   
    ChildVisitor::apply ( node );
    _xmlContent << "</layer>";
}

inline void SaveMesh::apply ( RectangleNodef& node )
{
    _xmlContent << "<rect id=\"" << node.getID() << "\" width='" << node.w() << "' height='" << node.h() << "'";
        
    AttrSet* pAttrSet = node.getAttrSet();
    if ( pAttrSet )
        _xmlContent << " fillcolor='" << pAttrSet->getBgColor()->toString() << "' bordercolor='" << pAttrSet->getFgColor()->toString() << "'";
    _xmlContent << " bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";

    ChildVisitor::apply ( node );
    _xmlContent << "</rect>";
}

inline void SaveMesh::apply ( TransformNode& node )
{
    stringstream ts, ss;
    mat4f& tm = node.getMatrix();
    ts << tm.dx() << ' ' << tm.dy() << ' ' << tm.dz();
    mat4f& sm = node.getMatrix();
    ss << sm.sx() << ' ' << sm.sy() << ' ' << sm.sz();

    _xmlContent << "<transform id=\"" << node.getID() << "\" translate='" << ts.str() << "' scale='" << ss.str() << "' bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
    ChildVisitor::apply ( node );
    _xmlContent << "</transform>";
}

inline void SaveMesh::apply ( ArrayNode& node )
{
    stringstream ss;
    for ( int i=0; i<6; i++ )
    {
        int levelcnt = node.getHLevels (i);
        if ( levelcnt != 0 )
            ss << "level" << i << "hcnt='" << levelcnt << "' ";
        levelcnt = node.getVLevels (i);
        if ( levelcnt != 0 )
            ss << "level" << i << "vcnt='" << levelcnt << "' ";
        float space = node.getMarginX (i);
        if ( space != 0 )
            ss << "spacex" << i+1 << "='" << space << "' ";
        space = node.getMarginY (i);
        if ( space != 0 )
            ss << "spacey" << i+1 << "='" << space << "' ";
    }
    ss << "cntx='" << node.getColumnCnt() << "' cnty='" << node.getRowCnt() << "'";

    _xmlContent << "<array id=\"" << node.getID() << "\" " << ss.str() << " bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
    ChildVisitor::apply ( node );
    _xmlContent << "</array>";
}

inline void SaveMesh::apply ( LODNode& node )
{
    stringstream ss;
    for ( LODNode::diterator pp=node.dbegin(); pp!=node.dend(); ++pp )
        ss << (*pp) << " ";
    if ( ss.str() == "" )
        _xmlContent << "<lod id=\"" << node.getID() << "\" bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
    else
        _xmlContent << "<lod id=\"" << node.getID() << "\" range='" << ss.str() << "' bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";

    ChildVisitor::apply ( node );
    _xmlContent << "</lod>";
}

inline void SaveMesh::apply ( PickableGroup& node )
{
    _xmlContent << "<pickablegroup id=\"" << node.getID() << "\" name='" << XMLCharEscape::getInst().transcode (node.name()) << "' bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
    ChildVisitor::apply ( node );
    _xmlContent << "</pickablegroup>";
}

inline void SaveMesh::apply ( KdTreeNode& node )
{
    _xmlContent << "<kdtree id=\"" << node.getID() << "\" bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
    ChildVisitor::apply ( node );
    _xmlContent << "</kdtree>";
}

inline void SaveMesh::apply ( SceneNode& node )
{
    stringstream ts, ss;
    mat4f& tm = node.getMatrix();
    ts << tm.dx() << ' ' << tm.dy() << ' ' << tm.dz();
    mat4f& sm = node.getMatrix();
    ss << sm.sx() << ' ' << sm.sy() << ' ' << sm.sz();

    _xmlContent << "<scene id=\"" << node.getID() << "\" isVisible=\"" << (node.isVisible() ? 1 : 0) << "\" translate=\"" << ts.str() << "\" scale=\"" << ss.str() << "\" bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
    ChildVisitor::apply ( node );
    _xmlContent << "</scene>";
    //_xmlContent << "<mesh>";
    //ChildVisitor::apply ( node );
    //_xmlContent << "</mesh>";
}

inline void SaveMesh::apply ( FontNode& node )
{
    _xmlContent << "<font id=\"" << node.getID() << "\" family=\"" << XMLCharEscape::getInst().transcode (node.family()) << "\" size=\"" << node.pointSize() << "\" style=\"" << "normal" << "\" def=\"" << node.defName() << "\" bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
    ChildVisitor::apply ( node );
    _xmlContent << "</font>";
}

inline void SaveMesh::apply ( GroupNode& node )
{
    _xmlContent << "<group id=\"" << node.getID() << "\" isVisible=\"" << (node.isVisible() ? 1 : 0) << "\" name='" << XMLCharEscape::getInst().transcode (node.name()) << "' bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
    ChildVisitor::apply ( node );
    _xmlContent << "</group>";
}

inline void SaveMesh::apply ( SwitchNode& node )
{
    _xmlContent << "<switch id=\"" << node.getID() << "\" isVisible='" << (node.isVisible() ? 1 : 0) << "' selection='" << node.selection() << "' bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
    ChildVisitor::apply ( node );
    _xmlContent << "</switch>";
}

inline void SaveMesh::apply ( LineNodef& node )
{
    _xmlContent << "<line id=\"" << node.getID() << "\" x1='" << node.x1() << "' y1='" << node.y1() << "' x2='" << node.x2() << "' y2='" << node.y2() << "' bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
    ChildVisitor::apply ( node );
    _xmlContent << "</line>";
}

inline void SaveMesh::apply ( TextNode& node )
{
    _xmlContent << "<text id=\"" << node.getID() << "\"";
    if ( NULL != node.getAttrSet() && NULL != node.getAttrSet()->getFont() ) 
        _xmlContent << " font='" << node.getAttrSet()->getFont()->defName() << "'";
    _xmlContent << " anchor='" << node.anchorValue() << "' bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
    _xmlContent << XMLCharEscape::getInst().transcode (node.text());
    ChildVisitor::apply ( node );
    _xmlContent << "</text>";
}

inline void SaveMesh::apply ( MeshNode3f& node )
{
    stringstream ss;
    MeshNode3f::coorditerator pp, end=node.coordend();
    for ( pp=node.coordbegin(); pp!=end; ++pp )
        ss << pp->x() << " " << pp->y() << " " << pp->z() << " ";
    
    _xmlContent << "<mesh id=\"" << node.getID() << "\"  coords='" << ss.str() << "' bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
    ChildVisitor::apply ( node );
    _xmlContent << "</mesh>";
}

inline void SaveMesh::apply ( MeshLineNode& node )
{
    stringstream ss;
    MeshLineNode::pntiterator pp, end=node.pntend();
    for ( pp=node.pntbegin(); pp!=end; ++pp )
        ss << *pp << " ";

    _xmlContent << "<meshline id=\"" << node.getID() << "\"  type='" << node.type() << "' bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
    _xmlContent << ss.str();
    ChildVisitor::apply ( node );
    _xmlContent << "</meshline>";
}

inline void SaveMesh::apply ( PolylineNode2Df& node )
{
    stringstream ss;
    PolylineNode2Df::pointiterator pp, end=node.pointend();
    for ( pp=node.pointbegin(); pp!=end; ++pp )
        ss << pp->x() << " " << pp->y() << " ";

    _xmlContent << "<polyline id=\"" << node.getID() << "\" bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
    _xmlContent << ss.str();
    ChildVisitor::apply ( node );
    _xmlContent << "</polyline>";
}

inline void SaveMesh::apply ( PolyNode2Df& node )
{
    stringstream ss;
    PolyNode2Df::pointiterator pp, end=node.pointend();
    for ( pp=node.pointbegin(); pp!=end; ++pp )
        ss << pp->x() << " " << pp->y() << " ";

    _xmlContent << "<poly id=\"" << node.getID() << "\" bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
    _xmlContent << ss.str();
    ChildVisitor::apply ( node );
    _xmlContent << "</poly>";
}

inline void SaveMesh::apply ( PointNode& node )
{
    _xmlContent << "<point id=\"" << node.getID() << "\" size=\"" << node.pointSize() << "\" bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
    _xmlContent << node.x() << ' ' << node.y();
    ChildVisitor::apply ( node );
    _xmlContent << "</point>";
}

inline void SaveMesh::apply ( MeshPointNode& node )
{
    _xmlContent << "<meshpoint id=\"" << node.getID() << "\" size=\"" << node.pointSize() << "\" index=\"" << node.coordIdx()<< "\" bbox =\"" << 
        node.getBBox().minvec().x() << ' ' << node.getBBox().minvec().y() << ' ' << node.getBBox().minvec().z() << " " <<
       node.getBBox().maxvec().x() << ' ' << node.getBBox().maxvec().y() << ' ' << node.getBBox().maxvec().z() << "\">";
    ChildVisitor::apply ( node );
    _xmlContent << "</meshpoint>";
}


}
#endif

