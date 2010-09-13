#ifndef _POLYLINE_H_
#define _POLYLINE_H_

#include "sgr_bbox.h"
#include "sgr_vec2.h"
#include "sgr_vec3.h"

namespace SGR
{

    template < class T >
    class /*SGR_DLL*/ Polyline : public std::list< T >
    {
    public:
        Polyline () {}
        Polyline ( const Polyline& rhs ) : std::list<T>(rhs) {}

        ~Polyline () {}
        BBox boundingbox ()
        {
            BBox bb;
            if ( empty() )
                return bb;

            bb.init ( front() );
            for ( list< T >::iterator pp=begin(); pp!=end(); ++pp )
                bb.expandby ( *pp );

            return bb;
        }
    };


    typedef Polyline<vec2f>  Polyline2Df;
    typedef Polyline<vec2d>  Polyline2Dd;
    typedef Polyline<vec2i>  Polyline2Di;
    typedef Polyline<vec2s>  Polyline2Ds;
    typedef Polyline<vec3f>  Polyline3Df;
    typedef Polyline<vec3d>  Polyline3Dd;
    typedef Polyline<vec3i>  Polyline3Di;
    typedef Polyline<vec3s>  Polyline3Ds;

}
#endif // _LINE_H_
