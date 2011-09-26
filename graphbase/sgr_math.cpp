#include "sgr_math.h"
#include <iostream>
using namespace std; 

bool is_outside ( const BBox2d& lhs, const BBox2d& rhs )
{
    if (lhs.maxvec().x() < rhs.minvec().x() || 
        lhs.minvec().x() > rhs.maxvec().x() ||
        lhs.maxvec().y() < rhs.minvec().y() || 
        lhs.minvec().y() > rhs.maxvec().y() )
        return true;
    return false;
}

bool is_inside ( const BBox2d& lhs, const BBox2d& rhs )
{
    if (lhs.maxvec().x() <= rhs.maxvec().x() &&
        lhs.minvec().x() >= rhs.minvec().x() &&
        lhs.maxvec().y() <= rhs.maxvec().y() &&
        lhs.minvec().y() >= rhs.minvec().y() )
        return true;
    return false;
}

bool aabb_outside ( float* lhs, const BBox2d& rhs )
{
    if (lhs[2] < rhs.minvec().x() ||
        lhs[0] > rhs.maxvec().x() ||
        lhs[3] < rhs.minvec().y() || 
        lhs[1] > rhs.maxvec().y() )
        return true;
    return false;
}

bool line_outside ( float* line, const BBox2d& rhs )
{
    BBox2d lbbox;
    lbbox.init ( vec2f(line) );
    lbbox.expandby ( vec2f(line+2) );

    if ( is_outside ( lbbox, rhs ) == true )
        return true;

    // second do line_outside test
    vec2f p1 ( line ), p2 ( line+2 );
    vec2f pp = p2 - p1;
    if ( pp.x() == 0 || pp.y() == 0 )
        return false;

    vec2f Normal[4];  // left, right, bottom, top
    Normal[0].xy (  1,  0 );
    Normal[1].xy ( -1,  0 );
    Normal[2].xy (  0,  1 );
    Normal[3].xy (  0, -1 );

    float tl=0, tu=1;

    // left, right, bottom, top
    float t[4] = { ( rhs.minvec().x() - p1.x() ) / pp.x(),
                   ( rhs.maxvec().x() - p1.x() ) / pp.x(),
                   ( rhs.minvec().y() - p1.y() ) / pp.y(),
                   ( rhs.maxvec().y() - p1.y() ) / pp.y() };

    for ( int i=0; i<4; i++ )
    {
        if ( Normal[i].dot(pp) > 0 )
            tl = tl > t[i] ? tl : t[i];
        else
            tu = tu < t[i] ? tu : t[i];
    }

    if ( tl <= tu )
        return false;
    else
        return true;
}

bool line_outside25 ( float* line, const BBox2d& rhs )
{
    BBox2d lbbox;
    lbbox.init ( vec2f(line) );
    lbbox.expandby ( vec2f(line+3) );

    if ( is_outside ( lbbox, rhs ) == true )
        return true;

    // second do line_outside test
    vec2f p1 ( line ), p2 ( line+3 );
    vec2f pp = p2 - p1;
    if ( pp.x() == 0 || pp.y() == 0 )
        return false;

    vec2f Normal[4];  // left, right, bottom, top
    Normal[0].xy (  1,  0 );
    Normal[1].xy ( -1,  0 );
    Normal[2].xy (  0,  1 );
    Normal[3].xy (  0, -1 );

    float tl=0, tu=1;

    // left, right, bottom, top
    float t[4] = { ( rhs.minvec().x() - p1.x() ) / pp.x(),
                   ( rhs.maxvec().x() - p1.x() ) / pp.x(),
                   ( rhs.minvec().y() - p1.y() ) / pp.y(),
                   ( rhs.maxvec().y() - p1.y() ) / pp.y() };

    for ( int i=0; i<4; i++ )
    {
        if ( Normal[i].dot(pp) > 0 )
            tl = tl > t[i] ? tl : t[i];
        else
            tu = tu < t[i] ? tu : t[i];
    }

    if ( tl <= tu )
        return false;
    else
        return true;
}

bool tri_outside ( float* tri, const BBox2d& rhs )
{
    // 1. do box_outside test
    // 2. for each tri.edge do line_outside test
    float tmp[4] = { tri[4], tri[5], tri[0], tri[1] };
    if ( line_outside ( tri, rhs ) &&
         line_outside ( tri+2, rhs ) &&
         line_outside ( tmp, rhs ) )
        return true;
    return false;
}

bool rect_outside ( float* rc, const BBox2d& rhs )
{
    // 1. do box_outside test
    if ( rc[0] <= rhs.minvec().x() &&
	 rc[1] <= rhs.minvec().y() &&
	 rc[4] >= rhs.maxvec().x() &&
	 rc[5] >= rhs.maxvec().y() )
	return false;
    // 2. for each tri.edge do line_outside test
    float tmp[4] = { rc[6], rc[7], rc[0], rc[1] };
    if ( line_outside ( rc, rhs ) &&
         line_outside ( rc+2, rhs ) &&
         line_outside ( rc+4, rhs ) &&
         line_outside ( tmp, rhs ) )
        return true;
    return false;
}

bool rect_outside25 ( float* rc, const BBox2d& rhs )
{
    // 1. do box_outside test
    if ( rc[0] <= rhs.minvec().x() &&
	 rc[1] <= rhs.minvec().y() &&
	 rc[6] >= rhs.maxvec().x() &&
	 rc[7] >= rhs.maxvec().y() )
	return false;
    // 2. for each tri.edge do line_outside test
    float tmp[4] = { rc[9], rc[10], rc[0], rc[1] };
    if ( line_outside25 ( rc, rhs ) &&
         line_outside25 ( rc+3, rhs ) &&
         line_outside25 ( rc+6, rhs ) &&
         line_outside25 ( tmp, rhs ) )
        return true;
    return false;
}

bool poly_outside ( float* poly, int pntcnt, const BBox2d& rhs )
{
    int cnt = pntcnt * 2;
//    pntcnt--;
    for ( int i=0; i<cnt; i+=2 )
    {
        if ( false == line_outside ( poly+i, rhs ) ){
	    return false;
	}
    }
    return true;
}

bool poly_outside25 ( float* poly, int pntcnt, const BBox2d& rhs )
{
    int cnt = pntcnt * 2;
//    pntcnt--;
    for ( int i=0; i<cnt; i+=3 )
    {
        if ( false == line_outside25 ( poly+i, rhs ) ){
	    return false;
	}
    }
    return true;
}
