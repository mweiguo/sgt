#ifndef _SGR_STATESETBUILDER_H_
#define _SGR_STATESETBUILDER_H_

#include "sgr_lc.h"
#include "sgr_stateset.h"

#include <list>
using namespace std;

class StateSetBuilder
{
public:
    static void build ( LC* lc, list<int>& input );
    static void freeStateSets ( StateSet* root );
    static StateSet* root;
};

#endif// _SGR_STATESETBUILDER_H_
