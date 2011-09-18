#ifndef _SGR_MATH_H_
#define _SGR_MATH_H_

#include "bbox2d.h"
#include <list>

bool is_outside ( const BBox2d& lhs, const BBox2d& rhs );
bool is_inside ( const BBox2d& lhs, const BBox2d& rhs );
bool aabb_outside ( float* aabb, const BBox2d& rhs );
bool line_outside ( float* line, const BBox2d& rhs );
bool tri_outside ( float* tri, const BBox2d& rhs );
bool rect_outside ( float* rc, const BBox2d& rhs );
bool poly_outside ( float* poly, int pntcnt, const BBox2d& rhs );

#endif // _SGR_MATH_H_
