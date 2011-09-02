#include "sgr_statesetbuilder.h"
#include "sgr_nodetypes.h"
//#include "sgr_lc.h"

StateSet* StateSetBuilder::root = NULL;

void StateSetBuilder::build ( LC* lc, list<int>& input )
{
    if ( input.empty() )
	return;

    if ( root == 0 ) 
	root = new StateSet();

    for ( list<int>::iterator pp=input.begin(); pp!=input.end(); ++pp )
    {
	GlobalLCRecord& grcd = lc->globalLCEntry->LCRecords[*pp];
	if ( SLC_RECT == grcd.type  )
	{
	    RectRecord& rrcd = lc->rectEntry->LCRecords[grcd.value];
	    MaterialRecord& mat = lc->materialEntry->LCRecords[rrcd.materialIdx];
	    StateSet* nss = root->CreateOrReuseStateSet ( &mat);
	    nss->renderObjects.push_back ( *pp );
	}
    }
}

void StateSetBuilder::freeStateSets ( StateSet* root )
{
    
}
