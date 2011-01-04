#ifndef __RECTANGLENODE_H__
#define __RECTANGLENODE_H__

#include "sgr_drawablenode.h"
#include "sgr_nodemgr.h"
#include <sgr_vec2.h>
#include <sgr_vec4.h>
#include <sgr_mat4.h>
#include <sgr_circlenode.h>
//#include "fbcolor.h"
//#include "sgr_parentfinder.h"
namespace SGR
{

    CircleNode::CircleNode ( float x, float y, float r ) : DrawableNode()
    {
        _circle.setCenter ( x, y, 0 );
        _circle.setRadius ( r );
    }

    CircleNode::CircleNode ( const CircleNode& rhs ) : DrawableNode ( rhs ), _circle(rhs._circle)
    {
    }

    CircleNode::~CircleNode ()
    {
    }

    SGNode* CircleNode::clone ()
    {
        return new CircleNode(*this);
    }

    void CircleNode::updateBBox( const mat4f& mat, bool force )
    {
        if ( force || isBBoxDirty()  )
        {
            _bb = _circle.boundingbox();

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

    // if mat == 0, mat should be computed inside function.
    void CircleNode::computeBBox( const mat4f* mat ) const
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
        bb = _circle.boundingbox();

        vec4f min = (*mat) * vec4f ( bb.minvec() );
        vec4f max = (*mat) * vec4f ( bb.maxvec() );
        bb.setminmax ( min.xyz(), max.xyz() );

        _bb = _bb.unionbox ( bb );
    }

    void CircleNode::setCenter ( float x, float y, float z )
    {
        _circle.setCenter ( x, y, z );
        setBBoxDirty (); 
//        setParentBBoxDirty ( true );
    }

    void CircleNode::setRadius ( float r )
    {
        _circle.setRadius ( r );
        setBBoxDirty (); 
//        setParentBBoxDirty ( true );
    }

}
#endif
