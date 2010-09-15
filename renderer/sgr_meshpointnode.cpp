#include "sgr_meshpointnode.h"
#include "sgr_parentfinder.h"
#include "sgr_vec4.h"
#include "sgr_meshnode.h"

namespace SGR
{
    void MeshPointNode::updateBBox( const mat4f& mat )
    {
        ParentFinder<MeshNode3f> finder(this);
        if ( finder.target() )
        {
            MeshNode3f& meshnode = *finder.target();

            float delta = _pointSize/2.f;
            vec4f min = vec4f ( meshnode[_idx].xyz() - delta );
            vec4f max = vec4f ( meshnode[_idx].xyz() + delta );
            min = mat * vec4f ( min );
            max = mat * vec4f ( max );

            _bb.setminmax ( min.xyz(), max.xyz() );
        }

        for ( iterator pp=begin(); pp!=end(); ++pp )
        {
            (*pp)->updateBBox();
            _bb = _bb.unionbox ( (*pp)->getBBox() );
        }

        setBBoxDirty ( false );
    }


    void MeshPointNode::setCoord ( const vec3f& coord )
    {
        // get meshnode
        ParentFinder<MeshNode3f> finder (this);
        if ( finder.target() )
        {
            MeshNode3f& meshnode = *(finder.target());
            meshnode[ _idx ] = coord;
            setBBoxDirty ( true );
            setParentBBoxDirty ( true );
        }
    }

    vec3f& MeshPointNode::getCoord ()
    {
        // get meshnode
        ParentFinder<MeshNode3f> finder (this);
        if ( finder.target() )
        {
            MeshNode3f& meshnode = *(finder.target());
            return meshnode[ _idx ];
        }
        throw std::logic_error ("meshpoint can not find mesh parent");
    }

    vec3f MeshPointNode::getCoord () const
    {
        // get meshnode
        ParentFinder<MeshNode3f> finder (this);
        if ( finder.target() )
        {
            MeshNode3f& meshnode = *(finder.target());
            return meshnode[ _idx ];
        }
        throw std::logic_error ("meshpoint can not find mesh parent");
    }

}

