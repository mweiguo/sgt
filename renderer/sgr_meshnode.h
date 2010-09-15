#ifndef _MESHNODE_H_
#define _MESHNODE_H_

#include "sgr_drawablenode.h"
#include <vector>
using namespace std;
namespace SGR
{

template < class COORDT >
class SGR_DLL MeshNode : public DrawableNode
{
public:
    typedef typename vector<COORDT>::iterator coorditerator;

    MeshNode() {}
    MeshNode( const MeshNode& rhs ) : DrawableNode(rhs) {_coords = rhs._coords;}
    
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~MeshNode () {}

    void setCoords ( COORDT* data, int elementN )
    {
        _coords.assign ( data, data+elementN );
        setBBoxDirty ( true );
        setParentBBoxDirty ( true );
        setChildrenBBoxDirty ( true );
    }

    void setSubCoords ( int* indexes, int elementN, COORDT* coords )
    {
        for ( int i=0; i<elementN; i++ )
        {
            _coords[indexes[i]] = coords[i];
        }
        setBBoxDirty ( true );
        setParentBBoxDirty ( true );
        setChildrenBBoxDirty ( true );
    }

    void setCoords ( int index, COORDT& coords )
    {
        _coords[index] = coords;
        setBBoxDirty ( true );
        setParentBBoxDirty ( true );
        setChildrenBBoxDirty ( true );
    }

    void appendCoords ( const COORDT& coords )
    {
        _coords.push_back ( coords );
        setBBoxDirty ( true );
        setParentBBoxDirty ( true );
        setChildrenBBoxDirty ( true );
    }

    void appendCoords ( COORDT* coords, int elementN )
    {
        copy ( coords, coords+elementN, back_inserter(_coords) );
        setBBoxDirty ( true );
        setParentBBoxDirty ( true );
        setChildrenBBoxDirty ( true );
    }

    COORDT& operator[] (int idx ) { setBBoxDirty ( true ); return _coords[idx]; }
    coorditerator coordbegin() { return _coords.begin(); }
    coorditerator coordend() { return _coords.end(); }
    int coordsN () { return _coords.size(); }


private:
    vector<COORDT> _coords;
};

typedef MeshNode<vec3f >      MeshNode3f;
typedef MeshNode<vec2f >      MeshNode2f;
typedef MeshNode<vec3d >      MeshNode3d;
typedef MeshNode<vec2d >      MeshNode2d;

}
#endif
