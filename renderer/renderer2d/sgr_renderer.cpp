#include "sgr_renderer.h"
#include "sgr_stateset.h"
#include <vector>
#include <iostream>
using namespace std;

//--------------------------------------------------------------------------------

void Renderer::render ( StateSet* node, LC* lc )
{
    node->pushAttributes ();
    node->applyStates ();
    node->render ( lc );
    for ( vector<StateSet*>::iterator pp=node->children.begin();
	  pp!=node->children.end(); ++pp )
	render ( *pp, lc );
    node->popAttributes ();
}

//--------------------------------------------------------------------------------
