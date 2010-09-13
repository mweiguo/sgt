#ifndef _LOADMESH_H_
#define _LOADMESH_H_

//#include <xml_xerceshelper.h>
//#include <algorithm>
//#include <time.h>
//#include "sgr_arraynode.h"
//#include "sgr_transformnode.h"
//#include "sgr_rectitem.h"
//#include "sgr_scenemgr.h"
//#include "sgr_arrayexpander.h"
//#include "sgr_parentfinder.h"
//#include "sgr_nodeleter.h"
//#include "sgr_nodedumper.h"
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

#include "sgr_kdtreenode.h"

/* #include "sgr_kdtree.h" */
#include <vector>
#include <map>
using namespace std;

namespace xercesc_2_7
{
    class DOMElement;
}
namespace SGR
{

    class SGNode;
    class SceneNode;
    class ArrayNode;


    class SGR_DLL LoadScene 
    {
    public:
        LoadScene ( const char* fileName, bool needExpand=false, bool needSceneManagement=false, bool useBaseId=false );
        ~LoadScene ();
        SGNode* root () { return _root; }
    private:
        void traverseNode ( xercesc::DOMElement* pnode, SGNode* data );
        void setDrawableNodeAttributes ( xercesc::DOMElement* tagNode, DrawableNode* node );
        void setSGNodeAttributes ( xercesc::DOMElement* tagNode, SGNode* node );
        void getShapeGenParas (int index, int& s1, int& s2, int& s3, int& s4, int& s5, int& s6, int level0Cnt, int level1Cnt, int level2Cnt, int level3Cnt, int level4Cnt, int level5Cnt );
    private:
        SGNode* _root;
        vector<ArrayNode*> _arraynodes;  // only save top arraynode
        //    vector<ArrayNode*> _allarraynodes;  // only save top arraynode
        vector<KdTreeNode*> _kdtreenodes;  // only save top kdtreenode
        map<string,SGNode*> _defines;
        int _maxid;
        int _baseId;
    };

    class UnloadNode
    {
    public:
        UnloadNode ( SGNode* sgnode );
    };
}

#endif

