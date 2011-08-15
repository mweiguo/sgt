#ifndef _SLC_NODES_H_
#define _SLC_NODES_H_

#include <list>
#include <vector>
#include <string>
#include <vec3.h>
#include "sgr_lc.h"
using namespace std;

class SLCNode
{
public:
    SLCNode () : parent(0) {}
    
    virtual int getType() = 0;
    virtual string toXML () const = 0;
    int getDepth();
    void addChild ( SLCNode* node );

    SLCNode* parent;
    list<SLCNode*> children;
};

class SLCGroupNode : public SLCNode
{
public:
    SLCGroupNode ( const char* gname );
    virtual int getType() { return SLC_GROUP; }
    virtual string toXML () const;
    char name[32];
};

class SLCLayerNode : public SLCNode
{
public:
    SLCLayerNode ( const char* lname );
    virtual int getType() { return SLC_LAYER; }
    virtual string toXML () const;
    char name[32];
    int flags;
};

// class SLCPLineNode : public SLCNode
// {
// public:
//     SLCPLineNode () : SLCNode() {}
//     virtual int getType() { return SLC_PLINE; }
//     vector < vec3f > pnts;
// };

// class SLCPolyNode : public SLCNode
// {
// public:
//     SLCPolyNode () : SLCNode() {}
//     virtual int getType () { return SLC_POLY; }
//     vector < vec3f > pnts;
// };

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

class SLCQuadNode : public SLCNode
{
public:
    SLCQuadNode () : SLCNode () {}
    virtual int getType () { return SLC_QUAD; }
    virtual string toXML () const;
    vec3f pnts[4];
};

class SLCModel : public SLCNode
{
public:
    SLCModel ( const char* name );
    virtual int getType () { return SLC_MODEL; }
    virtual string toXML () const;
    char name[32];
};

class SLCTexture : public SLCNode
{
public:
    SLCTexture ( const char* Name, const char* fileName );
    virtual int getType () { return SLC_TEXTURE; }
    virtual string toXML () const;
    char name[32];
    char filename[32];
};

class SLCAppearance : public SLCNode
{
public:
    SLCAppearance () : SLCNode() {}
    virtual int getType () { return SLC_APPEARANCE; }
    virtual string toXML () const;
};

class SLCMaterial : public SLCNode
{
public:
    virtual int getType () = 0;
    virtual string toXML () const;
    vec3f diffuse;
    vec3f ambient;
    vec3f specular;
};

class SLCPhongMaterial : public SLCMaterial
{
public:
    virtual int getType () { return SLC_PHONG_MATERIAL; }
};

// class SLCBlinnMaterial : public SLCMaterial
// {
// public:
//     virtual int getType () { return SLC_BLINN_MATERIAL; }
// };

class SLCMesh : public SLCNode
{
public:
    virtual int getType () { return SLC_MESH; }
    virtual string toXML () const;
    int vertexOffset;
    int normalOffset;
    int texcoordOffset;
};

class SLCVertex : public SLCNode
{
public:
    virtual int getType () { return SLC_VERTEX; }
    virtual string toXML () const;
    vector<vec3f> data;
};

class SLCNormal : public SLCNode
{
public:
    virtual int getType () { return SLC_NORMAL; }
    virtual string toXML () const;
    vector<vec3f> data;
};

class SLCTexCoord : public SLCNode
{
public:
    virtual int getType () { return SLC_TEXCOORD; }
    virtual string toXML () const;
    vector<vec2f> data;
};

class SLCVertexIndex : public SLCNode
{
public:
    virtual int getType () { return SLC_VERTEXINDEX; }
    virtual string toXML () const;
    vector<int> data;
};

class SLCNormalIndex : public SLCNode
{
public:
    virtual int getType () { return SLC_NORMALINDEX; }
    virtual string toXML () const;
    vector<int> data;
};

class SLCTexCoordIndex : public SLCNode
{
public:
    virtual int getType () { return SLC_TEXCOORDINDEX; }
    virtual string toXML () const;
    vector<int> data;
};

class SLCTransform : public SLCNode
{
public:
    virtual int getType () { return SLC_TRANSFORM; }
    virtual string toXML () const;
    float mat[16];
};

#endif

