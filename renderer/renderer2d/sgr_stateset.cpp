#include "sgr_stateset.h"
#include "sgr_lc.h"
#include <sstream>

//================================================================================

State::State ( StateType t, int value, StateFlag f )
{
    type = t;
    intValue = value;
    flag = f;
}

//================================================================================

State::State ( StateType t, float value, StateFlag f )
{
    type = t;
    floatValue = value;
    flag = f;
}

//================================================================================

State::State ( StateType t, vec3i value, StateFlag f )
{
    type = t;
    vec3iValue = value;
    flag = f;
}

//================================================================================

State::State ( const State& rhs )
{
    type = rhs.type;
    flag = rhs.flag;

    intValue = rhs.intValue;
    floatValue = rhs.floatValue;
    vec3iValue = rhs.vec3iValue;
}

//================================================================================

StateSet::StateSet ()
{
    parent = NULL;
}

/**
 * if return NULL, stateset not matched
 * else return a stateset
 */
StateSet* StateSet::CreateOrReuseStateSet ( MaterialRecord* mat )
{
    if ( parent == NULL ) // root
    {
	for ( int i=0; i<children.size(); i++ )
	{
	    StateSet* t = children[i]->CreateOrReuseStateSet ( mat );
	    if ( t != NULL )
		return t;
	} 
	// if this is leaf, or this is not leaf & all children not match
	StateSet* tss = new StateSet ();
	tss->addState ( State(State::BACKGROUND_COLOR, mat->background_color, State::INHERIT) );
	tss->addState ( State(State::FOREGROUND_COLOR, mat->foreground_color, State::INHERIT) );
	tss->addState ( State(State::LINE_TYPE, mat->linetype,   State::INHERIT) );
	tss->addState ( State(State::LINE_WIDTH, mat->linewidth, State::INHERIT) );
	addChild ( tss );
	return tss;
    }
    else
    {
	int cnt = 0;
	StateSet nss;
	State* state;

	/**
	 * matchCount == 0        create new StateSet as this's sibling
	 * matchCount < max(4)    create a new StateSet(nss) as this's descendant
	 * matchCount = max(4)    just return this
	 */

	// calculate match count
	if ( (state=getState ( State::BACKGROUND_COLOR )) != NULL ) {
	    if ( state->vec3iValue != mat->background_color ) 
		nss.addState ( State(State::BACKGROUND_COLOR, mat->background_color, State::OVERWRITE) );
	    else
		cnt ++;
	}
	if ( (state=getState ( State::FOREGROUND_COLOR )) != NULL ) {
	    if ( state->vec3iValue != mat->foreground_color )
		nss.addState ( State(State::FOREGROUND_COLOR, mat->foreground_color, State::OVERWRITE) );
	    else
		cnt ++;
	}
	if ( (state=getState ( State::LINE_TYPE )) != NULL ) {
	    if ( state->intValue != mat->linetype )
		nss.addState ( State(State::LINE_TYPE, mat->linetype, State::OVERWRITE) );
	    else
		cnt ++;
	}
	if ( (state=getState ( State::LINE_WIDTH )) != NULL ) {
	    if ( state->floatValue != mat->linewidth )
		nss.addState ( State(State::LINE_WIDTH, mat->linewidth, State::OVERWRITE) );
	    else
		cnt ++;
	}

	if ( cnt == 0 ) {
	    return NULL;
	} else if ( cnt == 4 ) {
	    return this;
	} else if ( cnt < 4 ) {
	    if ( cnt < parent->getMatchCount(mat) )
		return NULL;

	    // if this node not leaf
	    for ( int i=0; i<children.size(); i++ )
	    {
		StateSet* t = children[i]->CreateOrReuseStateSet ( mat );
		if ( t != NULL )
		    return t;
	    }

	    // if this node is leaf, or this is not leaf & all children not match
	    StateSet* tss = new StateSet ( nss );
	    addChild ( tss );
	    return tss;
	}
    }
}

