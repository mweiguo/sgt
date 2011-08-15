#ifndef _SST_BUILDER_H_
#define _SST_BUILDER_H_

#include <map>
#include <list>
#include "sgr_stateset.h"
#include "sgr_lc.h"

using namespace std;

class SSTStateSet;

class SSTStateSetWeight
{
public:
    SSTStateSetWeight ();
    SSTStateSetWeight* parent;
    vector<SSTStateSetWeight*> children;
    int value; // match count

    void addChild ( SSTStateSetWeight* );
};


class SSTBuilder
{
public:
    SSTBuilder ();
    // build from LC
    void buildFromLC ( LC& lc );
    // incremental build interface
    void begin();
    void end();

    void addTexture ( int tex );
    void addAmbient ( float r, float g, float b, float a );
    void addDiffuse ( float r, float g, float b, float a );
    void addSpecular (  float r, float g, float b, float a );
    void addMatrix (  float* mat );
    void addMesh ( int meshIndex );

    void save ( const char* filename );

    SSTStateSet root;
protected:
    void traverse ( LC& lc );
    void parseAppearance ( LC& lc );

private:
    void collectWeight ( SSTStateSet* node, SSTStateSetWeight* wnode );
    map<SSTStateSet*, SSTStateSetWeight*> leaves;
    SSTStateSetWeight rootWeight;
    list<SSTState> preStates;
    vector<int> preMeshes;
    map<string,int> texManager;
};

#endif // _SST_BUILDER_H_
