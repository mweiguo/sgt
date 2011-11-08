#ifndef _SGR_STATESETBUILDER_H_
#define _SGR_STATESETBUILDER_H_

#include "sgr_lc.h"
#include "sgr_stateset.h"

#include <map>
#include <vector>
#include <list>
//using namespace std;

class StateSetBuilder
{
public:
    static void build ( LC* lc, std::vector<int>& input );
    static void clear ();
    static StateSet* root;
private:
    static std::map<StateSet*,StateSet*> _nodes;
};

class StateSetBuilder2
{
public:
    static void build ( LC* lc, std::vector<int>& input, std::vector<StateSet*>& opaques, std::vector<StateSet*>& transparents );
    static void clear ();
    static std::map<MaterialRecord*,std::vector<int> > onodes; // opaque nodes
    static std::map<MaterialRecord*,std::vector<int> > tnodes; // transparent nodes
};

#endif// _SGR_STATESETBUILDER_H_
