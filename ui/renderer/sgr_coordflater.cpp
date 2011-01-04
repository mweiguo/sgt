#include "sgr_coordflater.h"
#include "sgr_nodes.h"

namespace SGR
{
    CoordFlater::CoordFlater()
    {
	result.clear();
	_curmat.normalize();
	_camid = -1;
    }

    CoordFlater::~CoordFlater()
    {
	typedef list<DrawableNode*>::iterator DrawableListIterator;
	for ( DrawableListIterator pp=result.begin(); pp!=result.end(); ++pp )
	    delete *pp;
	
	typedef list<MeshNode3f*>::iterator MeshListIterator;
	for ( MeshListIterator pp=_meshnodeStack.begin(); pp!=_meshnodeStack.end(); ++pp )
	    delete *pp;
    }

    void CoordFlater::doAction ( int camid, SGNode* node )
    {
	result.clear();
	_curmat.normalize();
	_camid = camid;

	node->accept ( *this );	
    }

    void CoordFlater::apply ( SceneNode& node )
    {
	const mat4f& m = node.getMatrix(), oldmat = _curmat;
	_curmat = m * _curmat;
	ChildVisitor::apply ( node );
	_curmat = oldmat;
    }

    void CoordFlater::apply ( RectangleNodef& node )
    {
	vec3f pos = (_curmat * vec4f (0, 0, 0, 1)).xy();
	vec3f pos1 = (_curmat * vec4f (node.w(), node.h(), 0, 1)).xy();
	RectangleNodef* rect = new RectangleNodef ( node );
	rect->setRect ( pos.x(), pos.y(), pos1.x()-pos.x(), pos1.y()-pos.y() );

	result.push_back ( rect );

	ChildVisitor::apply ( node );
    }

    void CoordFlater::apply ( TransformNode& node )
    {
	const mat4f& m = node.getMatrix(), oldmat = _curmat;
	_curmat = m * _curmat;
	ChildVisitor::apply ( node );
	_curmat = oldmat;
    }

    void CoordFlater::apply ( LODNode& node )
    {
	CameraOrtho* cam = NodeMgr::getInst().getNodePtr<CameraOrtho>(_camid);
	if ( cam )
	{
	    SGNode* sgnode = node.selectPresentation ( cam->mvmatrix().sx() );
	    if ( sgnode )
	    {
		sgnode->accept ( *this );
	    }
	}
    }

    void CoordFlater::apply ( TextNode& node )
    {
	if ( node.getSizeMode() == TextNode::TXTSIZEMODE_SCREEN )
	{
	    TextNode* textnode = new TextNode ( node );
	    textnode->removeAllChild ();
	    result.push_back ( textnode );
	}
	ChildVisitor::apply ( node );
    }

    void CoordFlater::apply ( SwitchNode& node )
    {
	if ( node.selection() >= 0 && (unsigned int)(node.selection()) < node.size() )
	{
	    int i=0;
	    SGNode::iterator pp = node.begin();
	    while ( i<node.selection() )
	    {
		++pp;
		++i;
	    }

	    (*pp)->accept ( *this );
	}
	else if ( node.selection() < 0 )
	{
	    ChildVisitor::apply ( node );
	}
    }

    void CoordFlater::apply ( LineNodef& node )
    {
	vec2f pos = (_curmat * vec4f (0,0,0,1)).xy();
	vec3f p1 ( pos.x()+node.x1(), pos.y()+node.y1(), 0.f );
	vec3f p2 ( pos.x()+node.x2(), pos.y()+node.y2(), 0.f );

	LineNodef* linenode = new LineNodef ( node );
	linenode->setPoints ( p1.x(), p1.y(), p2.x(), p2.y() );
	result.push_back ( linenode );

	ChildVisitor::apply ( node );
    }

    void CoordFlater::apply ( MeshNode3f& node )
    {
	// no rotation & shear?
	vec2f pos = (_curmat * vec4f (0,0,0,1)).xy();

	MeshNode3f* meshnode = new MeshNode3f ( node );
	MeshNode3f::coorditerator pp, end = meshnode->coordend();
	int idx = 0;
	for ( pp=meshnode->coordbegin(); pp!=end; ++pp, idx++ )
	{
	    vec3f& pnt = node[idx];
	    vec3f newpos ( pos.x()+pnt.x(), pos.y()+pnt.y(), 0.f );
	    node.setCoords ( idx, newpos );
	}

	// this stack will be checked in it's children visitation, if stack is not empty, then child node will be picked
	_meshnodeStack.push_back ( meshnode );

	ChildVisitor::apply ( node );
    }

