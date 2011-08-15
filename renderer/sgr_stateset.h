#ifndef _SST_STATESET_H_
#define _SST_STATESET_H_

#include <vector>
#include <list>
#include <string>
#include "sgr_state.h"

using namespace std;

class SSTStateSet
{
public:
    SSTStateSet ();
    SSTStateSet* parent;
    list<SSTStateSet*> children;
    vector<SSTState> states;
    vector<int> meshes;

    void addChild ( SSTStateSet* ss );
    void addMesh ( int meshIndex );
    void render ();
    void applyStates ();
    void draw ();
    string toXML() const;
};

#endif // _SST_STATESET_H_

// map<int,SSTStateSet*>

