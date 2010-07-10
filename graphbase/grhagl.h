#ifndef _GRAPH_ALGORITHM_H
#define _GRAPH_ALGORITHM_H

#include "line.h"
#include "bbox.h"
#include "vec3.h"
#include "agef_global.h"

bool AGEF_EXPORT is_intersect ( const Linef& lhs, const Linef& rhs );
bool AGEF_EXPORT is_intersect ( const BBox& lhs, const Linef& rhs );
bool AGEF_EXPORT is_intersect ( const BBox& lhs, const BBox& rhs );

bool AGEF_EXPORT is_contain ( const BBox& lhs, const BBox& rhs );

// reference;  http://zh.wikipedia.org/zh-cn/%E5%90%91%E9%87%8F%E7%A7%AF#.E7.9F.A9.E9.98.B5.E5.BD.A2.E5.BC.8F
template < class T >
inline vec3<T> AGEF_EXPORT cross ( const vec3<T>& lhs, const vec3<T>& rhs )
{
    return vec3<T> (
        lhs.y() * rhs.z() - rhs.y() * lhs.z(), 
        lhs.z() * rhs.x() - lhs.x() * rhs.z(),
        lhs.x() * rhs.y() - rhs.x() * lhs.y() );
} 

// reference; http://zh.wikipedia.org/zh-cn/%E6%95%B0%E9%87%8F%E7%A7%AF#.E4.B8.A4.E7.A7.8D.E5.AE.9A.E4.B9.89.E7.9A.84.E7.AD.89.E4.BB.B7.E6.80.A7.E7.9A.84.E8.AF.81.E6.98.8E
template < class T >
inline T AGEF_EXPORT dot ( const vec3<T>& lhs, const vec3<T>& rhs )
{
    return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
} 

#endif // _GRAPH_ALGORITHM_H
