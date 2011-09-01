#include "sgr_bboxupdater.h"

BBox2d BBox2dUpdater::traverse ( LC& lc )
{
    if ( lc.toElement ( FIRST_CHILD ) >= 0 )
    {
	BBox2d bbox2d;
	bbox2d += traverse ( lc );

	while ( lc.toElement ( NEXT_SIBLING )>=0 )
	    bbox2d += traverse ( lc );

	lc.toElement ( PARENT );

	BBox2d* tmp = (BBox2d*)(lc.getMinMax());
	bbox2d += *tmp;
	lc.setMinMax ( bbox2d.minvec().x(), bbox2d.minvec().y(), bbox2d.maxvec().x(), bbox2d.maxvec().y() );
	return bbox2d;
    }
    else
    {
	lc.updateMinMax ();
	BBox2d* tmp = (BBox2d*)(lc.getMinMax());
	return *tmp;
    }
}

void BBox2dUpdater::forward_update ( LC& lc )
{
    lc.toElement ( ROOT );
    traverse ( lc );    
}

void BBox2dUpdater::lazy_update ( LC& lc )
{
    lc.updateMinMax ();

    BBox2d bbox2d;
    do {
	lc.setMinMax ( bbox2d.minvec().x(), bbox2d.minvec().y(), bbox2d.maxvec().x(), bbox2d.maxvec().y() );
	do {
	    BBox2d* tmp = (BBox2d*)(lc.getMinMax());
	    bbox2d += *tmp;
	} while ( lc.toElement ( NEXT_SIBLING )>=0 );
    } while ( lc.toElement(PARENT) >= 0 );
}
