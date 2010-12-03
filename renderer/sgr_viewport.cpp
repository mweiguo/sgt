#include "sgr_viewport.h"
#include "sgr_nodemgr.h"

namespace SGR
{
Viewport::Viewport ( const string& name, int x, int y, int w, int h ) : 
_name(name), _pos(x,y), _size(w,h)
{
    _dirty = true;
    recalcVPMatrix ();
}

Viewport::~Viewport ()
{
}

void Viewport::attachcamera ( int camid )
{
    _camid = camid;
}

CameraOrtho* Viewport::camera() const 
{
    return NodeMgr::getInst().getNodePtr<CameraOrtho>(_camid);
}

int Viewport::cameraid() const
{
    return _camid;
}

void Viewport::attachproj ( int projid )
{
    _projid = projid;
}

Projection* Viewport::projection () const
{
    return NodeMgr::getInst().getNodePtr<Projection>(_projid);
}

int Viewport::projid() const
{
    return _projid;
}
//RnederList& getRenderList ( const string& cameraname );

bool Viewport::dirty()
{
    return _dirty; 
}

void Viewport::dirty( bool v ) 
{
    _dirty = v; 
}

void Viewport::position ( int orgx, int orgy )
{
    _pos.x(orgx);
    _pos.y(orgy); 
}

vec2i Viewport::position () const
{
    return _pos;
}

vec2i& Viewport::position ()
{
    return _pos;
}

const vec2i& Viewport::viewportSize() const
{ 
    return _size; 
}

void Viewport::viewportSize ( int w, int h )
{ 
    _size.w(w); 
    _size.h(h); 
    dirty(true); 
    recalcVPMatrix(); 
}
const mat4f& Viewport::vpmatrix () const
{ 
    return _vpmatrix; 
}

const mat4f& Viewport::inversematrix () const
{
    return _inversemvmatrix; 
}

void Viewport::update () 
{
    CameraOrtho* cam = camera();
    if ( cam )
        cam->dirty(true);
}

void Viewport::name ( const string& nm )
{
    _name = nm; 
}

const string& Viewport::name () const
{
    return _name; 
}

string Viewport::name ()
{
    return _name; 
}

// reset view port matrix
void Viewport::recalcVPMatrix () 
{
    // transform first, then scale
    _vpmatrix.normalize();
    mat4f tmatrix = mat4f::translate_matrix ( _pos.x(), _pos.y(), 0 );
    mat4f smatrix = mat4f::scale_matrix ( _size.w(), _size.h(), 0 );
    _vpmatrix = tmatrix * smatrix;

    _inversemvmatrix.normalize();
    tmatrix = mat4f::translate_matrix ( -_pos.x(), -_pos.y(), 0 );
    smatrix = mat4f::scale_matrix ( 1.0f/_size.w(), 1.0f/_size.h(), 0 );
    _inversemvmatrix = smatrix * tmatrix;

    /*
    // transform first, then scale
    _vpmatrix.normalize();
    //mat4f tmatrix = mat4f::translate_matrix ( _size.w() / 2.f, _size.h() / 2.f, 0 );
    mat4f tmatrix = mat4f::translate_matrix ( (_pos.x() + _size.w()) / 2.f, (_pos.y() + _size.h()) / 2.f, 0 );
    mat4f smatrix = mat4f::scale_matrix ( _size.w() / 2.f, _size.h() / -2.f, 0 );
    _vpmatrix = smatrix * tmatrix;

    _inversemvmatrix.normalize();
    //tmatrix = mat4f::translate_matrix ( _size.w() / -2.f, _size.h() / -2.f, 0 );
    tmatrix = mat4f::translate_matrix ( (_pos.x() + _size.w()) / -2.f, (_pos.y() + _size.h()) / -2.f, 0 );
    smatrix = mat4f::scale_matrix ( 2.f / _size.w(), -2.f / _size.h(), 0 );
    _inversemvmatrix = tmatrix * smatrix;
    */
}

}
