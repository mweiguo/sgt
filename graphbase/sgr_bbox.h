#ifndef _BBOX_H
#define _BBOX_H

#include "sgr_vec3.h"
#include <algorithm>
#include <sstream>
#include <string>

using namespace std;
//#include <tinylog.h>
#include "sgr_global.h"

namespace SGR
{

class /*SGR_DLL*/ BBox
{
public:
    // constructor
    BBox () { _min = _max + 1; }
    BBox ( const BBox& rhs ) { _min=rhs.minvec(); _max=rhs.maxvec(); }
    BBox ( const vec3f& min, const vec3f& max ) : _min(min), _max(max) {}
    // min/max
    const vec3f& minvec() const { return _min; }
    const vec3f& maxvec() const { return _max; }
    vec3f& minvec() { return _min; }
    vec3f& maxvec() { return _max; }
    void init ( const vec3f& v ) { _min = v; _max=v; }
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
                t.expandby ( rhs.minvec() );
                t.expandby ( rhs.maxvec() );
            }
            return t;
        }
    }
    //bool isIntersect ( const BBox& rhs ) const 
    //{
    //    if ( maxvec().x() < rhs.minvec().x() || maxvec().y() < rhs.minvec().y() ||
    //        minvec().x() > rhs.maxvec().x() || minvec().y() > rhs.maxvec().y() )
    //        return false;
    //    return true;
    //}
    //bool isContain ( const BBox& rhs ) const 
    //{
    //    if ( maxvec().x() >= rhs.maxvec().x() && 
    //        minvec().x() <= rhs.minvec().x() &&
    //        maxvec().y() >= rhs.maxvec().y() &&
    //        minvec().y() <= rhs.minvec().y() )
    //        return true;
    //    return false;
    //}
    vec3f dimension () const
    {
        return maxvec() - minvec();
    }
    string dump () const 
    {
        stringstream ss;
        ss << "min=("<<_min.x()<<"," << _min.y() << "," << _min.z() << ")\tmax=(" << _max.x() << "," << _max.y() << "," << _max.z() << ")";
        return ss.str();
    }
    bool operator != ( const BBox& rhs ) const
    {
        if ( _min != rhs.minvec() || _max != rhs.maxvec() )
            return true;
        return false;
    }
    bool isValidBBox () const
    {
        return (_min <= _max);
    }
    void setInvalid ()
    {
        _max = vec3f();
        _min = _max+1;
    }
private:
    vec3f _min, _max;
};


}
#endif
