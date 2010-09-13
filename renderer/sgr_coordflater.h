#ifndef _COORD_FLATER_H_
#define _COORD_FLATER_H_

#include "sgr_nodevisitor.h"
#include "sgr_rectitem.h"
#include "sgr_transformnode.h"
#include "sgr_arraynode.h"
#include "sgr_mat4.h"
#include "sgr_vec2.h"
#include "sgr_vec4.h"
#include "sgr_linenode.h"

#include <vector>
#include <string>
#include <sstream>
using namespace std;

// this class instance will change level coordinates to flat coordinates
// and save each item to Output
namespace SGR
{

class SGR_DLL CoordFlater : public NodeVisitor
{
public:
    CoordFlater( Output output) : _result(output) {}
    virtual void apply ( SGNode& /*node*/ );
    virtual void apply ( LayerNode& /*node*/ );
    virtual void apply ( RectangleNodef& /*node*/ );
    virtual void apply ( TransformNode& /*node*/ );
    virtual void apply ( ArrayNode& /*node*/ );
    virtual void apply ( LODNode& /*node*/ );
    virtual void apply ( PickableGroup& /*node*/ );
    virtual void apply ( KdTreeNode& /*node*/ );
    virtual void apply ( LineNodef& /*node*/ );
    void operator () ( SGNode& node ) { node.accept ( *this ); }
private:
    mat4f _curmat;
};

void CoordFlater::apply ( SGNode& node )
{
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
}

void CoordFlater::apply ( LayerNode& node )
{
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
}

void CoordFlater::apply ( RectangleNodef& node )
{
    vec2f v = (_curmat * vec4f (0,0,0,1)).xy();
    RectangleNodef* pNode = new RectangleNodef ( node );
    pNode->setRect ( v.x(), v.y(), node.w(), node.h() );
    *_result++ = pNode;
    //qDebug ("rect (x, y, w, h) : rect ( %f, %f, %f, %f )", v.x(), v.y(), node.w(), node.h());
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
}

void CoordFlater::apply ( TransformNode& node )
{
    const mat4f& m = node.getMatrix(), oldmat = _curmat;
    _curmat = m * _curmat;
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
    _curmat = oldmat;
}

void CoordFlater::apply ( ArrayNode& node )
{
    //_result.reserve ( node.rowCnt() * node.columnCnt() * 8);
    for ( int j=0; j<node.rowCnt(); j++ ) {
        float y = node.getVOffset ( j );
        for ( int k=0; k<node.columnCnt(); k++ ) {
            float x = node.getHOffset ( k );
            // generate new matrix & use this matrix
            mat4f m = mat4f::translate_matrix ( x, y, 0 ), oldmat = _curmat;
            _curmat = m * _curmat;
            for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
                (*pp)->accept ( *this );
            _curmat = oldmat;
        }
    }
}

void CoordFlater::apply ( LODNode& node )
{
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
}

void CoordFlater::apply ( PickableGroup& node )
{
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
}

void CoordFlater::apply ( KdTreeNode& node )
{
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
}

void CoordFlater::apply ( LineNodef& node )
{
    vec2f v = (_curmat * vec4f (0,0,0,1)).xy();
    LineNode* pNode = new LineNodef ( node );
    pNode->setPoints ( v.x(), v.y(), node.w(), node.h() );
    *_result++ = pNode;
    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
}
}
#endif
