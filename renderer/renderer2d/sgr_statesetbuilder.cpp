#include "sgr_statesetbuilder.h"
#include "sgr_nodetypes.h"
//#include "sgr_lc.h"

StateSet* StateSetBuilder::root = NULL;
map<StateSet*,StateSet*> StateSetBuilder::_nodes;

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
	case SLC_TEXT:
	case SLC_PLINE:
	    DrawableRecord& drcd = lc->rectEntry->LCRecords[grcd.value];
	    MaterialRecord& mat = lc->materialEntry->LCRecords[drcd.materialIdx];
	    StateSet* nss = root->CreateOrReuseStateSet ( &mat);
	    map<StateSet*,StateSet*>::iterator pp1 = _nodes.find ( nss );
	    if ( pp1 == _nodes.end() )
		_nodes.insert ( pair<StateSet*,StateSet*>(nss,nss) );
	    nss->renderObjects.push_back ( *pp );
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
