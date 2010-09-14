#ifndef _MESHLINENODE_H_
#define _MESHLINENODE_H_

#include "sgr_drawablenode.h"
namespace SGR
{

class SGR_DLL MeshLineNode : public DrawableNode
{
public:
    static const int LINETYPE_LINES      = 0;
    static const int LINETYPE_LINE_STRIP = 1;
    static const int LINETYPE_LINE_LOOP  = 2;

    typedef vector<int>::iterator pntiterator;

    MeshLineNode() {_type = LINETYPE_LINE_STRIP;}
    MeshLineNode( const MeshLineNode& rhs ) : DrawableNode(rhs) { _type = rhs._type; _data = rhs._data; }

    virtual void accept ( NodeVisitor& pvisitor ) const { pvisitor.apply ( *this ); }
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~MeshLineNode () {}

    int type() { return _type; }
    void type ( int type ) { _type = type; }
    
    // bbox will be calculated in these functions
    void setData ( int* data, int elementN )
    {
        _data.assign ( data, data+elementN );
    }
    void setData ( int* indexes, int elementN, int* data )
    {
        for ( int i=0; i<elementN; i++ )
        {
            _data[indexes[i]] = data[i];
        }
    }
    void setData ( int index, int data )
    {
        _data[index] = data;
    }
    void appendCoordIdx ( int idx )
    {
        _data.push_back ( idx );
    }

    virtual void updateBBox( const mat4f& mat=mat4f() )
    {
        // get bounding box, and assume there is no transform on it coords
        ParentFinder<MeshNode3f> finder(this);
        if ( finder.target() )
        {
            _bb.setInvalid();
            MeshNode3f* meshnode = finder.target();
            pntiterator pp, e = pntend();
            for ( pp=pntbegin(); pp!=e; ++pp )
            {
                vec3f& pos = (*meshnode)[*pp];
                _bb.expandby ( pos );
            }
        }

        // add transform matrix on it
        vec4f min = mat * vec4f ( _bb.minvec() );
        vec4f max = mat * vec4f ( _bb.maxvec() );
        _bb.setminmax ( min.xyz(), max.xyz() );

        for ( iterator pp=begin(); pp!=end(); ++pp )
        {
            (*pp)->updateBBox();
            _bb = _bb.unionbox ( (*pp)->getBBox() );
        }
    }

    pntiterator pntbegin() { return _data.begin(); }
    pntiterator pntend() { return _data.end(); }
    int pntsize() { return _data.size(); }

private:
    vector<int> _data;
    int _type;
};
}
//typedef NodeMgr<SceneNode>      SceneNodeMgr;
#endif
