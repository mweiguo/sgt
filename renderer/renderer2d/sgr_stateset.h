#ifndef _SGR_STATESET_H_
#define _SGR_STATESET_H_
#include <vector>
#include <string>
using namespace std;
#include <vec3.h>

class State
{
public:
    enum StateFlag
    {
	INHERIT,
	OVERWRITE
    };
    enum StateType
    {
	FOREGROUND_COLOR,
	BACKGROUND_COLOR,
	LINE_TYPE,
	LINE_WIDTH,
	TEXTURE
    };
    State ( StateType t, int value, StateFlag f );
    State ( StateType t, float value, StateFlag f );
    State ( StateType t, vec3i value, StateFlag f );
    State ( const State& rhs );
    void applyState ();
    StateFlag flag;
    StateType type;
    int intValue;
    float floatValue;
    vec3i vec3iValue;
};

class LC;
struct MaterialRecord;
class StateSet
{
public:
    StateSet* parent;
    vector<StateSet*> children;
    vector<State> states;
    vector<int> renderObjects;

    StateSet ();
    StateSet* CreateOrReuseStateSet ( LC* lc, MaterialRecord* mat );
    int getMatchCount ( MaterialRecord* mat );    
    void addState ( const State& rhs );
    State* getState ( State::StateType type );
    State* getInheritState ( State::StateType type );

    void addChild ( StateSet* ss );
    string toXML () const;

    void pushAttributes ();
    void popAttributes ();
    void applyStates ();
    void render ( LC* lc );
};


#endif // _SGR_STATESET_H_
