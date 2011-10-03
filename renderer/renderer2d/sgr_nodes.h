#ifndef _SLC_NODES_H_
#define _SLC_NODES_H_

#ifdef _SUPPORT_PYTHON_
#include <Python.h>
#endif

#include <list>
#include <vector>
#include <string>
#include <vec3.h>
#include "sgr_nodetypes.h"

//PYTHON_BUILD(#include <Python.h>)

using namespace std;

class SLCNode
{
public:
#ifdef _SUPPORT_PYTHON_
    PyListObject _list;
#endif
    //    PYTHON_BUILD(PyListObject list;)
    
    SLCNode () : parent(0) {}
    
    virtual int getType() = 0;
    virtual string toXML () const = 0;
    int getDepth();
    void addChild ( SLCNode* node );
    void push_front_child ( SLCNode* node );

    SLCNode* parent;
    list<SLCNode*> children;
};

class SLCSceneNode : public SLCNode
{
public:
    SLCSceneNode ( const char* gname );
    virtual int getType() { return SLC_SCENE; }
    virtual string toXML () const;
    string name;
};

class SLCMaterial : public SLCNode
{
public:
    enum LINETYPE {
	LINETYPE_SOLID,
	LINETYPE_DASH
    };
    SLCMaterial ( const char* name );
    virtual int getType () { return SLC_MATERIAL; }
    virtual string toXML () const;
    string name;
    vec3i foreground_color;
    vec3i background_color;
    int linetype;
    float linewidth;
    string fontfilename;
    string texturefilename;
};

class SLCLayerNode : public SLCNode
{
public:
    SLCLayerNode ( const char* lname, SLCMaterial* bindmat );
    virtual int getType() { return SLC_LAYER; }
    virtual string toXML () const;
    string name;
    bool visible;
    SLCMaterial* bindmat;
};

class SLCLODNode : public SLCNode
{
public:
    SLCLODNode ();
    virtual int getType() { return SLC_LOD; }
    virtual string toXML () const;
    list<float> scales;
};

class SLCLODPageNode : public SLCNode
{
public:
    SLCLODPageNode ();
    virtual int getType() { return SLC_LODPAGE; }
    virtual string toXML () const;
    bool delayloading;
    bool imposter;
    string kdtree;
};

class SLCPrimitiveNode : public SLCNode
{
public:
    SLCPrimitiveNode ( SLCMaterial* mat );
    virtual int getType() = 0;
    virtual string toXML () const = 0;
    SLCMaterial* bindmat;
};

class SLCFillablePrimitiveNode : public SLCPrimitiveNode
{
public:
    SLCFillablePrimitiveNode ( SLCMaterial* mat ) : SLCPrimitiveNode(mat) { 
	textureScale = 0;
	textureAngle = 0;
	filltexture = false;
    }
    virtual string toXML () const;
    float textureScale;
    float textureAngle;
    bool filltexture;
};

class SLCLineNode : public SLCNode
{
public:
    SLCLineNode () : SLCNode() {}
    virtual int getType() { return SLC_LINE; }
    virtual string toXML () const;
    vec3f pnts[2];
};

class SLCTriNode : public SLCNode
{
public:
    SLCTriNode () : SLCNode () {}
    virtual int getType () { return SLC_TRIANGLE; }
    virtual string toXML () const;
    vec3f pnts[3];
};

class SLCRectNode : public SLCFillablePrimitiveNode
{
public:
    SLCRectNode ( SLCMaterial* mat );
    virtual int getType () { return SLC_RECT; }
    virtual string toXML () const;
    vec2f pnts[4];
    float z;
};

class SLCTextNode : public SLCPrimitiveNode
{
public:
    SLCTextNode ( SLCMaterial* mat );
    virtual int getType () { return SLC_TEXT; }
    virtual string toXML () const;
    vec3f pos;
    float scale;
    float rotz;
    string text;
};

class SLCPLineNode : public SLCPrimitiveNode
{
public:
    SLCPLineNode ( SLCMaterial* mat );
    virtual int getType () { return SLC_PLINE; }
    virtual string toXML () const;
    vector<vec2f> pnts;
    float z;
};

class SLCPolyNode : public SLCFillablePrimitiveNode
{
public:
    SLCPolyNode ( SLCMaterial* mat );
    virtual int getType () { return SLC_POLY; }
    virtual string toXML () const;
    vector<vec2f> pnts;
    float z;
};

class SLCTransformNode : public SLCNode
{
public:
    SLCTransformNode ();
    virtual int getType () { return SLC_TRANSFORM; }
    virtual string toXML () const;
    float mat[16];
};
#endif

