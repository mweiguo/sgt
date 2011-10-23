#include "sgr_statesetbuilder.h"
#include "sgr_nodetypes.h"
//#include "sgr_lc.h"

StateSet* StateSetBuilder::root = NULL;
map<StateSet*,StateSet*> StateSetBuilder::_nodes;
map<MaterialRecord*,list<int> > StateSetBuilder2::onodes; // opaque nodes
map<MaterialRecord*,list<int> > StateSetBuilder2::tnodes; // transparent nodes


void StateSetBuilder::build ( LC* lc, list<int>& input )
{
    if ( input.empty() )
	return;

    if ( root == 0 ) {
	root = new StateSet();
	_nodes[root] = root;
    }

    for ( list<int>::iterator pp=input.begin(); pp!=input.end(); ++pp )
    {
	GlobalLCRecord& grcd = lc->globalLCEntry->LCRecords[*pp];
	switch ( grcd.type )
	{
	case SLC_RECT:
	{
	    RectRecord& drcd = lc->rectEntry->LCRecords[grcd.value];
	    MaterialRecord& mat = lc->materialEntry->LCRecords[drcd.materialIdx];
	    StateSet* nss = root->CreateOrReuseStateSet ( lc, &mat);
	    map<StateSet*,StateSet*>::iterator pp1 = _nodes.find ( nss );
	    if ( pp1 == _nodes.end() )
		_nodes.insert ( pair<StateSet*,StateSet*>(nss,nss) );
	    nss->renderObjects.push_back ( *pp );
	    break;
	}
	case SLC_TEXT:
	{
	    TextRecord& drcd = lc->textEntry->LCRecords[grcd.value];
	    MaterialRecord& mat = lc->materialEntry->LCRecords[drcd.materialIdx];
	    StateSet* nss = root->CreateOrReuseStateSet ( lc, &mat);
	    map<StateSet*,StateSet*>::iterator pp1 = _nodes.find ( nss );
	    if ( pp1 == _nodes.end() )
		_nodes.insert ( pair<StateSet*,StateSet*>(nss,nss) );
	    nss->renderObjects.push_back ( *pp );
	    break;
	}
	case SLC_PLINE:
	{
	    PLineRecord& drcd = lc->plineEntry->LCRecords[grcd.value];
	    MaterialRecord& mat = lc->materialEntry->LCRecords[drcd.materialIdx];
	    StateSet* nss = root->CreateOrReuseStateSet ( lc, &mat);
	    map<StateSet*,StateSet*>::iterator pp1 = _nodes.find ( nss );
	    if ( pp1 == _nodes.end() )
		_nodes.insert ( pair<StateSet*,StateSet*>(nss,nss) );
	    nss->renderObjects.push_back ( *pp );
	    break;
	}
	case SLC_POLY:
	{
	    PolyRecord& drcd = lc->polyEntry->LCRecords[grcd.value];
	    MaterialRecord& mat = lc->materialEntry->LCRecords[drcd.materialIdx];
	    StateSet* nss = root->CreateOrReuseStateSet ( lc, &mat);
	    map<StateSet*,StateSet*>::iterator pp1 = _nodes.find ( nss );
	    if ( pp1 == _nodes.end() )
		_nodes.insert ( pair<StateSet*,StateSet*>(nss,nss) );
	    nss->renderObjects.push_back ( *pp );
	    break;
	}
	}
    }
}

void StateSetBuilder::clear ()
{
    for ( map<StateSet*,StateSet*>::iterator pp=_nodes.begin();
	  pp!=_nodes.end(); ++pp )
	delete pp->second;
    _nodes.clear();
    root = NULL;
}

//================================================================================

void StateSetBuilder2::build ( LC* lc, list<int>& input, list<StateSet*>& opaques, list<StateSet*>& transparents )
{
    if ( input.empty() )
	return;

    for ( list<int>::iterator pp=input.begin(); pp!=input.end(); ++pp )
    {
	GlobalLCRecord& grcd = lc->globalLCEntry->LCRecords[*pp];
	MaterialRecord* mat = 0;
	switch ( grcd.type )
	{
	case SLC_RECT:
	    mat = &(lc->materialEntry->LCRecords[lc->rectEntry->LCRecords[grcd.value].materialIdx]);
	    break;
	case SLC_TEXT:
	    mat = &(lc->materialEntry->LCRecords[lc->textEntry->LCRecords[grcd.value].materialIdx]);
	    break;
	case SLC_PLINE:
	    mat = &(lc->materialEntry->LCRecords[lc->plineEntry->LCRecords[grcd.value].materialIdx]);
	    break;
	case SLC_POLY:
	    mat = &(lc->materialEntry->LCRecords[lc->polyEntry->LCRecords[grcd.value].materialIdx]);
	    break;
	}

	if ( mat == 0 ) 
	    continue;

	std::map<MaterialRecord*,std::list<int> >* nodes;
//	if ( mat->background_color.w() != 1 || mat->foreground_color.w() != 1 )
	if ( mat->background_color.w() == 255 )// || mat->foreground_color.w() != 1 )
	    nodes = &onodes;
	else
	    nodes = &tnodes;

	map<MaterialRecord*,list<int> >::iterator pp1 = nodes->find ( mat );
	if ( pp1 == nodes->end() ) {
	    nodes->insert ( pair<MaterialRecord*, list<int> >(mat,list<int> (1, *pp)) );
	} else {
	    pp1->second.push_back ( *pp );
	}
    }

    for ( map<MaterialRecord*,list<int> >::iterator pp=onodes.begin(); pp!=onodes.end(); ++pp )
	opaques.push_back ( new StateSet ( lc, pp->first, pp->second ) );
    for ( map<MaterialRecord*,list<int> >::iterator pp=tnodes.begin(); pp!=tnodes.end(); ++pp )
	transparents.push_back ( new StateSet ( lc, pp->first, pp->second ) );
}

void StateSetBuilder2::clear ()
{
    onodes.clear();
    tnodes.clear();
}
