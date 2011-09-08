#include "sgr_renderer.h"
#include "sgr_stateset.h"
#include <vector>
#include <iostream>
using namespace std;

//--------------------------------------------------------------------------------

void Renderer::render ( StateSet* node, LC* lc )
{
//    cout << "Renderer::render" << endl;
    // push all attributes in stack
    node->pushAttributes ();
    // apply stateset's state
    node->applyStates ();
    node->render ( lc );
    // doTraverse for each children
    for ( vector<StateSet*>::iterator pp=node->children.begin();
	  pp!=node->children.end(); ++pp )
	render ( *pp, lc );
    // pop attributes from stack
    node->popAttributes ();
}

//--------------------------------------------------------------------------------