    void CoordFlater::apply ( MeshLineNode& node )
    {
	MeshNode3f* meshnode = _meshnodeStack.back();
	// change meshnode's coord
	for ( MeshLineNode::pntiterator pp=node.pntbegin(); pp!=node.pntend(); ++pp )
	{
	    vec3f pos = (_curmat * vec4f (0,0,0,1)).xyz();
	    (*meshnode)[*pp] += pos;
	}
	MeshLineNode* meshlinenode = new MeshLineNode ( node );
	meshlinenode->setParentNode ( meshnode );

	result.push_back ( meshlinenode );

	ChildVisitor::apply ( node );
    }

    void CoordFlater::apply ( PolylineNode2Df& node )
    {
	// no rotation & shear?
	vec2f pos = (_curmat * vec4f (0,0,0,1)).xy();
	PolylineNode2Df* linenode = new PolylineNode2Df ( node );
	for ( PolylineNode2Df::pointiterator pp=linenode->pointbegin(); pp!=linenode->pointend(); ++pp )
	{
	    (*pp) += pos;
	}
                
	result.push_back ( linenode );

	ChildVisitor::apply ( node );
    }

    void CoordFlater::apply ( PolyNode2Df& node )
    {
	vec2f pos = (_curmat * vec4f (0,0,0,1)).xy();
	PolyNode2Df* polynode = new PolyNode2Df ( node );
                
	for ( PolylineNode2Df::pointiterator pp=polynode->pointbegin(); pp!=polynode->pointend(); ++pp )
	    (*pp) += pos;
                
	result.push_back ( polynode );

	ChildVisitor::apply ( node );
    }

    void CoordFlater::apply ( PointNode& node )
    {
	// no rotation & shear?
	vec3f pos = (_curmat * vec4f (0,0,0,1)).xyz();
	vec3f p ( pos.x()+node.x(), pos.y()+node.y(), pos.z()+node.z() );
	PointNode* pointnode = new PointNode ( node );
	pointnode->xyz ( p.x(), p.y(), p.z() );

	result.push_back ( pointnode );

	ChildVisitor::apply ( node );
    }

    void CoordFlater::apply ( MeshPointNode& node )
    {
	// no rotation & shear?
	vec3f pos = (_curmat * vec4f (0,0,0,1)).xyz();
	MeshNode3f* meshnode = _meshnodeStack.back();
	(*meshnode)[node.coordIdx()] += pos;

	MeshPointNode* pointnode = new MeshPointNode ( node );
	result.push_back ( pointnode );

	ChildVisitor::apply ( node );
    }

    void CoordFlater::apply ( CircleNode& node )
    {
	vec2f cen = (_curmat * vec4f (node.getCenter())).xy();
	vec3f v1 = (_curmat * vec4f (node.getRadius(),0,0,1)).xyz();
	vec3f v2 = (_curmat * vec4f (0,0,0,1)).xyz();
	float newRadius = (v1-v2).mod();
        
	CircleNode* circle = new CircleNode ( node );
	circle->setCenter ( cen.x(), cen.y() );
	circle->setRadius ( newRadius );

	result.push_back ( circle );

	ChildVisitor::apply ( node );
    }

    void CoordFlater::apply ( ImageNode& node )
    {
	ImageNode* imagenode = new ImageNode ( node );
	result.push_back ( imagenode );

	ChildVisitor::apply ( node );
    }

    void CoordFlater::apply ( ImposterNode& node )
    {
	ChildVisitor::apply ( node );
	return;
	CameraOrtho* cam = NodeMgr::getInst().getNodePtr<CameraOrtho>(_camid);
	if ( cam )
	{
	    int idx = node.getLodDelimiterIndex ( cam->mvmatrix().sx() );
	    if ( idx >= 0 )
	    {
		ImposterNode* temp = new ImposterNode ( node );
		result.push_back ( temp );
	    }
	    else
	    {
		ChildVisitor::apply ( node );
	    }
	}
    }
}
