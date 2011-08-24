#ifndef _SLC_NODES_H_
#define _SLC_NODES_H_

#include <list>
#include <vector>
#include <string>
#include <vec3.h>
#include "sgr_nodetypes.h"
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

class SLCSceneNode : public SLCNode
{
public:
    SLCSceneNode ( const char* gname );
    virtual int getType() { return SLC_SCENE; }
    virtual string toXML () const;
    char name[32];
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
};

class SLCLayerNode : public SLCNode
{
public:
    SLCLayerNode ( const char* lname, SLCMaterial* bindmat );
    virtual int getType() { return SLC_LAYER; }
    virtual string toXML () const;
    char name[32];
    int flags;
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

class SLCRectNode : public SLCPrimitiveNode
{
public:
    SLCRectNode (SLCMaterial* mat) : SLCPrimitiveNode (mat) {}
    virtual int getType () { return SLC_RECT; }
    virtual string toXML () const;
    vec2f pnts[4];
};

#endif

