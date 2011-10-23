#include "sgr_stateset.h"
#include "sgr_lc.h"
#include "sgr_nodetypes.h"
#include <sstream>
#include <algorithm>
#include <windows.h>
#include <GL/gl.h>
#include "sgr_font.h"
#include "sgr_texture.h"

#ifndef MAKEWORD
#define MAKEWORD(a, b)      ((WORD)(((BYTE)((DWORD_PTR)(a) & 0xff)) | ((WORD)((BYTE)((DWORD_PTR)(b) & 0xff))) << 8))
#endif

#ifndef MAKELONG
#define MAKELONG(a, b)      ((LONG)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))
#endif

#ifndef LOWORD
#define LOWORD(l)           ((WORD)((DWORD_PTR)(l) & 0xffff))
#endif

#ifndef HIWORD
#define HIWORD(l)           ((WORD)((DWORD_PTR)(l) >> 16))
#endif

extern float currentScale;

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

State::State ( StateType t, vec4i value, StateFlag f )
{
    type = t;
    vec4iValue = value;
    flag = f;
}

//================================================================================

State::State ( const State& rhs )
{
    type = rhs.type;
    flag = rhs.flag;

    intValue = rhs.intValue;
    floatValue = rhs.floatValue;
    vec4iValue = rhs.vec4iValue;
}

//================================================================================

void State::applyState ()
{
    switch ( type )
    {
    case FOREGROUND_COLOR:
//  	cout << "State::applyState, FOREGROUND_COLOR" << vec4iValue.x() << ", " << vec4iValue.y() << ", " << vec4iValue.z() << endl;
// 	glColor3f ( vec4iValue.x()/255.0f, vec4iValue.x()/255.0f, vec4iValue.z()/255.0f );
	break;
    case BACKGROUND_COLOR:
//  	cout << "State::applyState, BACKGROUND_COLOR : " << 
// 	   vec4iValue.x() << ", " << vec4iValue.y() << ", " << vec4iValue.z() << ", " << vec4iValue.w() << endl;
	glColor4f ( vec4iValue.x()/255.0f, vec4iValue.y()/255.0f, vec4iValue.z()/255.0f, vec4iValue.w()/255.0f );
	break;
    case LINE_TYPE:
    {
	unsigned short pattern = LOWORD(intValue);
	unsigned short factor = HIWORD(intValue);
 	//cout << "line type = " << pattern << ", linetype factor = " << factor << endl;
	glEnable ( GL_LINE_STIPPLE );
	glLineStipple ( factor, pattern );
	break;
    }
    case LINE_WIDTH:
	//cout << "line width = " << currentScale * floatValue << endl;
	glLineWidth ( currentScale * floatValue );
	break;
    case TEXTURE:
	if ( intValue >= 0 ) {
	    //cout << "glBindTexture texture = " << intValue << endl;
	    glBindTexture ( GL_TEXTURE_2D, intValue );
	}
	break;
    }
}

//================================================================================

StateSet::StateSet ()
{
    parent = NULL;
}

//================================================================================

StateSet::StateSet ( LC* c, MaterialRecord* mat, list<int> objs )
{
    parent = NULL;
    lc = c;
    renderObjects.assign ( objs.begin(), objs.end() );
    
    addState ( State(State::BACKGROUND_COLOR, mat->background_color, State::INHERIT) );
    addState ( State(State::FOREGROUND_COLOR, mat->foreground_color, State::INHERIT) );
    addState ( State(State::LINE_TYPE, (int)MAKELONG(mat->linetype,mat->linetypefactor), State::INHERIT) );
    addState ( State(State::LINE_WIDTH, mat->linewidth, State::INHERIT) );
    if ( mat->textureIdx<0 || mat->textureIdx>=lc->textures.size() ) {
	addState ( State(State::TEXTURE, -1, State::INHERIT) );
    } else {
	Texture* tex = lc->textures[mat->textureIdx];
	addState ( State(State::TEXTURE, (int)tex->texture, State::INHERIT) );
    }

}

//================================================================================

/**
 * if return NULL, stateset not matched
 * else return a stateset
 */
