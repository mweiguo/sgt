#include "sgr_meshlinenode.h"
#include "sgr_parentfinder.h"

namespace SGR
{
    // bbox will be calculated in these functions
    void MeshLineNode::setData ( int* data, int elementN )
    {
        _data.assign ( data, data+elementN );
        setBBoxDirty ();
        //setParentBBoxDirty ( true );
    }
    void MeshLineNode::setData ( int* indexes, int elementN, int* data )
    {
        for ( int i=0; i<elementN; i++ )
            _data[indexes[i]] = data[i];
        setBBoxDirty ();
        //setParentBBoxDirty ( true );
    }
    void MeshLineNode::setData ( int index, int data )
    {
        _data[index] = data;
        setBBoxDirty ();
        //setParentBBoxDirty ( true );
    }
    void MeshLineNode::appendCoordIdx ( int idx )
    {
        _data.push_back ( idx );
        setBBoxDirty ();
        //setParentBBoxDirty ( true );
    }

    void MeshLineNode::updateBBox( const mat4f& mat, bool force )
    {
        if ( force || isBBoxDirty()  )
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
                if ( force || (*pp)->isBBoxDirty () )
                    (*pp)->updateBBox(mat, force);
                _bb = _bb.unionbox ( (*pp)->getBBox() );
            }

            _isBBoxDirty = false;
        }
    }

    void MeshLineNode::computeBBox( const mat4f* mat ) const
    {
        if ( false == _isBBoxDirty )
            return;

        mat4f tmat;
        if ( 0 == mat )
        {
            mat = &tmat;
            tmat = getParentTranMatrix ();
        }
        DrawableNode::computeBBox ( mat );

        BBox bb;
        // get bounding box, and assume there is no transform on it coords
        ParentFinder<MeshNode3f> finder(this);
        if ( finder.target() )
        {
            bb.setInvalid();
            MeshNode3f* meshnode = finder.target();
            const_pntiterator pp, e = pntend();
            for ( pp=pntbegin(); pp!=e; ++pp )
            {
                vec3f& pos = (*meshnode)[*pp];
                bb.expandby ( pos );
            }
        }

        // add transform matrix on it
        vec4f min = (*mat) * vec4f ( bb.minvec() );
        vec4f max = (*mat) * vec4f ( bb.maxvec() );
        bb.setminmax ( min.xyz(), max.xyz() );

        _bb = _bb.unionbox ( bb );
    }

}
