#include "cameraortho2.h"

CameraOrtho2::CameraOrtho2 (const string& name="default")
{
    _mvid = SGR::SeedGenerator::getInst().maxseed();
    _invmvid = SGR::SeedGenerator::getInst().maxseed();
    _projid = SGR::SeedGenerator::getInst().maxseed();
    _invprojid = SGR::SeedGenerator::getInst().maxseed();

    camera_create ( _mvid );
    camera_create ( _invmvid );
    projection_create ( _projid );
    projection_create ( _invprojid );
}

void CameraOrtho2::translate ( float dx, float dy, float dz )
{
    SGR::vec3f dir = _tar.normal();
    _pos += dir * SGR::vec3f(dx, dy, dz);
}

void CameraOrtho2::lookat ( const vec3f& pos, const vec3f& tar, const vec3f& right )
{
    _pos = pos;
    _tar = tar;
    _right = right;
    calcMVMatrix();
}

void CameraOrtho2::position ( const vec3f& pos )
{
    _pos = pos;
    calcMVMatrix();
}
 
void CameraOrtho2::target ( const vec3f& tar )
{
    _tar = tar;
    calcMVMatrix();
}

void CameraOrtho2::righthand ( const vec3f& r )
{
    _right = right;
    calcMVMatrix();
}


void CameraOrtho2::setortho ( float left, float top, float right, float bottom, float near, float far )
{
    _projmatrix = ortho_matrix ( left, top, right, bottom, near, far );
    _inverseprojmatrix = ortho_inv_matrix ( left, top, right, bottom, near, far );
}


void CameraOrtho2::calcMVMatrix()
{
}
