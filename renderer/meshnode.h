#ifndef _MESHNODE_H_
#define _MESHNODE_H_

#include "drawablenode.h"
#include <vector>
using namespace std;

template < class COORDT >
class AGEF_EXPORT MeshNode : public DrawableNode
{
public:
    typedef typename vector<COORDT>::iterator coorditerator;

    MeshNode() {}
    MeshNode( const MeshNode& rhs ) : DrawableNode(rhs) {_coords = rhs._coords;}
    virtual void accept ( NodeVisitor& pvisitor ) const { pvisitor.apply ( *this ); }
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~MeshNode () {}

    void setCoords ( COORDT* data, int elementN )
    {
        _coords.assign ( data, data+elementN );
    }
    void setSubCoords ( int* indexes, int elementN, COORDT* coords )
    {
        for ( int i=0; i<elementN; i++ )
        {
            _coords[indexes[i]] = coords[i];
        }
    }
    void setCoords ( int index, COORDT& coords )
    {
        _coords[index] = coords;
    }

    COORDT& operator[] (int idx ) { return _coords[idx]; }
    coorditerator coordbegin() { return _coords.begin(); }
    coorditerator coordend() { return _coords.end(); }
private:
    vector<COORDT> _coords;
};

typedef MeshNode<vec3f >      MeshNode3f;
typedef MeshNode<vec2f >      MeshNode2f;
typedef MeshNode<vec3d >      MeshNode3d;
typedef MeshNode<vec2d >      MeshNode2d;
#endif
