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
class MeshNode;
class ArrayNode;


#ifdef _USE_XERCES3_
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
using namespace xercesc;
#else // _USE_XERCES3_
namespace xercesc_2_7
{
    class DOMElement;
}
#endif // _USE_XERCES3_
class AGEF_EXPORT LoadMesh 
{
public:
    LoadMesh ( const char* fileName, bool needExpand=false, bool needSceneManagement=false );
    MeshNode* root () { return _root; }
private:
#ifndef _USE_XERCES3_
    void traverseNode ( xercesc_2_7::DOMElement* pnode, SGNode* data );
#else // _USE_XERCES3_
    void traverseNode ( DOMElement* pnode, SGNode* data );
#endif // _USE_XERCES3_
    void getShapeGenParas (int index, int& s1, int& s2, int& s3, int& s4, int& s5, int& s6, int level0Cnt, int level1Cnt, int level2Cnt, int level3Cnt, int level4Cnt, int level5Cnt );
private:
    MeshNode* _root;
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

