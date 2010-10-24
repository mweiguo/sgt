#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include <string>
#include "sgr_vec2.h"
#include "sgr_cameraortho.h"
#include "sgr_projection.h"
namespace SGR
{

class Viewport : public SGNode
{
public:
    Viewport ( const string& name="default", int x=0, int y=0, int w=400, int h=300 );
    virtual ~Viewport ();

    void attachcamera ( int camid );
    CameraOrtho* camera() const;
    int cameraid() const;

    void attachproj ( int projid );
    Projection* projection () const;
    int projid() const;

    //RnederList& getRenderList ( const string& cameraname );
    bool dirty();
    void dirty( bool v );


    void position ( int orgx, int orgy );
    const vec2i& viewportSize() const;
    void viewportSize ( int w, int h );
    const mat4f& vpmatrix () const;
    const mat4f& inversematrix () const;
    void update ();
    void name ( const string& nm );
    const string& name () const;
    string name ();

    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
private:
    // reset view port matrix
    void recalcVPMatrix (); 

private:
    string _name;

    vec2i _pos, _size;

    int _camid, _projid;
    bool _dirty;

    // auto gen
    mat4f _vpmatrix, _inversemvmatrix;


};

}
#endif

