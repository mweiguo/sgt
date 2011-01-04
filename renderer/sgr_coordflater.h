#ifndef _COORD_FLATER_H_
#define _COORD_FLATER_H_

#include "sgr_childvisitor.h"
#include "sgr_mat4.h"

#include <list>
using namespace std;

/** this class instance will change level coordinates to flat coordinates
 * and save each item to Output
 */
namespace SGR
{

    class SGR_DLL CoordFlater : public ChildVisitor
    {
    public:
	CoordFlater();
	virtual ~CoordFlater();

	/**
	 * camid is used for filter nodes, like lod, imposter
	 */
	void doAction ( int camid, SGNode* node );

	virtual void apply ( SceneNode& node );
	virtual void apply ( RectangleNodef& node );
	virtual void apply ( TransformNode& node );
	virtual void apply ( LODNode& node );
	virtual void apply ( TextNode& node );
	virtual void apply ( SwitchNode& node );
	virtual void apply ( LineNodef& node );
	virtual void apply ( MeshNode3f& node );
	virtual void apply ( MeshLineNode& node );
	virtual void apply ( PolylineNode2Df& node );
	virtual void apply ( PolyNode2Df& node );
	virtual void apply ( PointNode& node );
	virtual void apply ( MeshPointNode& node );
	virtual void apply ( CircleNode& node );
	virtual void apply ( ImageNode& node );
	virtual void apply ( ImposterNode& node );
	

	list<DrawableNode*> result;
    private:
	mat4f _curmat;
	int _camid;
	list<MeshNode3f*> _meshnodeStack;
    };

}
#endif
