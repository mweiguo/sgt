#ifndef _BBOX2D_H_
#define _BBOX2D_H_

#include "vec2.h"
#include <algorithm>
#include <sstream>
#include <string>

using namespace std;

class BBox2d
{
public:
    // constructor
    BBox2d () { _min = _max + 1; }
    BBox2d ( const float* minmax ) {
	_min.xy ( minmax[0], minmax[1] );
	_max.xy ( minmax[2], minmax[3] );
    }
    BBox2d ( const BBox2d& rhs ) { _min=rhs.minvec(); _max=rhs.maxvec(); }
    BBox2d ( const vec2f& min, const vec2f& max ) : _min(min), _max(max) {}
    // min/max
    const vec2f& minvec() const { return _min; }
    const vec2f& maxvec() const { return _max; }
    vec2f& minvec() { return _min; }
    vec2f& maxvec() { return _max; }
    void init ( const vec2f& v ) { _min = v; _max=v; }
    // other operations
    vec2f center () const { return ( _min + _max ) / 2.f; }
    void translate ( const vec2f& delta )
    {
        _min += delta;
        _max += delta;
    }
    void expandby ( const vec2f& pnt )
    {
        _min = _min.min_element ( pnt );
        _max = _max.max_element ( pnt );
    }
    void setminmax ( const vec2f& min, const vec2f& max )
    {
        _min = min;
        _max = max;
    }
    BBox2d unionbox ( const BBox2d& rhs ) const
    {
        if ( !isValidBBox() )
        {
            return rhs;
        }
        else
        {
            BBox2d t = *this;
            if ( rhs.isValidBBox () )
            {
                t.expandby ( rhs.minvec() );
                t.expandby ( rhs.maxvec() );
            }
            return t;
        }
    }
    BBox2d& operator += ( const BBox2d& rhs )
    {
        if ( !isValidBBox() )
	    *this = rhs; 
        else
	{
            if ( rhs.isValidBBox () )
            {
                this->expandby ( rhs.minvec() );
                this->expandby ( rhs.maxvec() );
            }
        }
	return *this;
    }
    vec2f dimension () const
    {
        return maxvec() - minvec();
    }
    string dump () const 
    {
        stringstream ss;
        ss << "min=("<<_min.x()<<"," << _min.y() << ")\tmax=(" << _max.x() << "," << _max.y() << ")";
        return ss.str();
    }
    bool operator != ( const BBox2d& rhs ) const
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
        _max = vec2f();
        _min = _max+1;
    }
private:
    vec2f _min, _max;
};


#endif //_BBOX2D_H_
