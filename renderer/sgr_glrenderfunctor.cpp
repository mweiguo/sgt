#include "sgr_glrenderfunctor.h"
#include "sgr_nodes.h"

#include <GL/gl.h>

namespace SGR
{

GLStateChanger::GLStateChanger ( GLRenderOption* opt, AttrSet* set )
{
    if ( NULL == set )
	return;
    
    // set foreground color ==>  QPen
    GColor* color = set->getFgColor();
    if ( color )
    {
	glClearColor ( color->r() / 255.0, color->g()/255.0, color->b()/255.0, color->a()/255.0 );
	glClear( GL_COLOR_BUFFER_BIT );
    }
        
    // set background color ==>  QBrush
    color = set->getBgColor();
    if ( color )
    {
	glColor4f ( color->r() / 255.0, color->g()/255.0, color->b()/255.0, color->a()/255.0 );
    }
}

GLRenderVisitor::GLRenderVisitor ( RenderOption* opt, RenderList& renderlist )
{
    _opt = dynamic_cast<GLRenderOption*>(opt);
    if ( NULL == _opt )
	return;

    // setup opengl matrixes, include mvmat, projmat, viewport
    glViewport ( _opt->vpXYWH[0], _opt->vpXYWH[1], _opt->vpXYWH[2], _opt->vpXYWH[3]);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf ( (float*)(&(_opt->projmat)) );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf ( (float*)(&(_opt->mvmat)) );

    LOG_INFO ( "all drawable nodes have %d count\n", renderlist.size() );
    AttrSet* lastAttrset = NULL;
    for ( RenderList::iterator pp=renderlist.begin(); pp!=renderlist.end(); ++pp )
    {
	if ( (*pp)->getAttrSet() != lastAttrset )
	{
	    // switch all state in AttrSet
	    GLStateChanger changeState ( _opt, (*pp)->getAttrSet() );
	    lastAttrset = (*pp)->getAttrSet();
	}
	(*pp)->accept ( *this );
    }
    glFlush();
}

void GLRenderVisitor::apply ( RectangleNodef& rect )
{
//     LOG_DEBUG ( "draw RectangleNodef\n" );
    glBegin ( GL_QUADS );
        glVertex3f ( rect.x(), rect.y(), 0 );
        glVertex3f ( rect.x()+rect.w(), rect.y(), 0 );
        glVertex3f ( rect.x(), rect.y()+rect.h(), 0 );
        glVertex3f ( rect.x()+rect.w(), rect.y()+rect.h(), 0 );
    glEnd ();
}

void GLRenderVisitor::apply ( TextNode& text )
{
//    LOG_INFO ( "draw TextNode\n" );
}

void GLRenderVisitor::apply ( LineNodef& node )
{
//    LOG_INFO ( "draw LineNodef\n" );
    glBegin ( GL_LINES );
        glVertex3f ( node.point1().x(), node.point1().y(), 0 );
        glVertex3f ( node.point2().x(), node.point2().y(), 0 );
    glEnd ();
}

void GLRenderVisitor::apply ( MeshLineNode& node )
{
//    LOG_INFO ( "draw MeshLineNode\n" );
    glDrawElements ( GL_LINES, node.pntsize() * 3, GL_UNSIGNED_BYTE, &(*node.pntbegin()) );
}

void GLRenderVisitor::apply ( PolylineNode2Df& node )
{
//    LOG_INFO ( "draw PolylineNode2Df\n" );
    glBegin ( GL_LINE_LOOP );
    for ( PolylineNode2Df::pointiterator pp=node.pointbegin(); pp!=node.pointend(); ++pp )
        glVertex3f ( pp->x(), pp->y(), 0 );
    glEnd ();
}

void GLRenderVisitor::apply ( PolyNode2Df& node )
{
//    LOG_INFO ( "draw PolyNode2Df\n" );
    glBegin ( GL_POLYGON );
    for ( PolyNode2Df::pointiterator pp=node.pointbegin(); pp!=node.pointend(); ++pp )
        glVertex3f ( pp->x(), pp->y(), 0 );
    glEnd ();
}

void GLRenderVisitor::apply ( PointNode& node )
{
//    LOG_INFO ( "draw PointNode\n" );
    glBegin ( GL_POINTS );
        glVertex3f ( node.x(), node.y(), node.z() );
    glEnd ();
}

void GLRenderVisitor::apply ( MeshPointNode& node )
{
//    LOG_INFO ( "draw MeshPointNode\n" );
    int p = node.coordIdx();
    glDrawElements ( GL_POINTS, 1, GL_UNSIGNED_BYTE, &p );
}

void GLRenderVisitor::apply ( MeshNode3f& node )
{
//    LOG_INFO ( "draw MeshNode3f\n" );
    glVertexPointer ( 3, GL_FLOAT, 0, &node[0] );
}

}
