#ifndef _BBOX_H
#define _BBOX_H

#include "vec3.h"
#include <algorithm>
#include <sstream>
#include <string>

using namespace std;
//#include <tinylog.h>
#include "agef_global.h"

class AGEF_EXPORT BBox
{
public:
    // constructor
    BBox () {}
    BBox ( const BBox& rhs ) { _min=rhs.min(); _max=rhs.max(); }
    BBox ( const vec3f& min, const vec3f& max ) : _min(min), _max(max) {}
    // min/max
    const vec3f& min() const { return _min; }
    const vec3f& max() const { return _max; }
    vec3f& min() { return _min; }
    vec3f& max() { return _max; }
    void init ( const vec3f& v=vec3f() ) { _min = v; _max=v; }
    // other operations
    vec3f center () const { return ( _min + _max ) / 2.f; }
    void translate ( const vec3f& delta )
    {
        _min += delta;
        _max += delta;
    }
    void expandby ( const vec3f& pnt )
    {
        _min = _min.min_element ( pnt );
        _max = _max.max_element ( pnt );
    }
    void setminmax ( const vec3f& min, const vec3f& max )
    {
        _min = min;
        _max = max;
    }
    BBox unionbox ( const BBox& rhs ) const
    {
        if ( !isValidBBox() )
        {
            return rhs;
        }
        else
        {
            BBox t = *this;
            if ( rhs.isValidBBox () )
            {
                t.expandby ( rhs.min() );
                t.expandby ( rhs.max() );
            }
            return t;
        }
    }
    //bool isIntersect ( const BBox& rhs ) const 
    //{
    //    if ( max().x() < rhs.min().x() || max().y() < rhs.min().y() ||
    //        min().x() > rhs.max().x() || min().y() > rhs.max().y() )
    //        return false;
    //    return true;
    //}
    //bool isContain ( const BBox& rhs ) const 
    //{
    //    if ( max().x() >= rhs.max().x() && 
    //        min().x() <= rhs.min().x() &&
    //        max().y() >= rhs.max().y() &&
    //        min().y() <= rhs.min().y() )
    //        return true;
    //    return false;
    //}
    vec3f dimension () const
    {
        return max() - min();
    }
    string dump () const 
    {
        stringstream ss;
        ss << "min=("<<_min.x()<<"," << _min.y() << "," << _min.z() << ")\tmax=(" << _max.x() << "," << _max.y() << "," << _max.z() << ")";
        return ss.str();
    }
    bool operator != ( const BBox& rhs ) const
    {
        if ( _min != rhs.min() || _max != rhs.max() )
            return true;
        return false;
    }
    bool isValidBBox () const
    {
        return (_min != _max);
    }
private:
    vec3f _min, _max;
};



#endif