//================================================================================

int StateSet::getMatchCount ( MaterialRecord* mat )
{
    int cnt = 0;
    StateSet nss;
    State* state;
    if ( (state=getState(State::BACKGROUND_COLOR))!=NULL )
	if ( state->vec3iValue == mat->background_color )
	    cnt ++;
    if ( (state=getState ( State::FOREGROUND_COLOR)) != NULL )
	if ( state->vec3iValue == mat->foreground_color )
	    cnt ++;
    if ( (state=getState ( State::LINE_TYPE ))!= NULL )
	if ( state->intValue == mat->linetype )
	    cnt ++;
    if ( (state=getState ( State::LINE_WIDTH )) != NULL )
	if ( state->floatValue == mat->linewidth )
	    cnt ++;
    return cnt;
}

//================================================================================

void StateSet::addState ( const State& rhs )
{
    states.push_back ( rhs );
}

//================================================================================

State* StateSet::getState ( State::StateType type )
{
    for ( vector<State>::iterator pp=states.begin(); pp!=states.end(); ++pp )
	if ( pp->type == type )
	    return &(*pp);
    if ( parent )
	return parent->getInheritState ( type );
    else
	return NULL;
}

//================================================================================

State* StateSet::getInheritState ( State::StateType type )
{
    for ( vector<State>::iterator pp=states.begin(); pp!=states.end(); ++pp )
	if ( pp->type==type && pp->flag ==State::INHERIT )
	    return &(*pp);
    if ( parent )
	return parent->getInheritState ( type );
    else
	return NULL;
}

//================================================================================

string StateSet::toXML () const
{
    stringstream ss;
    if ( parent == NULL )
	ss << "<stateset_root>" << endl;

    ss << "<ss";
    for ( int i=0; i<states.size(); i++ ) {
	const State& state = states[i];
	switch ( state.type )
	{
	case State::FOREGROUND_COLOR:
	    ss << " FOREGROUND_COLOR=\"" << state.vec3iValue.x() << ' ' << state.vec3iValue.y() << ' ' << state.vec3iValue.z() << "\"";
	    break;
	case State::BACKGROUND_COLOR:
	    ss << " BACKGROUND_COLOR=\"" << state.vec3iValue.x() << ' ' << state.vec3iValue.y() << ' ' << state.vec3iValue.z() << "\"";
	    break;
	case State::LINE_TYPE:
	    ss << " LINE_TYPE=\"" << state.intValue << "\"";
	    break;
	case State::LINE_WIDTH:
	    ss << " LINE_WIDTH=\"" << state.floatValue << "\"";
	    break;
	}
    }
    ss << ">" << endl;
    for ( int i=0; i<renderObjects.size(); i++ )
	ss << renderObjects[i] << ' ';
    if ( renderObjects.empty() == false )
	ss << endl;
    for ( vector<StateSet*>::const_iterator pp=children.begin(); pp!=children.end(); ++pp )
	ss << (*pp)->toXML();
    ss << "</ss>" << endl;

    if ( parent == NULL )
	ss << "</stateset_root>" << endl;

    return ss.str();
}

//================================================================================

void StateSet::addChild ( StateSet* ss )
{
//     // do state inherit 
//     for ( size_t i=0; i<states.size(); i++ )
//     {	
// 	State& lhs = states[i];
// 	if ( lhs.flag == State::INHERIT )
// 	{
// 	    State* rhs = ss->getState ( lhs.type );
// 	    if ( rhs == NULL )
// 		ss->addState ( lhs );
// 	    else {
// 		if ( rhs->flag != State::OVERWRITE ) {
// 		    rhs->intValue = lhs.intValue;
// 		    rhs->floatValue = lhs.floatValue;
// 		    rhs->vec3iValue = lhs.vec3iValue;
// 		}
// 	    }
// 	}
//     }
    children.push_back ( ss );
    ss->parent = this;
}
