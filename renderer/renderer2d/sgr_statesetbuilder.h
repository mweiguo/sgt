#ifndef _SGR_STATESETBUILDER_H_
#define _SGR_STATESETBUILDER_H_

#include "sgr_lc.h"
#include "sgr_stateset.h"

#include <map>
#include <list>
using namespace std;

class StateSetBuilder
{
public:
    static void build ( LC* lc, list<int>& input );
    static void clear ();
    static StateSet* root;
private:
    static map<StateSet*,StateSet*> _nodes;
};

class StateSetBuilder2
{
public:
    static void build ( LC* lc, list<int>& input, list<StateSet*>& opaques, list<StateSet*>& transparents );
    static void clear ();
    static std::map<MaterialRecord*,std::list<int> > onodes; // opaque nodes
    static std::map<MaterialRecord*,std::list<int> > tnodes; // transparent nodes
};

#endif// _SGR_STATESETBUILDER_H_
