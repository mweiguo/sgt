#ifndef _RENDERNODE_COLLECTOR_
#define _RENDERNODE_COLLECTOR_

#include "sgr_childvisitor.h"

//#include "sgr_rectitem.h"
#include "sgr_transformnode.h"
#include "sgr_arraynode.h"
#include "sgr_mat4.h"
#include "sgr_vec2.h"
#include "sgr_vec4.h"
#include "sgr_nodes.h"

#include <vector>
#include <string>
#include <sstream>
using namespace std;

// this class instance will change level coordinates to flat coordinates
// and save each item to Output
namespace SGR
{

template<class Output>
class /*SGR_DLL*/ RenderNodeCollector : public ChildVisitor
{
public:
    RenderNodeCollector( Output output) : _result(output) {}
    virtual void apply ( RectangleNodef& node );
    virtual void apply ( TransformNode& node );
    virtual void apply ( ArrayNode& node );
    virtual void apply ( LineNodef& node );
    virtual void apply ( CircleNode& node );
    void operator () ( SGNode& node ) { node.accept ( *this ); }
private:
    Output _result;
    mat4f _curmat;
};

template<class Output>
void RenderNodeCollector<Output>::apply ( RectangleNodef& node )
{
    vec2f v = (_curmat * vec4f (0,0,0,1)).xy();
    RectangleNodef* pNode = new RectangleNodef ( node );
    pNode->setRect ( v.x(), v.y(), node.w(), node.h() );
    *_result++ = pNode;
//     qDebug ("\t\trect (x, y, w, h) : rect ( %f, %f, %f, %f )", v.x(), v.y(), node.w(), node.h());
    ChildVisitor::apply ( node );
}

template<class Output>
void RenderNodeCollector<Output>::apply ( TransformNode& node )
{
    const mat4f& m = node.getMatrix(), oldmat = _curmat;
    _curmat = m * _curmat;

    ChildVisitor::apply ( node );

    _curmat = oldmat;
}   

template<class Output>
void RenderNodeCollector<Output>::apply ( ArrayNode& node )
{
    //_result.reserve ( node.rowCnt() * node.columnCnt() * 8);
    for ( int j=0; j<node.rowCnt(); j++ ) {
        float y = node.getVOffset ( j );
        for ( int k=0; k<node.columnCnt(); k++ ) {
            float x = node.getHOffset ( k );
            // generate new matrix & use this matrix
            mat4f m = mat4f::translate_matrix ( x, y, 0 ), oldmat = _curmat;
            _curmat = m * _curmat;

            ChildVisitor::apply ( node );
            _curmat = oldmat;
        }
    }
}

template<class Output>
void RenderNodeCollector<Output>::apply ( LineNodef& node )
{
    vec2f v = (_curmat * vec4f (0,0,0,1)).xy();
    LineNodef* pNode = new LineNodef ( node );
    pNode->setPoints ( v.x()+node.x1(), v.y()+node.y1(), v.x()+node.x2(), v.y()+node.y2() );
    *_result++ = pNode;

    ChildVisitor::apply ( node );
}

template<class Output>
void RenderNodeCollector<Output>::apply ( CircleNode& node )
{
    vec2f v = (_curmat * vec4f ( node.getCenter() )).xy();
    vec3f v1 = (_curmat * vec4f (node.getRadius(),0,0,1)).xyz();
    vec3f v2 = (_curmat * vec4f (0,0,0,1)).xyz();
    float newRadius = (v1-v2).mod();

    CircleNode* pNode = new CircleNode ( node );
    pNode->setCenter ( v.x(), v.y() );
    pNode->setRadius ( newRadius );

    *_result++ = pNode;
    ChildVisitor::apply ( node );
}

}
#endif
