#include "sgr_meshlinenode.h"
#include "sgr_parentfinder.h"

namespace SGR
{
    // bbox will be calculated in these functions
    void MeshLineNode::setData ( int* data, int elementN )
    {
        _data.assign ( data, data+elementN );
        setBBoxDirty ( true );
        setParentBBoxDirty ( true );
    }
    void MeshLineNode::setData ( int* indexes, int elementN, int* data )
    {
        for ( int i=0; i<elementN; i++ )
            _data[indexes[i]] = data[i];
        setBBoxDirty ( true );
        setParentBBoxDirty ( true );
    }
    void MeshLineNode::setData ( int index, int data )
    {
        _data[index] = data;
        setBBoxDirty ( true );
        setParentBBoxDirty ( true );
    }
    void MeshLineNode::appendCoordIdx ( int idx )
    {
        _data.push_back ( idx );
        setBBoxDirty ( true );
        setParentBBoxDirty ( true );
    }

    void MeshLineNode::updateBBox( const mat4f& mat )
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

        setBBoxDirty ( false );
    }


}
