#include "sgr_glrenderfunctor.h"
#include "sgr_nodes.h"

#include <GL/gl.h>

namespace SGR
{

GLRenderVisitor::GLRenderVisitor ()
{
}

void GLRenderVisitor::apply ( RectangleNodef& rect )
{
    glBegin ( GL_QUADS );
        glVertex3f ( rect.x(), rect.y(), 0 );
        glVertex3f ( rect.x()+rect.w(), rect.y(), 0 );
        glVertex3f ( rect.x(), rect.y()+rect.h(), 0 );
        glVertex3f ( rect.x()+rect.w(), rect.y()+rect.h(), 0 );
    glEnd ();
}

void GLRenderVisitor::apply ( TextNode& text )
{
}

void GLRenderVisitor::apply ( LineNodef& node )
{
    glBegin ( GL_LINES );
        glVertex3f ( node.point1().x(), node.point1().y(), 0 );
        glVertex3f ( node.point2().x(), node.point2().y(), 0 );
    glEnd ();
}

void GLRenderVisitor::apply ( MeshLineNode& node )
{
    glDrawElements ( GL_LINES, node.pntsize() * 3, GL_UNSIGNED_BYTE, &(*node.pntbegin()) );
}

void GLRenderVisitor::apply ( PolylineNode2Df& node )
{
    glBegin ( GL_LINE_LOOP );
    for ( PolylineNode2Df::pointiterator pp=node.pointbegin(); pp!=node.pointend(); ++pp )
        glVertex3f ( pp->x(), pp->y(), 0 );
    glEnd ();
}

void GLRenderVisitor::apply ( PolyNode2Df& node )
{
    glBegin ( GL_POLYGON );
    for ( PolyNode2Df::pointiterator pp=node.pointbegin(); pp!=node.pointend(); ++pp )
        glVertex3f ( pp->x(), pp->y(), 0 );
    glEnd ();
}

void GLRenderVisitor::apply ( PointNode& node )
{
    glBegin ( GL_POINTS );
        glVertex3f ( node.x(), node.y(), node.z() );
    glEnd ();
}

void GLRenderVisitor::apply ( MeshPointNode& node )
{
    int p = node.coordIdx();
    glDrawElements ( GL_POINTS, 1, GL_UNSIGNED_BYTE, &p );
}

void GLRenderVisitor::apply ( MeshNode3f& node )
{
    glVertexPointer ( 3, GL_FLOAT, 0, &node[0] );
}

}
