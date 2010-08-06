#ifndef _LOADMESH_H_
#define _LOADMESH_H_

//#include <xml_xerceshelper.h>
//#include <algorithm>
//#include <time.h>
//#include "arraynode.h"
//#include "transformnode.h"
//#include "rectitem.h"
//#include "scenemgr.h"
//#include "arrayexpander.h"
//#include "parentfinder.h"
//#include "nodeleter.h"
//#include "nodedumper.h"
#include "kdtreenode.h"

/* #include "kdtree.h" */
#include <vector>
#include <map>
using namespace std;
class SGNode;
class SceneNode;
class ArrayNode;

namespace xercesc_2_7
{
    class DOMElement;
}

class AGEF_EXPORT LoadScene 
{
public:
    LoadScene ( const char* fileName, bool needExpand=false, bool needSceneManagement=false );
    SGNode* root () { return _root; }
private:
    void traverseNode ( xercesc_2_7::DOMElement* pnode, SGNode* data );
    void getShapeGenParas (int index, int& s1, int& s2, int& s3, int& s4, int& s5, int& s6, int level0Cnt, int level1Cnt, int level2Cnt, int level3Cnt, int level4Cnt, int level5Cnt );
private:
    SGNode* _root;
    vector<ArrayNode*> _arraynodes;  // only save top arraynode
    vector<KdTreeNode*> _kdtreenodes;  // only save top kdtreenode
    map<string,SGNode*> _defines;
};

class UnloadNode
{
public:
    UnloadNode ( SGNode* sgnode );
};

#endif

