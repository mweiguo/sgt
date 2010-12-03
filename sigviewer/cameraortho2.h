#ifndef _CAMERAORTHO2_H_
#define _CAMERAORTHO2_H_

class CameraOrtho2 : public SGNode
{
public:
    CameraOrtho2 (const string& name="default");
    void translate ( float dx, float dy, float dz );

    // modelview matrix
    void lookat ( const vec3f& pos, const vec3f& tar, const vec3f& right );
    void position ( const vec3f& pos ); 
    void target ( const vec3f& tar );
    void righthand ( const vec3f& r );

    // projection matrix
    void setortho ( float left, float top, float right, float bottom, float near, float far );

    // matrix getter function
    const mat4f& mvmatrix () const { return _mvmatrix; }
    const mat4f& inversematrix () const { return _inversemvmatrix; }

    const mat4f& projmatrix () const { return _projmatrix; }
    const mat4f& inverseprojmatrix () const { return _inverseprojmatrix; }

protected:
    void calcMVMatrix();

private:
    int _mvid, _projid, _invmvid, _invprojid;

    vec3f _pos, _tar, _right;
    SGR::mat4f _mvmatrix, _inversemvmatrix;
    SGR::mat4f _projmatrix, _inverseprojmatrix;
    
};


#endif // _CAMERAORTHO2_H_