StateSet* StateSet::CreateOrReuseStateSet ( LC* lc, MaterialRecord* mat )
{
    if ( parent == NULL ) // root
    {
	for ( int i=0; i<children.size(); i++ )
	{
	    StateSet* t = children[i]->CreateOrReuseStateSet ( lc, mat );
	    if ( t != NULL )
		return t;
	} 
	// if this is leaf, or this is not leaf & all children not match
	StateSet* tss = new StateSet ();
	tss->addState ( State(State::BACKGROUND_COLOR, mat->background_color, State::INHERIT) );
	tss->addState ( State(State::FOREGROUND_COLOR, mat->foreground_color, State::INHERIT) );
	tss->addState ( State(State::LINE_TYPE, (int)MAKELONG(mat->linetype,mat->linetypefactor), State::INHERIT) );
	tss->addState ( State(State::LINE_WIDTH, mat->linewidth, State::INHERIT) );
	if ( mat->textureIdx<0 || mat->textureIdx>=lc->textures.size() ) {
	    tss->addState ( State(State::TEXTURE, -1, State::INHERIT) );
	} else {
	    Texture* tex = lc->textures[mat->textureIdx];
	    tss->addState ( State(State::TEXTURE, (int)tex->texture, State::INHERIT) );
	}
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
	 * matchCount < max(5)    create a new StateSet(nss) as this's descendant
	 * matchCount = max(5)    just return this
	 */

	// calculate match count
	if ( (state=getState ( State::BACKGROUND_COLOR )) != NULL ) {
	    if ( state->vec4iValue != mat->background_color ) 
		nss.addState ( State(State::BACKGROUND_COLOR, mat->background_color, State::OVERWRITE) );
	    else
		cnt ++;
	}
	if ( (state=getState ( State::FOREGROUND_COLOR )) != NULL ) {
	    if ( state->vec4iValue != mat->foreground_color )
		nss.addState ( State(State::FOREGROUND_COLOR, mat->foreground_color, State::OVERWRITE) );
	    else
		cnt ++;
	}
	if ( (state=getState ( State::LINE_TYPE )) != NULL ) {
	    if ( state->intValue != mat->linetype )
		nss.addState ( State(State::LINE_TYPE, (int)MAKELONG(mat->linetype,mat->linetypefactor), State::OVERWRITE) );
	    else
		cnt ++;
	}
	if ( (state=getState ( State::LINE_WIDTH )) != NULL ) {
	    if ( state->floatValue != mat->linewidth )
		nss.addState ( State(State::LINE_WIDTH, mat->linewidth, State::OVERWRITE) );
	    else
		cnt ++;
	}
	if ( (state=getState ( State::TEXTURE )) != NULL ) {
	    if ( mat->textureIdx<0 && mat->textureIdx>lc->textures.size() ) {
		cnt ++;
	    } else {
		Texture* tex = lc->textures[mat->textureIdx];
		if ( state->intValue != tex->texture )
		    nss.addState ( State(State::TEXTURE, (int)tex->texture, State::OVERWRITE) );
		else
		    cnt ++;
	    }
	}

	if ( cnt == 0 ) {
	    return NULL;
	} else if ( cnt == 5 ) {
	    return this;
	} else if ( cnt < 5 ) {
	    if ( cnt < parent->getMatchCount(mat) )
		return NULL;

	    // if this node not leaf
	    for ( int i=0; i<children.size(); i++ )
	    {
		StateSet* t = children[i]->CreateOrReuseStateSet ( lc, mat );
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
	if ( state->vec4iValue == mat->background_color )
	    cnt ++;
    if ( (state=getState ( State::FOREGROUND_COLOR)) != NULL )
	if ( state->vec4iValue == mat->foreground_color )
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

void StateSet::addChild ( StateSet* ss )
{
    children.push_back ( ss );
    ss->parent = this;
}

//================================================================================

string StateSet::toXML () const
{
    stringstream ss;

    ss << "<ss";
    for ( int i=0; i<states.size(); i++ ) {
	const State& state = states[i];
	switch ( state.type )
	{
	case State::FOREGROUND_COLOR:
	    ss << " FOREGROUND_COLOR=\"" << state.vec4iValue.x() << ' ' << state.vec4iValue.y() << ' ' << state.vec4iValue.z() << ' ' << state.vec4iValue.w() << "\"";
	    break;
	case State::BACKGROUND_COLOR:
	    ss << " BACKGROUND_COLOR=\"" << state.vec4iValue.x() << ' ' << state.vec4iValue.y() << ' ' << state.vec4iValue.z() << ' ' << state.vec4iValue.w() << "\"";
	    break;
	case State::LINE_TYPE:
	    unsigned short pattern, factor;
	    pattern = LOWORD(state.intValue);
	    factor = HIWORD(state.intValue);
	    ss << " LINE_TYPE_PATTERN=\"" << pattern << "\" LINE_TYPE_FACTOR=\"" << factor << "\"";
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

    return ss.str();
}

//================================================================================

void StateSet::pushAttributes ()
{
//     if ( parent == NULL )
// 	return;
    glPushAttrib ( GL_CURRENT_BIT | GL_LINE_BIT );
}

//================================================================================

void StateSet::popAttributes ()
{
//     if ( parent == NULL )
// 	return;
    glPopAttrib ();
}

//================================================================================

void StateSet::applyStates ()
{
//     if ( parent == NULL )
// 	return;
    for ( vector<State>::iterator pp=states.begin(); pp!=states.end(); ++pp )
	pp->applyState ();
}

//================================================================================

void StateSet::render ( LC* lc )
{
    vector<float> rects;
    vector<float> polys;
    vector<TextRecord*> texts;
    for ( vector<int>::iterator pp=renderObjects.begin(); pp!=renderObjects.end(); ++pp )
    {
	// get primitive
	GlobalLCRecord& g = lc->globalLCEntry->LCRecords[*pp];
	switch ( g.type )
	{ 
	case SLC_RECT:
	{
	    //cout << "draw rect" << endl;
	    RectRecord& r = lc->rectEntry->LCRecords[g.value];
	    float* begin = (float*)&(r.data[0]);
	    float* end   = begin + 12;
	    copy ( begin, end, back_inserter(rects) );
	    //cout << "draw rect" << endl;
	    break;
	}
	case SLC_TEXT:
	{
	    //cout << "draw text" << endl;
	    texts.push_back ( &lc->textEntry->LCRecords[g.value] );
	    //cout << "draw text" << endl;
	    break;
	}
	case SLC_PLINE:
	{
	    //cout << "draw pline" << endl;
//	    cout << "glDisable ( GL_TEXTURE_2D );" << endl;
	    glDisable ( GL_TEXTURE_2D );
	    PLineRecord& pline = lc->plineEntry->LCRecords[g.value];
	    glVertexPointer ( 3, GL_FLOAT, 0, (float*)(lc->plineBufferEntry->LCRecords + pline.start) );
	    glDrawArrays ( GL_LINE_STRIP, 0, pline.end - pline.start );
	    glEnable ( GL_TEXTURE_2D );
//	    cout << "glEnable ( GL_TEXTURE_2D );" << endl;
	    //cout << "draw pline" << endl;
	    break;
	}
	case SLC_POLY:
	{
	    //cout << "draw poly" << endl;
//	    cout << "glDisable ( GL_TEXTURE_2D );" << endl;
	    glDisable ( GL_TEXTURE_2D );
//	    glEnableClientState ( GL_TEXTURE_COORD_ARRAY );
	    PolyRecord& poly = lc->polyEntry->LCRecords[g.value];
	    glVertexPointer ( 3, GL_FLOAT, 0, (float*)(lc->polyTessellationBufferEntry->LCRecords + poly.tessellationstart) );
//	    glTexCoordPointer ( 2, GL_FLOAT, 0, (float*)(lc->texCoordBufferEntry->LCRecords + poly.texcoordstart) );
	    glDrawArrays ( GL_TRIANGLES, 0, poly.tessellationend - poly.tessellationstart );
//	    glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
	    glEnable ( GL_TEXTURE_2D );
//	    cout << "glEnable ( GL_TEXTURE_2D );" << endl;
	    //cout << "draw poly" << endl;
	    break;
	}
	}
    }

    // draw rects
    if ( false == rects.empty() ) {
//	cout << "glDisable ( GL_TEXTURE_2D );" << endl;
	glDisable ( GL_TEXTURE_2D );
	glVertexPointer ( 3, GL_FLOAT, 0, &(rects[0]) );
	glDrawArrays ( GL_QUADS, 0, rects.size()/3 );
//	glBegin( GL_QUADS );
// 	cout << "draw rects ";
// 	for ( int i=0; i<rects.size(); i+=3 ) {
// 	    glVertex3f ( rects[i], rects[i+1], rects[i+2] );
// 	    cout << "( " << rects[i] << ", " << rects[i+1] << ", " << rects[i+2] << ") ";
// 	}
// 	cout << endl;
//	glEnd();
// 	cout << "draw rect (" << rects[0] << ", " << rects[1] << ", " << rects[2] << "), " << 
// 	    ", (" << rects[3] << ", " << rects[4] << ", " << rects[5] << "), " << 
// 	    ", (" << rects[6] << ", " << rects[7] << ", " << rects[8] << "), " << 
// 	    ", (" << rects[9] << ", " << rects[10] << ", " << rects[11] << "), " << endl;
	glEnable ( GL_TEXTURE_2D );
//	cout << "glEnable ( GL_TEXTURE_2D );" << endl;
    }

    // draw text
    for ( vector<TextRecord*>::iterator pp=texts.begin(); pp!=texts.end(); ++pp )
    {
//	cout << "glEnable ( GL_TEXTURE_2D );" << endl;
	glEnable ( GL_TEXTURE_2D );
	TextRecord* tr = *pp;
	glPushMatrix ();
	glTranslatef ( tr->pos.x(), tr->pos.y(), tr->pos.z() );
	glScalef ( tr->scale, tr->scale, tr->scale );
	glRotatef ( tr->rotz, 0, 0, 1 );
	// get material
	MaterialRecord& mr = lc->materialEntry->LCRecords[ tr->materialIdx ];
	// get font
	Font* font = lc->fonts[ mr.fontIdx ];
	font->drawText ( lc->textBufferEntry->LCRecords + tr->start );

	glPopMatrix ();
	glDisable ( GL_TEXTURE_2D );
//	cout << "glDisable ( GL_TEXTURE_2D );" << endl;
    }
}

//================================================================================

void StateSet::render ()
{
    pushAttributes ();
    applyStates ();

    vector<float> rects;
    vector<float> polys;
    vector<TextRecord*> texts;
    for ( vector<int>::iterator pp=renderObjects.begin(); pp!=renderObjects.end(); ++pp )
    {
	// get primitive
	GlobalLCRecord& g = lc->globalLCEntry->LCRecords[*pp];
	switch ( g.type )
	{ 
	case SLC_RECT:
	{
	    RectRecord& r = lc->rectEntry->LCRecords[g.value];
	    float* begin = (float*)&(r.data[0]);
	    float* end   = begin + 12;
	    copy ( begin, end, back_inserter(rects) );
	    break;
	}
	case SLC_TEXT:
	{
	    texts.push_back ( &lc->textEntry->LCRecords[g.value] );
	    break;
	}
	case SLC_PLINE:
	{
	    glDisable ( GL_TEXTURE_2D );
	    PLineRecord& pline = lc->plineEntry->LCRecords[g.value];
	    glVertexPointer ( 3, GL_FLOAT, 0, (float*)(lc->plineBufferEntry->LCRecords + pline.start) );
	    glDrawArrays ( GL_LINE_STRIP, 0, pline.end - pline.start );
	    glEnable ( GL_TEXTURE_2D );
	    break;
	}
	case SLC_POLY:
	{
	    glDisable ( GL_TEXTURE_2D );
//	    glEnableClientState ( GL_TEXTURE_COORD_ARRAY );
	    PolyRecord& poly = lc->polyEntry->LCRecords[g.value];
	    glVertexPointer ( 3, GL_FLOAT, 0, (float*)(lc->polyTessellationBufferEntry->LCRecords + poly.tessellationstart) );
//	    glTexCoordPointer ( 2, GL_FLOAT, 0, (float*)(lc->texCoordBufferEntry->LCRecords + poly.texcoordstart) );
	    glDrawArrays ( GL_TRIANGLES, 0, poly.tessellationend - poly.tessellationstart );
//	    glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
	    glEnable ( GL_TEXTURE_2D );
	    break;
	}
	}
    }

    // draw rects
    if ( false == rects.empty() ) {
	cout << "draw rects " << rects.size()/3 << endl;
	glDisable ( GL_TEXTURE_2D );
	glVertexPointer ( 3, GL_FLOAT, 0, &(rects[0]) );
	glDrawArrays ( GL_QUADS, 0, rects.size()/3 );
	glEnable ( GL_TEXTURE_2D );
    }

    // draw text
    for ( vector<TextRecord*>::iterator pp=texts.begin(); pp!=texts.end(); ++pp )
    {
	glEnable ( GL_TEXTURE_2D );
	TextRecord* tr = *pp;
	glPushMatrix ();
	glTranslatef ( tr->pos.x(), tr->pos.y(), tr->pos.z() );
	glScalef ( tr->scale, tr->scale, tr->scale );
	glRotatef ( tr->rotz, 0, 0, 1 );
	// get material
	MaterialRecord& mr = lc->materialEntry->LCRecords[ tr->materialIdx ];
	// get font
	Font* font = lc->fonts[ mr.fontIdx ];
	font->drawText ( lc->textBufferEntry->LCRecords + tr->start );

	glPopMatrix ();
	glDisable ( GL_TEXTURE_2D );
    }
    popAttributes ();
}

//================================================================================
