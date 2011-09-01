#include "sgr_lc2kdtree.h"
#include "sgr_nodetypes.h"

KdTree<int>* LC2KDT::curKDT = 0;
map<string, KdTree<int> > LC2KDT::kdtreemap;

void LC2KDT::buildKDT ( LC& lc, KdTree<int>& kdtree )
{
    // traverse lc to collect primitives
    collectPrimitive ( lc.getType(), lc.getGIndex(), kdtree );
    traverse ( lc, kdtree );

    // build kdtree
    GetPrimitiveCenter getPrimitiveCenter;
    GetPrimitiveMinMax getPrimitiveMinMax;
    getPrimitiveCenter.init ( &lc );
    getPrimitiveMinMax.init ( &lc );
    typedef KdTree<int>::BuildKdTree<GetPrimitiveCenter, GetPrimitiveMinMax> BuildLCKdTree;
    typedef BuildKdTreeOption<GetPrimitiveCenter, GetPrimitiveMinMax> BuildLCKdTreeOption;
    BuildLCKdTreeOption option;
    option.getPrimitiveCenter = getPrimitiveCenter;
    option.getPrimitiveMinMax = getPrimitiveMinMax;
    BuildLCKdTree buildkdt ( kdtree, option );
    buildkdt.build ();
}

void LC2KDT::traverse ( LC& lc, KdTree<int>& kdtree )
{
    if ( lc.toElement ( FIRST_CHILD ) >= 0 ) {
	collectPrimitive ( lc.getType(), lc.getGIndex(), kdtree );
	traverse ( lc, kdtree );
        while ( lc.toElement ( NEXT_SIBLING )>=0 ) {
	    collectPrimitive ( lc.getType(), lc.getGIndex(), kdtree );
            traverse ( lc, kdtree );
        }
        lc.toElement ( PARENT );
    }
}

void LC2KDT::collectPrimitive ( int type, int gidx, KdTree<int>& kdtree )
{
    switch ( type )
    {
    case SLC_SCENE:
    case SLC_LAYER:
    case SLC_LOD:
    case SLC_LODPAGE:
        break;
    case SLC_PLINE:
    case SLC_POLY:
    case SLC_LINE:
    case SLC_TRIANGLE:
    case SLC_RECT:
	kdtree._primitives.push_back ( gidx );
        break;
    default:
//        ss << "unsupport type " << type;
        break;
    }
}

void LC2KDT::buildKDTs ( LC& lc )
{
    curKDT = NULL;
    kdtreemap.clear();

    collectPrimitive2 ( lc );
    traverse2 ( lc );

    for ( map<string, KdTree<int> >::iterator pp=kdtreemap.begin();
	  pp!=kdtreemap.end();
	  ++pp )
    {
	// build kdtree
	GetPrimitiveCenter getPrimitiveCenter;
	GetPrimitiveMinMax getPrimitiveMinMax;
	getPrimitiveCenter.init ( &lc );
	getPrimitiveMinMax.init ( &lc );
	typedef KdTree<int>::BuildKdTree<GetPrimitiveCenter, GetPrimitiveMinMax> BuildLCKdTree;
	typedef BuildKdTreeOption<GetPrimitiveCenter, GetPrimitiveMinMax> BuildLCKdTreeOption;
	BuildLCKdTreeOption option;
	option.targetnumperleaf = 1;
	option.maxlevel = 32;
	option.getPrimitiveCenter = getPrimitiveCenter;
	option.getPrimitiveMinMax = getPrimitiveMinMax;
	BuildLCKdTree buildkdt ( pp->second, option );
	buildkdt.build ();

	pp->second.save ( pp->first.c_str() );
    }
}

void LC2KDT::traverse2 ( LC& lc )
{
    if ( lc.toElement ( FIRST_CHILD ) >= 0 ) {
	bool isSetCurKDT = collectPrimitive2 ( lc );
	traverse2 ( lc );
	if ( isSetCurKDT )
	    curKDT = NULL;
        while ( lc.toElement ( NEXT_SIBLING )>=0 ) {
	    isSetCurKDT = collectPrimitive2 ( lc );
            traverse2 ( lc );
	    if ( isSetCurKDT )
		curKDT = NULL;
        }
        lc.toElement ( PARENT );
    }
}

/**
 * if new kdtree created, return true
 */
bool LC2KDT::collectPrimitive2 ( LC& lc  )
{
    int type = lc.getType();
    int gidx = lc.getGIndex();
    int idx  = lc.getValue();
    switch ( type )
    {
    case SLC_LODPAGE:
    {
        string path = lc.lodpageEntry->LCRecords[idx].kdtreepath;
	if ( path != "" )
	{
	    kdtreemap[path] = KdTree<int>();
	    curKDT = &(kdtreemap[path]);
	    return true;
	}
	// check lodpage's kdtree member, if exist, build a new kdtree
        break;
    }
    case SLC_PLINE:
    case SLC_POLY:
    case SLC_LINE:
    case SLC_TRIANGLE:
    case SLC_RECT:
	if ( curKDT )
	    curKDT->_primitives.push_back ( gidx );
        break;
    default:
        break;
    }
    return false;
}
