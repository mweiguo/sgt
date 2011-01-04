#ifndef _LOADMESH_H_
#define _LOADMESH_H_

#ifdef _USE_XERCES3_
    #include <xercesc/parsers/XercesDOMParser.hpp>
    #include <xercesc/dom/DOM.hpp>
    #include <xercesc/sax/HandlerBase.hpp>
    #include <xercesc/util/XMLString.hpp>
    #include <xercesc/util/PlatformUtils.hpp>
    using namespace xercesc;
#else // _USE_XERCES3_
    #include <xml_xerceshelper.h>
#endif // _USE_XERCES3_

#include "sgr_kdtreenode.h"
#include <vector>
#include <map>
using namespace std;


namespace SGR
{

    class SGNode;
    class SceneNode;
    class ArrayNode;
    class NodeParser;

    class SGR_DLL LoadScene : public list<SGNode*>
    {
    public:
        LoadScene ( const char* fileName, bool needExpand=false, bool needSceneManagement=false, bool useBaseId=false );
        ~LoadScene ();
        //SGNode* root () { return _root; }
    private:
        void traverseNode ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* data );
    protected:
        //SGNode* _root;
        vector<ArrayNode*> _arraynodes;  // only save top arraynode
        //    vector<ArrayNode*> _allarraynodes;  // only save top arraynode
        vector<KdTreeNode*> _kdtreenodes;  // only save top kdtreenode
        map<string,SGNode*> _defines;
        int _maxid;
        int _baseId;
        map<string, NodeParser*> _nodeParsers;

        friend class NodeParser;
        friend class SceneNodeParser;
        friend class FontNodeParser;
        friend class LayerNodeParser;
        friend class PickablegroupNodeParser;
        friend class LodNodeParser;
        friend class KDTreeNodeParser;
        friend class TransformNodeParser;
        friend class ArrayNodeParser;
        friend class RectNodeParser;
        friend class TextNodeParser;
        friend class GroupNodeParser;
        friend class SwitchNodeParser;
        friend class LineNodeParser;
        friend class MeshNodeParser;
        friend class MeshlineNodeParser;
        friend class PolylineNodeParser;
        friend class PolyNodeParser;
        friend class PointNodeParser;
        friend class MeshpointNodeParser;
        friend class CircleNodeParser;
        friend class ImageNodeParser;
        friend class ImposterNodeParser;
    };

    class UnloadNode
    {
    public:
        UnloadNode ( SGNode* sgnode );
    };

    class NodeParser
    {
    public:
        NodeParser ( LoadScene* scene ) : _scene(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent ) = 0;
    protected:
        void setDrawableNodeAttributes ( XERCES_CPP_NAMESPACE::DOMElement* tagNode, DrawableNode* node );
        void setSGNodeAttributes ( XERCES_CPP_NAMESPACE::DOMElement* tagNode, SGNode* node );
        int checkIDAttr ( XERCES_CPP_NAMESPACE::DOMElement* pnode );
    protected:
        LoadScene* _scene;
    };
    
    class SceneNodeParser : public NodeParser
    {
    public:
        SceneNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class FontNodeParser : public NodeParser
    {
    public:
        FontNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class LayerNodeParser : public NodeParser
    {
    public:
        LayerNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class PickablegroupNodeParser : public NodeParser
    {
    public:
        PickablegroupNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class LodNodeParser : public NodeParser
    {
    public:
        LodNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class KDTreeNodeParser : public NodeParser
    {
    public:
        KDTreeNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class TransformNodeParser : public NodeParser
    {
    public:
        TransformNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class ArrayNodeParser : public NodeParser
    {
    public:
        ArrayNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    protected:
        void getShapeGenParas (int index, int& s1, int& s2, int& s3, int& s4, int& s5, int& s6, int level0Cnt, int level1Cnt, int level2Cnt, int level3Cnt, int level4Cnt, int level5Cnt );
    };

    class RectNodeParser : public NodeParser
    {
    public:
        RectNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class TextNodeParser : public NodeParser
    {
    public:
        TextNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class GroupNodeParser : public NodeParser
    {
    public:
        GroupNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class SwitchNodeParser : public NodeParser
    {
    public:
        SwitchNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class LineNodeParser : public NodeParser
    {
    public:
        LineNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class MeshNodeParser : public NodeParser
    {
    public:
        MeshNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class MeshlineNodeParser : public NodeParser
    {
    public:
        MeshlineNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class PolylineNodeParser : public NodeParser
    {
    public:
        PolylineNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class PolyNodeParser : public NodeParser
    {
    public:
        PolyNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class PointNodeParser : public NodeParser
    {
    public:
        PointNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class MeshpointNodeParser : public NodeParser
    {
    public:
        MeshpointNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class CircleNodeParser : public NodeParser
    {
    public:
        CircleNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class ImageNodeParser : public NodeParser
    {
    public:
        ImageNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

    class ImposterNodeParser : public NodeParser
    {
    public:
        ImposterNodeParser ( LoadScene* scene ) : NodeParser(scene) {}
        virtual void parse ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent );
    };

}

#endif


