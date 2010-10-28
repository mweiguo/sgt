#ifndef _CAMERAORTHO_H_
#define _CAMERAORTHO_H_

#include "sgr_mat4.h"
#include "sgr_vec4.h"
#include "sgr_bbox.h"
#include "sgr_nodemgr.h"
#include "sgr_global.h"
#include <tinyLog.h>
namespace SGR
{

class SGR_DLL CameraOrtho : public SGNode
{
public:
    CameraOrtho (const string& name="default");
    const string& name() const { return _name; }
    void name(const string& n) { _name = n; }
    bool dirty() { return _dirty; }
    void dirty( bool v ) { _dirty = v; }
    
        
    // bbox in object-coord;
    BBox viewvolume ()
    {
        BBox box;
        box.init ( (_inversemvmatrix * vec4f(-1, -1, -1, 1)).xyz() );
        box.expandby ( (_inversemvmatrix * vec4f(1, 1, -2, 1 )).xyz() );
        return box;
    }

    // behaviors
    // these interface only modify model-view matrix and  do not support anisotropic scale
    void zoom ( float scale ); 
    void translate ( const vec3f& offset ); 

    const mat4f& mvmatrix () const { return _mvmatrix; }
    const mat4f& inversematrix () const { return _inversemvmatrix; }
    // return value in object-coord
    vec3f position () { return ( _inversemvmatrix * vec4f(0,0,0,0)).xyz(); };
    
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~CameraOrtho () {}
    
//    void setConstraint ( vec3f minTranslate, vec3f maxTranslate, float minScale, float maxScale );
protected:
    void recalcuMatrix ();
private:
    string _name;
    mat4f _mvmatrix, _inversemvmatrix;
    vec3f _translate;
    float _scale;
    short _precision; // 小数点后保留几位小数，如果是保留2位，则为100

    bool _dirty;

/*     vec3f _minTranslate, _maxTranslate; */
/*     float _minScale, _maxScale; */
/*     bool _isInConstraintMode; */
};

inline CameraOrtho::CameraOrtho (const string& name) : _name(name)
{
    _mvmatrix.normalize();
    _inversemvmatrix.normalize();
    _translate.xyz ( 0.f, 0.f, 0.f );
    _precision = 1;
    _scale = _precision;
    dirty ( true );

/*     _minScale = _maxScale = 0; */
/*     _minTranslate = _maxTranslate = vec3f(0,0,0); */
/*     _isInConstraintMode = false; */
}


inline void CameraOrtho::zoom ( float scale )
{
/*     if ( _isInConstraintMode ) */
/*     { */
/* 	if ( scale<=_minScale ) */
/* 	    scale = _minScale; */
/* 	if ( scale>=_maxScale ) */
/* 	    scale = _maxScale; */
/*     } */

    if ( scale < 0 )
        scale = -1/scale;

    _scale = scale;
    recalcuMatrix ();

    //mat4f smat = mat4f::scale_matrix ( scale, scale, scale );
    //_mvmatrix = smat * _mvmatrix;

    //scale = 1.f/scale;
    //smat = mat4f::scale_matrix ( scale, scale, scale );
    //_inversemvmatrix = _inversemvmatrix * smat;

    if ( scale != _precision )
        dirty ( true );

}

inline void CameraOrtho::translate ( const vec3f& off )
{
    vec3f offset = off;
//    LOG_INFO ("CameraOrtho::translate, _isInConstraintMode=%d", _isInConstraintMode);
/*     if ( _isInConstraintMode ) */
/*     { */
/* 	if ( offset.x() <= _minTranslate.x() ) */
/* 	    offset.x( _minTranslate.x() ); */
/* 	if ( offset.y() <= _minTranslate.y() ) */
/* 	    offset.y( _minTranslate.y() ); */
/* 	if ( offset.z() <= _minTranslate.z() ) */
/* 	    offset.z( _minTranslate.z() ); */

/* 	if ( offset.x() >= _maxTranslate.x() ) */
/* 	    offset.x( _maxTranslate.x() ); */
/* 	if ( offset.y() >= _maxTranslate.y() ) */
/* 	    offset.y( _maxTranslate.y() ); */
/* 	if ( offset.z() >= _maxTranslate.z() ) */
/* 	    offset.z( _maxTranslate.z() ); */
/*     } */

    _translate.xyz ( -offset.x(), -offset.y(), -offset.z() );
    recalcuMatrix ();
    //// camera translation and object transltation are opposite 
    //mat4f tmat = mat4f::translate_matrix ( -offset.x(), -offset.y(), -offset.z() );
    //_mvmatrix = tmat * _mvmatrix;

    //tmat = mat4f::translate_matrix ( offset.x(), offset.y(), offset.z() );
    //_inversemvmatrix = _inversemvmatrix * tmat;

    if ( offset != vec3f(0,0,0) )
        dirty ( true );
}

inline void CameraOrtho::recalcuMatrix ()
{
    mat4f tmat = mat4f::translate_matrix ( _translate.x(), _translate.y(), _translate.z() );
    mat4f smat = mat4f::scale_matrix ( _scale, _scale, _scale );
    _mvmatrix = smat * tmat;

    float scale = 1.f / _scale;
    tmat = mat4f::translate_matrix ( -_translate.x(), -_translate.y(), -_translate.z() );
    smat = mat4f::scale_matrix ( scale, scale, scale );
    _inversemvmatrix = tmat * smat;
}

/* inline void CameraOrtho::setConstraint ( vec3f minTranslate, vec3f maxTranslate, float minScale, float maxScale ) */
/* { */
/*     if ( minTranslate == maxTranslate || minScale == maxScale ) */
/*     { */
/* 	_isInConstraintMode = false; */
/*     } */
/*     else */
/*     { */
/* 	_minTranslate = minTranslate; */
/* 	_maxTranslate = maxTranslate; */
/* 	_minScale = minScale; */
/* 	_maxScale = maxScale; */
/* 	_isInConstraintMode = true; */
/*     } */
	
/* } */

}
//typedef NodeMgr<CameraOrtho>      CameraMgr;
#endif
