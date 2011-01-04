#ifndef _MESHLINENODE_H_
#define _MESHLINENODE_H_

#include "sgr_drawablenode.h"
#include <vector>

namespace SGR
{

class SGR_DLL MeshLineNode : public DrawableNode
{
public:
    static const int LINETYPE_LINES      = 0;
    static const int LINETYPE_LINE_STRIP = 1;
    static const int LINETYPE_LINE_LOOP  = 2;

    typedef std::vector<int>::iterator pntiterator;
    typedef std::vector<int>::const_iterator const_pntiterator;

    MeshLineNode() {_type = LINETYPE_LINE_STRIP;}
    MeshLineNode( const MeshLineNode& rhs ) : DrawableNode(rhs) { _type = rhs._type; _data = rhs._data; }

    virtual SGNode* clone ()
    {
        return new MeshLineNode(*this);
    }

    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~MeshLineNode () {}

    int type() { return _type; }
    void type ( int type ) { _type = type; }
    
    // bbox will be calculated in these functions
    void setData ( int* data, int elementN );
    void setData ( int* indexes, int elementN, int* data );
    void setData ( int index, int data );
    void appendCoordIdx ( int idx );
    virtual void updateBBox( const mat4f& mat=mat4f(), bool force=false );
    virtual void computeBBox( const mat4f* mat=0 ) const;

    pntiterator pntbegin() { return _data.begin(); }
    pntiterator pntend() { return _data.end(); }
    const_pntiterator pntbegin() const { return _data.begin(); }
    const_pntiterator pntend() const { return _data.end(); }
    int pntsize() const { return _data.size(); }

private:
    vector<int> _data;
    int _type;
};
}
//typedef NodeMgr<SceneNode>      SceneNodeMgr;
#endif
