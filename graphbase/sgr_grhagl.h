#ifndef _GRAPH_ALGORITHM_H
#define _GRAPH_ALGORITHM_H

#include "sgr_line.h"
#include "sgr_bbox.h"
#include "sgr_vec3.h"
#include "sgr_global.h"
namespace SGR
{

    bool SGR_DLL is_intersect ( const Linef& lhs, const Linef& rhs );
    bool SGR_DLL is_intersect ( const BBox& lhs, const Linef& rhs );
    bool SGR_DLL is_intersect ( const BBox& lhs, const BBox& rhs );

    bool SGR_DLL is_contain ( const BBox& lhs, const BBox& rhs );
    bool SGR_DLL is_contain ( const BBox& lhs, const vec3f& rhs );

    bool SGR_DLL is_separate ( const BBox& lhs, const BBox& rhs );
    bool SGR_DLL is_separate ( const BBox& lhs, const vec3f& rhs );

    // reference;  http://zh.wikipedia.org/zh-cn/%E5%90%91%E9%87%8F%E7%A7%AF#.E7.9F.A9.E9.98.B5.E5.BD.A2.E5.BC.8F
    template < class T >
    inline vec3<T> SGR_DLL cross ( const vec3<T>& lhs, const vec3<T>& rhs )
    {
        return vec3<T> (
            lhs.y() * rhs.z() - rhs.y() * lhs.z(), 
            lhs.z() * rhs.x() - lhs.x() * rhs.z(),
            lhs.x() * rhs.y() - rhs.x() * lhs.y() );
    } 

    // reference; http://zh.wikipedia.org/zh-cn/%E6%95%B0%E9%87%8F%E7%A7%AF#.E4.B8.A4.E7.A7.8D.E5.AE.9A.E4.B9.89.E7.9A.84.E7.AD.89.E4.BB.B7.E6.80.A7.E7.9A.84.E8.AF.81.E6.98.8E
    template < class T >
    inline T SGR_DLL dot ( const vec3<T>& lhs, const vec3<T>& rhs )
    {
        return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
    } 

    //// for kdtree
    //// objtype /* KDT_RECTANGLE */
    void kdtree_create ( int id, int objtype );
    void kdtree_add ( int id, void* objptr, int objN );
    void kdtree_buildidx ( int id );
    void kdtree_clearall ( int id );
    bool kdtree_intersect ( int id, float minx, float miny, float minz, float maxx, float maxy, float maxz, void* objptr, int& objN );

}
#endif // _GRAPH_ALGORITHM_H
