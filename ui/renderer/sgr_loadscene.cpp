

#include "sgr_loadscene.h"

//#ifndef _USE_XERCES3_
//#include <xml_xerceshelper.h>
//#endif// _USE_XERCES3_

#include <algorithm>
#include <sstream>
#include <time.h>

#include "sgr_scenemgr.h"
#include "sgr_arrayexpander.h"
#include "sgr_parentfinder.h"
#include "sgr_nodeleter.h"
#include "sgr_nodedumper.h"
#include "sgr_linenode.h"
#include <QDebug>
#include "sgr_attrset.h"
#include "sgr_scenenode.h"
#include "sgr_nodes.h"
#include "sgr_interface.h"
#include "sgr_seedgen.h"

namespace SGR
{


#ifdef _USE_XERCES3_
    class ChWrapper
    {
    public:
        ChWrapper ( const XMLCh* data )
        {
            _data = XMLString::transcode ( data );
        }
        operator char*() const
        {
            return _data;
        }
        ~ChWrapper()
        {
            XMLString::release ( &_data );
        }
    private:
        char* _data;
    };

    class XMLChWrapper
    {
    public:
        XMLChWrapper ( const char* data )
        {
            _data = XMLString::transcode ( data );
        }
        operator XMLCh*() const
        {
            return _data;
        }
        ~XMLChWrapper ()
        {
            XMLString::release ( &_data );
        }
    private:
        XMLCh* _data;       
    };

    class XercesHelper
    {
    public:
        static bool hasAttribute ( DOMElement* p, const char* attr )
        {
            XMLChWrapper xercesCh (attr);
            return p->hasAttribute ( (XMLCh*)xercesCh );
        }
        static string getAttribute ( DOMElement* p, const char* attr )
        {
            XMLChWrapper xercesCh (attr);
            ChWrapper nativeCh ( p->getAttribute ( (XMLCh*)xercesCh ) );
            return string((char*)nativeCh);
        }
    };
#endif //_USE_XERCES3_

    UnloadNode::UnloadNode ( SGNode* sgnode )
    {
        sgnode->setParentNode ( NULL );

        Nodeleter deleter;
        deleter ( sgnode ); 

        LayerMgr::getInst().clear();
    }

    LoadScene::LoadScene ( const char* fileName, bool needExpand, bool needSceneManagement, bool useBaseId ) : _baseId(0)
    {
        // init parsers
        _nodeParsers["scene"] = new SceneNodeParser ( this );
        _nodeParsers["font"] = new FontNodeParser ( this );
        _nodeParsers["layer"] = new LayerNodeParser ( this );
        _nodeParsers["pickablegroup"] = new PickablegroupNodeParser ( this );
        _nodeParsers["lod"] = new LodNodeParser ( this );
        _nodeParsers["kdtree"] = new KDTreeNodeParser ( this );
        _nodeParsers["transform"] = new TransformNodeParser ( this );
        _nodeParsers["array"] = new ArrayNodeParser ( this );
        _nodeParsers["rect"] = new RectNodeParser ( this );
        _nodeParsers["text"] = new TextNodeParser ( this );
        _nodeParsers["group"] = new GroupNodeParser ( this );
        _nodeParsers["switch"] = new SwitchNodeParser ( this );
        _nodeParsers["line"] = new LineNodeParser ( this );
        _nodeParsers["mesh"] = new MeshNodeParser ( this );
        _nodeParsers["meshline"] = new MeshlineNodeParser ( this );
        _nodeParsers["polyline"] = new PolylineNodeParser ( this );
        _nodeParsers["poly"] = new PolyNodeParser ( this );
        _nodeParsers["point"] = new PointNodeParser ( this );
        _nodeParsers["meshpoint"] = new MeshpointNodeParser ( this );
        _nodeParsers["circle"] = new CircleNodeParser ( this );
        _nodeParsers["image"] = new ImageNodeParser ( this );
        _nodeParsers["imposter"] = new ImposterNodeParser ( this );

        _maxid = 0;
        if ( useBaseId )
            _baseId = SeedGenerator::getInst().maxseed ();

        int clo = clock();
#ifndef _USE_XERCES3_
        XercesParser parser;
        XERCES_CPP_NAMESPACE::DOMDocument* doc = parser.parseFile ( fileName, false);
#else   //_USE_XERCES3_
        XercesDOMParser parser;
        parser.parse ( fileName );
        DOMDocument* doc = parser.getDocument();
#endif  //_USE_XERCES3_
        if(doc == NULL)     
	{
	    stringstream ss;
	    ss << "Fail to load Shape Template: " << fileName;
            throw logic_error( ss.str().c_str()  );
	}

        XERCES_PRODUCT::DOMElement* root = doc->getDocumentElement();
        if (root == NULL) 
            throw logic_error("invalid Shape Template file:");
        //char* tagName = (char*)XercesHelper::getTagName (root);
        qDebug ( "parseFile TAKE %d clock, %f (s)", clock() - clo,  (1.0*(clock() - clo))/CLOCKS_PER_SEC );

        clo = clock();
        //_root = new SGNode();
        traverseNode ( root, 0 );

        qDebug ( "traverseNode TAKE %d clock, %f (s)", clock() - clo,  (1.0*(clock() - clo))/CLOCKS_PER_SEC );

        // expand array node
        if ( needExpand )
        {
            clo = clock();
            int ii = 0;
            for ( vector<ArrayNode*>::iterator pp=_arraynodes.begin(); pp!=_arraynodes.end(); ++pp )
            {
                ArrayNode* node = *pp;
                SGNode* parent = node->getParentNode();
                node->setParentNode ( NULL );
                ArrayExpander expander ( parent );
                expander ( *(*pp) );
                copy ( expander.kdbegin(), expander.kdend(), back_inserter(_kdtreenodes) );
            }
            qDebug ( "%d array expander TAKE %d clock, %f (s)", ii, clock() - clo,  (1.0*(clock() - clo))/CLOCKS_PER_SEC );
        }

        if ( _maxid > SeedGenerator::getInst().maxseed () )
        {
            //if ( 0 == _baseId )
            //    SeedGenerator::getInst().maxseed ( _maxid+1 );
            //else
                SeedGenerator::getInst().maxseed ( _maxid + 1 );
        }

        for ( list<SGNode*>::iterator pp=begin(); pp!=end(); ++pp )
        {
//            (*pp)->updateBBox();
            (*pp)->computeBBox ();
//            node_save ("dump.xml", (*pp)->getID() );
        }

        if ( needSceneManagement )
        {
            clo = clock();
            // if exists kdtree, build it
            for ( vector<KdTreeNode*>::iterator pp=_kdtreenodes.begin(); pp!=_kdtreenodes.end(); ++pp )
            {
                (*pp)->buildKdTree ();
            }
            qDebug ( "build kdtree TAKE %d clock, %f (s)", clock() - clo,  (1.0*(clock() - clo))/CLOCKS_PER_SEC );
        }

        //_root->updateBBox();
        //NodeDumper dumper;
        //dumper ( _root );
        //qDebug ( "%s", dumper.dumpstring().c_str() );
    }

    LoadScene::~LoadScene ()
    {
        vector<ArrayNode*>::iterator pp = _arraynodes.begin(), pp1;
        while ( pp!=_arraynodes.end() )
        {
            pp1 = pp++;
            Nodeleter del;
            del ( *pp1 );
        }
    }

    map<string, NodeParser*> _nodeParsers;

    void LoadScene::traverseNode ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent )
    {
        DOMNodeList * children = pnode->getChildNodes ();
        for ( XMLSize_t i=0; i<children->getLength (); i++ )
        {
            DOMElement* tagElement = dynamic_cast < DOMElement* > ( children->item ( i ) );
            if ( NULL == tagElement )
                continue;

            string name = (const char*)XercesHelper::getTagName ( tagElement );
            map<string, NodeParser*>::iterator pp = _nodeParsers.find ( name );
            if ( pp != _nodeParsers.end() )
            {
                pp->second->parse ( tagElement, parent );
            }

            //traverseNode ( tagElement, 0 );
        }
    }

    void NodeParser::setSGNodeAttributes ( XERCES_CPP_NAMESPACE::DOMElement* tagNode, SGNode* node )
    {
        if ( XercesHelper::hasAttribute ( tagNode, "isVisible" ) ) {
            string tmp = (char*)XercesHelper::getAttribute ( tagNode, "isVisible" );
            if ( tmp == "1" ) node->setVisible ( true );
            else if ( tmp == "0" ) node->setVisible ( false );
        }
/*
        if ( XercesHelper::hasAttribute ( tagNode, "bbox" ) ) {
            string tmp = (char*)XercesHelper::getAttribute ( tagNode, "bbox" );
            istringstream iss(tmp);
            vector<string> tokens;
            copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
            if ( tokens.size() == 6 )
            {
                vec3f minv, maxv;
                minv.x ( atof ( tokens[0].c_str() ) );
                minv.y ( atof ( tokens[1].c_str() ) );
                minv.z ( atof ( tokens[2].c_str() ) );
                maxv.x ( atof ( tokens[3].c_str() ) );
                maxv.y ( atof ( tokens[4].c_str() ) );
                maxv.z ( atof ( tokens[5].c_str() ) );
                node->setBBox ( BBox ( minv, maxv ) );
//                node->_isBBoxDirty = true;
            }
        }
*/
    }

    void NodeParser::setDrawableNodeAttributes ( XERCES_CPP_NAMESPACE::DOMElement* tagNode, DrawableNode* node )
    {
        static string bygroupstr = "bygroup";
        // get color, font from xml
        GColor *fgcolor = NULL, *bgcolor = NULL;
        FontNode* fontnode = NULL;
        if ( XercesHelper::hasAttribute ( tagNode, "bgcolor" ) )
        {
            string fillcolor = (const char*)(XercesHelper::getAttribute( tagNode, "bgcolor" ));
            if ( bygroupstr != fillcolor )
                bgcolor = new GColor(fillcolor);
        }

        if ( XercesHelper::hasAttribute ( tagNode, "fgcolor" ) )
        {
            string bordercolor = (const char*)(XercesHelper::getAttribute( tagNode, "fgcolor" ));
            if ( bygroupstr != bordercolor )
                fgcolor = new GColor(bordercolor);
        }

        if ( XercesHelper::hasAttribute ( tagNode, "font" ) )
        {
            string def = (const char*)(XercesHelper::getAttribute ( tagNode, "font" ));

            map<string,SGNode*>::iterator pp = _scene->_defines.find (def);
            if ( pp != _scene->_defines.end() )
                fontnode = static_cast<FontNode*>(pp->second);
        }

        // set drawablenode's attributes
        ParentFinder<LayerNode> finder( node );
        if ( NULL != finder.target() )
        {
            if ( NULL==bgcolor && NULL==fgcolor && NULL==fontnode)
            {
                node->setAttrSet ( finder.target()->getAttrSet() );
            }
            else
            {
                AttrSet* set = NodeMgr::getInst().addNode<AttrSet> ( SeedGenerator::getInst().minseed() );
                //AttrSet* set = new At trSet ( /*finder.target()->getRenderOrder()*/ );
                node->setAttrSet ( set );
                if ( NULL != bgcolor )
                    set->setBgColor ( bgcolor );
                else
                    set->setBgColor ( finder.target()->getBgColorPtr() );

                if ( NULL != fgcolor )
                    set->setFgColor ( fgcolor );
                else
                    set->setFgColor ( finder.target()->getFgColorPtr() );

                if ( NULL != fontnode )
                    set->setFont ( fontnode );
                else
                    set->setFont ( finder.target()->getFontNode() );
            }
        }
        else
        {
            AttrSet* set = NodeMgr::getInst().addNode<AttrSet> ( SeedGenerator::getInst().minseed() );
            //AttrSet* set = new AttrSet ( /*-1*/ );
            node->setAttrSet ( set );
            if ( NULL != bgcolor )
                set->setBgColor ( bgcolor );

            if ( NULL != fgcolor )
                set->setFgColor ( fgcolor );

            if ( NULL != fontnode )
                set->setFont ( fontnode );
        }

    }

    void ArrayNodeParser::getShapeGenParas (int index, int& s1, int& s2, int& s3, int& s4, int& s5, int& s6, int /*level0Cnt*/, int level1Cnt, int level2Cnt, int level3Cnt, int level4Cnt, int level5Cnt )
    {
        // init
        s1 = s2 = s3 = s4 = s5 = 0;

        if ( 0 == level1Cnt ) {
            //s1 = index;
            return;
        }
        else
            s1 = index - index / level1Cnt;

        if ( 0 == level2Cnt ) {
            //s2 = index / level1Cnt;
            return;
        }
        else
            s2 = index / level1Cnt - index / level2Cnt;

        if ( 0 == level3Cnt ) {
            //s3 = index / level2Cnt;
            return;
        }
        else
            s3 = index / level2Cnt - index / level3Cnt;

        if ( 0 == level4Cnt ) {
            //s4 = index / level3Cnt;
            return;
        }
        else
            s4 = index / level3Cnt - index / level4Cnt;

        if ( 0 == level5Cnt ) {
            //s5 = index / level4Cnt;
            return;
        }
        else
            s5 = index / level4Cnt - index / level5Cnt;

        s6 = index / level5Cnt;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    int NodeParser::checkIDAttr ( XERCES_CPP_NAMESPACE::DOMElement* tagElement )
    {
        if ( !XercesHelper::hasAttribute ( tagElement, "id" ) )
	{
	    stringstream ss;
	    ss << (const char*)XercesHelper::getTagName ( tagElement ) << "'s attribute id is required, every node should define a unique id";
            throw std::invalid_argument ( ss.str().c_str() );
	}
        int id = atoi((const char*)XercesHelper::getAttribute ( tagElement, "id" ));

        if ( _scene->_baseId )
            id += _scene->_baseId;
        _scene->_maxid = id > _scene->_maxid ? id :_scene-> _maxid;
        return id;
    }

    void SceneNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        scene_create ( id );

        SceneNode* sceneNode = NodeMgr::getInst().getNodePtr<SceneNode> (id);

        if ( 0 == parent )
            _scene->push_back ( sceneNode );
        else
            parent->addChild ( sceneNode );

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, sceneNode );

        if ( XercesHelper::hasAttribute ( tagElement, "translate" ) )
            sceneNode->setTranslate ( (char*)XercesHelper::getAttribute ( tagElement, "translate" ) );

        if ( XercesHelper::hasAttribute ( tagElement, "scale" ) )
            sceneNode->setScale ( (char*)XercesHelper::getAttribute ( tagElement, "scale" ) );

        _scene->traverseNode ( tagElement, sceneNode );

    }

    void FontNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        font_create ( id );

        FontNode* fontnode = NodeMgr::getInst().getNodePtr<FontNode> (id);

        if ( 0 == parent )
            _scene->push_back ( fontnode );
        else
            parent->addChild ( fontnode );


        if ( XercesHelper::hasAttribute ( tagElement, "def" ) )
        {
            string def = (const char*)XercesHelper::getAttribute ( tagElement, "def" );
            _scene->_defines[def] = fontnode;
            fontnode->defName ( def );
        }
        if ( XercesHelper::hasAttribute ( tagElement, "family" ) )
            fontnode->family ( (const char*)XercesHelper::getAttribute ( tagElement, "family" ) );
        if ( XercesHelper::hasAttribute ( tagElement, "size" ) )
            fontnode->pointSize ( atoi((const char*)XercesHelper::getAttribute ( tagElement, "size" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "style" ) )
        {
            string style = (const char*)XercesHelper::getAttribute ( tagElement, "style" );
            int istyle = 1;
            if ( style == "bold" )
                istyle = 2;
            else if ( style == "italic" )
                istyle = 3;
            else if ( style == "bolditalic" )
                istyle = 4;
            //font_style ( id, istyle );
        }

        _scene->traverseNode ( tagElement, fontnode );
    }
    void LayerNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        layer_create ( id );

        LayerNode* layer = NodeMgr::getInst().getNodePtr<LayerNode> (id);
        if ( 0 == parent )
            _scene->push_back ( layer );
        else
            parent->addChild ( layer );

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, layer );

        if ( XercesHelper::hasAttribute ( tagElement, "name" ) )
            layer->name ( (char*)XercesHelper::getAttribute ( tagElement, "name" ) );
        if ( XercesHelper::hasAttribute ( tagElement, "fgcolor" ) )
            layer->setFgColor ( GColor((char*)XercesHelper::getAttribute ( tagElement, "fgcolor" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "bgcolor" ) )
            layer->setBgColor ( GColor((char*)XercesHelper::getAttribute ( tagElement, "bgcolor" )) );

// 	qDebug ("LayerNodeParser::parse");
        _scene->traverseNode ( tagElement, layer );
    }
    void PickablegroupNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        pickablegroup_create ( id );

        PickableGroup* pick = NodeMgr::getInst().getNodePtr<PickableGroup> (id);
        if ( 0 == parent )
            _scene->push_back ( pick );
        else
            parent->addChild ( pick );

        if ( XercesHelper::hasAttribute ( tagElement, "name" ) )
            pick->name ( (char*)XercesHelper::getAttribute ( tagElement, "name" ) );

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, pick );

// 	qDebug ("PickablegroupNodeParser::parse");
        _scene->traverseNode ( tagElement, pick );
    }
    void LodNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        lod_create ( id );

        LODNode* lod = NodeMgr::getInst().getNodePtr<LODNode> (id);
        if ( 0 == parent )
            _scene->push_back ( lod );
        else
            parent->addChild ( lod );

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, lod );

        // set up lodnode's properties
        if ( XercesHelper::hasAttribute ( tagElement, "range" ) )
            lod->setdelimiters ( (char*)XercesHelper::getAttribute ( tagElement, "range" ) );

        _scene->traverseNode ( tagElement, lod );
    }
    void KDTreeNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        kdtreenode_create ( id );

        KdTreeNode* kdtreenode = NodeMgr::getInst().getNodePtr<KdTreeNode> (id);
        if ( 0 == parent )
            _scene->push_back ( kdtreenode );
        else
            parent->addChild ( kdtreenode );

        ParentFinder<KdTreeNode> finder (kdtreenode);
        if ( NULL == finder.target () )
            _scene->_kdtreenodes.push_back ( kdtreenode );

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, kdtreenode );

        _scene->traverseNode ( tagElement, kdtreenode );
    }
    void TransformNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        transform_create ( id );

        TransformNode* trans = NodeMgr::getInst().getNodePtr<TransformNode> (id);
        if ( 0 == parent )
            _scene->push_back ( trans );
        else
            parent->addChild ( trans );

        // set up transformnode's properties
        if ( XercesHelper::hasAttribute ( tagElement, "translate" ) )
            trans->setTranslate ( (char*)XercesHelper::getAttribute ( tagElement, "translate" ) );

        if ( XercesHelper::hasAttribute ( tagElement, "scale" ) )
            trans->setScale ( (char*)XercesHelper::getAttribute ( tagElement, "scale" ) );

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, trans );

// 	qDebug ("TransformNodeParser::parse");
        _scene->traverseNode ( tagElement, trans );
    }
    void ArrayNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        array_create ( id );

        ArrayNode* arraynode = NodeMgr::getInst().getNodePtr<ArrayNode> (id);
        if ( 0 == parent )
            _scene->push_back ( arraynode );
        else
            parent->addChild ( arraynode );

        if ( XercesHelper::hasAttribute ( tagElement, "cnty" ) ) arraynode->setRowCnt ( atoi((char*)XercesHelper::getAttribute( tagElement, "cnty" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "cntx" ) ) arraynode->setColumnCnt ( atoi((char*)XercesHelper::getAttribute( tagElement, "cntx" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "level0hcnt" ) ) arraynode->setHLevels ( 0, atoi((char*)XercesHelper::getAttribute( tagElement, "level0hcnt" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "level1hcnt" ) ) arraynode->setHLevels ( 1, atoi((char*)XercesHelper::getAttribute( tagElement, "level1hcnt" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "level2hcnt" ) ) arraynode->setHLevels ( 2, atoi((char*)XercesHelper::getAttribute( tagElement, "level2hcnt" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "level3hcnt" ) ) arraynode->setHLevels ( 3, atoi((char*)XercesHelper::getAttribute( tagElement, "level3hcnt" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "level4hcnt" ) ) arraynode->setHLevels ( 4, atoi((char*)XercesHelper::getAttribute( tagElement, "level4hcnt" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "level5hcnt" ) ) arraynode->setHLevels ( 5, atoi((char*)XercesHelper::getAttribute( tagElement, "level5hcnt" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "level0vcnt" ) ) arraynode->setVLevels ( 0, atoi((char*)XercesHelper::getAttribute( tagElement, "level0vcnt" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "level1vcnt" ) ) arraynode->setVLevels ( 1, atoi((char*)XercesHelper::getAttribute( tagElement, "level1vcnt" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "level2vcnt" ) ) arraynode->setVLevels ( 2, atoi((char*)XercesHelper::getAttribute( tagElement, "level2vcnt" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "level3vcnt" ) ) arraynode->setVLevels ( 3, atoi((char*)XercesHelper::getAttribute( tagElement, "level3vcnt" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "level4vcnt" ) ) arraynode->setVLevels ( 4, atoi((char*)XercesHelper::getAttribute( tagElement, "level4vcnt" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "level5vcnt" ) ) arraynode->setVLevels ( 5, atoi((char*)XercesHelper::getAttribute( tagElement, "level5vcnt" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "spacex1" ) ) arraynode->setMarginX ( 0, atof((char*)XercesHelper::getAttribute( tagElement, "spacex1" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "spacex2" ) ) arraynode->setMarginX ( 1, atof((char*)XercesHelper::getAttribute( tagElement, "spacex2" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "spacex3" ) ) arraynode->setMarginX ( 2, atof((char*)XercesHelper::getAttribute( tagElement, "spacex3" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "spacex4" ) ) arraynode->setMarginX ( 3, atof((char*)XercesHelper::getAttribute( tagElement, "spacex4" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "spacex5" ) ) arraynode->setMarginX ( 4, atof((char*)XercesHelper::getAttribute( tagElement, "spacex5" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "spacex6" ) ) arraynode->setMarginX ( 5, atof((char*)XercesHelper::getAttribute( tagElement, "spacex6" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "spacey1" ) ) arraynode->setMarginY ( 0, atof((char*)XercesHelper::getAttribute( tagElement, "spacey1" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "spacey2" ) ) arraynode->setMarginY ( 1, atof((char*)XercesHelper::getAttribute( tagElement, "spacey2" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "spacey3" ) ) arraynode->setMarginY ( 2, atof((char*)XercesHelper::getAttribute( tagElement, "spacey3" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "spacey4" ) ) arraynode->setMarginY ( 3, atof((char*)XercesHelper::getAttribute( tagElement, "spacey4" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "spacey5" ) ) arraynode->setMarginY ( 4, atof((char*)XercesHelper::getAttribute( tagElement, "spacey5" )) );
        if ( XercesHelper::hasAttribute ( tagElement, "spacey6" ) ) arraynode->setMarginY ( 5, atof((char*)XercesHelper::getAttribute( tagElement, "spacey6" )) );

        ParentFinder<ArrayNode> finder(arraynode);
        if ( NULL == finder.target() )
            _scene->_arraynodes.push_back ( arraynode );
        //        _allarraynodes.push_back ( arraynode );

        _scene->traverseNode ( tagElement, arraynode );
    }
    void RectNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        rectangle_create ( id );

        RectangleNodef* rect = NodeMgr::getInst().getNodePtr<RectangleNodef> (id);
        if ( 0 == parent )
            _scene->push_back ( rect );
        else
            parent->addChild ( rect );
        //Rectanglef* rect = new Rectanglef();
        //_temp.push_back ( rect );

        // set up rectnode's properties
        float w = 1.f, h = 1.f;
        if ( XercesHelper::hasAttribute ( tagElement, "width" ) ) 
            w = atof((char*)XercesHelper::getAttribute( tagElement, "width" ));
        if ( XercesHelper::hasAttribute ( tagElement, "height" ) ) 
            h = atof((char*)XercesHelper::getAttribute( tagElement, "height" ));
        rect->dimention ( w, h );

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, rect );

        // set drawablenode's attributes
        setDrawableNodeAttributes ( tagElement, rect );

        _scene->traverseNode ( tagElement, rect );
    }
    void TextNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
// 	qDebug ( "TextNodeParser::parse");
        int id = checkIDAttr ( tagElement );

        text_create ( id );

        TextNode* textnode = NodeMgr::getInst().getNodePtr<TextNode> (id);
        if ( 0 == parent )
            _scene->push_back ( textnode );
        else
            parent->addChild ( textnode );

// 	qDebug ( "111111111111111111111");
        if ( XercesHelper::hasAttribute ( tagElement, "anchor" ) )
            textnode->anchorValue ( atoi((const char*)XercesHelper::getAttribute ( tagElement, "anchor" )));
// 	qDebug ( "2222222222222222222");
        if ( XercesHelper::hasAttribute ( tagElement, "justify" ) )
            textnode->setAlignFlag ( atoi((const char*)XercesHelper::getAttribute ( tagElement, "justify" )));
// 	qDebug ( "3333333333333333333");
        if ( XercesHelper::hasAttribute ( tagElement, "sizemode" ) )
	{
	     // default is scene mode, so if can not regnice this attribute, we use screen node
	    string sizemode = (const char*)XercesHelper::getAttribute ( tagElement, "sizemode" );
	    if ( sizemode == "screenmode" )
		textnode->setSizeMode ( TextNode::TXTSIZEMODE_SCREEN );
	}
// 	qDebug ( "444444444444444444444");
        if ( XercesHelper::hasAttribute ( tagElement, "width" ) )
            textnode->width ( atof((const char*)XercesHelper::getAttribute ( tagElement, "width" )));
// 	qDebug ( "5555555555555555555555");
        if ( XercesHelper::hasAttribute ( tagElement, "height" ) )
            textnode->height ( atof((const char*)XercesHelper::getAttribute ( tagElement, "height" )));
// 	qDebug ( "6666666666666666666666");
	string tttt = (const char*)XercesHelper::getTextContent (tagElement);
// 	qDebug ( "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa, %s, %d", tttt.c_str(), textnode);
        textnode->text ( tttt.c_str() );
// 	qDebug ( "777777777777777777777");

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, textnode );
// 	qDebug ( "88888888888888888888");

        // set drawablenode's attributes
        setDrawableNodeAttributes ( tagElement, textnode );

// 	qDebug ( "end TextNodeParser::parse");
        _scene->traverseNode ( tagElement, textnode );
    }
    void GroupNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        groupnode_create ( id );

        GroupNode* groupnode = NodeMgr::getInst().getNodePtr<GroupNode> (id);
        if ( 0 == parent )
            _scene->push_back ( groupnode );
        else
            parent->addChild ( groupnode );

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, groupnode );

        if ( XercesHelper::hasAttribute ( tagElement, "name" ) )
            groupnode->name ( (char*)XercesHelper::getAttribute ( tagElement, "name" ) );

        _scene->traverseNode ( tagElement, groupnode );
    }
    void SwitchNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        switchnode_create ( id );

        SwitchNode* switchnode = NodeMgr::getInst().getNodePtr<SwitchNode> (id);
        if ( 0 == parent )
            _scene->push_back ( switchnode );
        else
            parent->addChild ( switchnode );

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, switchnode );

        if ( XercesHelper::hasAttribute ( tagElement, "selection" ) )
            switchnode->selection ( atoi((const char*)XercesHelper::getAttribute ( tagElement, "selection" )) );

        _scene->traverseNode ( tagElement, switchnode );
    }
    void LineNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        line_create ( id );

        LineNodef* linenode = NodeMgr::getInst().getNodePtr<LineNodef> (id);
        if ( 0 == parent )
            _scene->push_back ( linenode );
        else
            parent->addChild ( linenode );

        float x1=0, y1=0, x2=0, y2=0;
        if ( XercesHelper::hasAttribute ( tagElement, "x1" ) )
            x1 = atof ( (char*)XercesHelper::getAttribute ( tagElement, "x1" ) );
        if ( XercesHelper::hasAttribute ( tagElement, "y1" ) )
            y1 = atof ( (char*)XercesHelper::getAttribute ( tagElement, "y1" ) );
        if ( XercesHelper::hasAttribute ( tagElement, "x2" ) )
            x2 = atof ( (char*)XercesHelper::getAttribute ( tagElement, "x2" ) );
        if ( XercesHelper::hasAttribute ( tagElement, "y2" ) )
            y2 = atof ( (char*)XercesHelper::getAttribute ( tagElement, "y2" ) );
        linenode->setPoints ( x1, y1, x2, y2 );

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, linenode );

        // set drawablenode's attributes
        setDrawableNodeAttributes ( tagElement, linenode );

// 	qDebug ("LineNodeParser::parse");
        _scene->traverseNode ( tagElement, linenode );
    }
    void MeshNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        mesh_create ( id );

        MeshNode3f* meshnode = NodeMgr::getInst().getNodePtr<MeshNode3f> (id);
        if ( 0 == parent )
            _scene->push_back ( meshnode );
        else
            parent->addChild ( meshnode );

        if ( XercesHelper::hasAttribute ( tagElement, "coords" ) )
        {
            istringstream iss((char*)XercesHelper::getAttribute ( tagElement, "coords" ));
            vector<string> tokens;
            copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
            vector<vec3f> tmp;
            for ( size_t i=0; i<tokens.size(); i+=3 )
            {
                tmp.push_back ( 
                    vec3f(
                    atof ( tokens[i].c_str() ), 
                    atof ( tokens[i+1].c_str() ),
                    atof ( tokens[i+2].c_str() )
                    )
                    );
            }
            if ( !tmp.empty() )
                meshnode->setCoords ( &(tmp[0]), tmp.size() );
        }

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, meshnode );

// 	qDebug ( "MeshNodeParser::parse" );
        _scene->traverseNode ( tagElement, meshnode );
    }
    void MeshlineNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
// 	qDebug ("MeshlineNodeParser::parse");
        int id = checkIDAttr ( tagElement );

        meshline_create ( id );

        MeshLineNode* meshlinenode = NodeMgr::getInst().getNodePtr<MeshLineNode> (id);
        if ( 0 == parent )
            _scene->push_back ( meshlinenode );
        else
            parent->addChild ( meshlinenode );

        if ( XercesHelper::hasAttribute ( tagElement, "type" ) )
        {
            int t = atoi ((char*)XercesHelper::getAttribute ( tagElement, "type" ));
            meshlinenode->type ( t );
        }

        // set index data
        istringstream iss((const char*)XercesHelper::getTextContent (tagElement));
        vector<string> tokens;
        copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
        vector<int> tmp;
        for ( size_t i=0; i<tokens.size(); i++ )
        {
            tmp.push_back ( atoi ( tokens[i].c_str() ) );
        }
        meshlinenode->setData ( &(tmp[0]), tmp.size() );

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, meshlinenode );

        // set drawablenode's attributes
        setDrawableNodeAttributes ( tagElement, meshlinenode );

// 	qDebug ("end MeshlineNodeParser::parse");
        _scene->traverseNode ( tagElement, meshlinenode );
    }
    void PolylineNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        polyline_create ( id );

        PolylineNode2Df* linenode = NodeMgr::getInst().getNodePtr<PolylineNode2Df> (id);
        if ( 0 == parent )
            _scene->push_back ( linenode );
        else
            parent->addChild ( linenode );

        // get coords
        vector<string> tokens;
        istringstream iss( (const char*)XercesHelper::getTextContent (tagElement) );
        copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens) );
        float* tempdata = new float[tokens.size()];
        float* bak = tempdata;
        for ( vector<string>::iterator pp=tokens.begin(); pp!=tokens.end(); ++pp, ++tempdata )
        {
            stringstream ss ( *pp );
            ss >> *tempdata;
        }
        vec2f* ptr = (vec2f*)bak;
        linenode->assignpoints ( ptr, ptr + tokens.size()/2 );
        delete[] bak;

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, linenode );

        // set drawablenode's attributes
        setDrawableNodeAttributes ( tagElement, linenode );

// 	qDebug ("PolylineNodeParser::parse");
        _scene->traverseNode ( tagElement, linenode );
    }
    void PolyNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        poly_create ( id );

        PolylineNode2Df* polynode = NodeMgr::getInst().getNodePtr<PolylineNode2Df> (id);
        if ( 0 == parent )
            _scene->push_back ( polynode );
        else
            parent->addChild ( polynode );

        // get coords
        vector<string> tokens;
        istringstream iss( (const char*)XercesHelper::getTextContent (tagElement) );
        copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens) );
        float* tempdata = new float[tokens.size()];
        float* bak = tempdata;
        for ( vector<string>::iterator pp=tokens.begin(); pp!=tokens.end(); ++pp, ++tempdata )
        {
            stringstream ss ( *pp );
            ss >> *tempdata;
        }
        vec2f* ptr = (vec2f*)bak;
        polynode->assignpoints ( ptr, ptr + tokens.size()/2 );
        delete[] bak;

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, polynode );

        // set drawablenode's attributes
        setDrawableNodeAttributes ( tagElement, polynode );

        _scene->traverseNode ( tagElement, polynode );
    }
    void PointNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        point_create ( id );

        PointNode* pointnode = NodeMgr::getInst().getNodePtr<PointNode> (id);
        if ( 0 == parent )
            _scene->push_back ( pointnode );
        else
            parent->addChild ( pointnode );

        if ( XercesHelper::hasAttribute ( tagElement, "isVisible" ) ) {
            string tmp = (char*)XercesHelper::getAttribute ( tagElement, "isVisible" );
            if ( tmp == "1" ) pointnode->setVisible ( true );
            else if ( tmp == "0" ) pointnode->setVisible ( false );
        }
        if ( XercesHelper::hasAttribute ( tagElement, "name" ) )
            pointnode->name ( (char*)XercesHelper::getAttribute ( tagElement, "name" ) );
        if ( XercesHelper::hasAttribute ( tagElement, "size" ) )
            pointnode->pointSize ( atof((char*)XercesHelper::getAttribute ( tagElement, "size" )) );

        // get coords
        vector<string> tokens;
        istringstream iss( (const char*)XercesHelper::getTextContent (tagElement) );
        copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens) );
        float x, y;
        stringstream ss ( tokens[0] );
        ss >> x;
        ss.clear ();
        ss.str ( tokens[1] );
        ss >> y;
        pointnode->xy ( x, y );

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, pointnode );

        // set drawablenode's attributes
        setDrawableNodeAttributes ( tagElement, pointnode );

        _scene->traverseNode ( tagElement, pointnode );
    }
    void MeshpointNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        if ( !XercesHelper::hasAttribute ( tagElement, "index" ) )
            throw std::invalid_argument ( "meshpoint's attribute idx is required, every meshpoint node should define a coord idx" );
        int idx = atoi((const char*)XercesHelper::getAttribute ( tagElement, "index" ));

        meshpoint_create ( id, idx );

        // parent == 0 means this node is top node
        MeshPointNode* pointnode = NodeMgr::getInst().getNodePtr<MeshPointNode> (id);
        if ( 0 == parent )
            _scene->push_back ( pointnode );
        else
            parent->addChild ( pointnode );

        if ( XercesHelper::hasAttribute ( tagElement, "size" ) )
        {
            pointnode->pointSize ( atof((const char*)XercesHelper::getAttribute ( tagElement, "size" )) );
            //qDebug ( "========mesh point's size = %f", pointnode->pointSize() );
        }

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, pointnode );

        // set drawablenode's attributes
        setDrawableNodeAttributes ( tagElement, pointnode );

        _scene->traverseNode ( tagElement, pointnode );
    }
    void CircleNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        circle_create ( id );

        // parent == 0 means this node is top node
        CircleNode* circlenode = NodeMgr::getInst().getNodePtr<CircleNode> (id);
        if ( 0 == parent )
            _scene->push_back ( circlenode );
        else
            parent->addChild ( circlenode );

        if ( XercesHelper::hasAttribute ( tagElement, "center" ) )
        {
            istringstream iss((char*)XercesHelper::getAttribute ( tagElement, "center" ));
            vector<string> tokens;
            copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
            circlenode->setCenter ( atof (tokens[0].c_str()), atof (tokens[1].c_str()) );
        }
        if ( XercesHelper::hasAttribute ( tagElement, "radius" ) )
        {
            circlenode->setRadius ( atof((const char*)XercesHelper::getAttribute ( tagElement, "radius" )) );
        }

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, circlenode );

        // set drawablenode's attributes
        setDrawableNodeAttributes ( tagElement, circlenode );

        _scene->traverseNode ( tagElement, circlenode );
    }
    void ImageNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        image_create ( id );

        // parent == 0 means this node is top node
        ImageNode* imagenode = NodeMgr::getInst().getNodePtr<ImageNode> (id);
        if ( 0 == parent )
            _scene->push_back ( imagenode );
        else
            parent->addChild ( imagenode );

        if ( XercesHelper::hasAttribute ( tagElement, "path" ) )
	    imagenode->filePath ( (const char*)XercesHelper::getAttribute ( tagElement, "path" ));

	float w=0, h=0;
        if ( XercesHelper::hasAttribute ( tagElement, "width" ) )
	    w = atof((const char*)XercesHelper::getAttribute ( tagElement, "width" ));
        if ( XercesHelper::hasAttribute ( tagElement, "height" ) )
	    h = atof((const char*)XercesHelper::getAttribute ( tagElement, "height" ));

	imagenode->size ( vec2f(w,h) );

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, imagenode );

        // set drawablenode's attributes
        setDrawableNodeAttributes ( tagElement, imagenode );

        _scene->traverseNode ( tagElement, imagenode );
    }
    void ImposterNodeParser::parse ( XERCES_CPP_NAMESPACE::DOMElement* tagElement, SGNode* parent )
    {
        int id = checkIDAttr ( tagElement );

        imposter_create ( id );

        // parent == 0 means this node is top node
        ImposterNode* imposternode = NodeMgr::getInst().getNodePtr<ImposterNode> (id);
        if ( 0 == parent )
            _scene->push_back ( imposternode );
        else
            parent->addChild ( imposternode );

        if ( XercesHelper::hasAttribute ( tagElement, "range" ) )
	    imposternode->setLodDelimiters ( (const char*)XercesHelper::getAttribute ( tagElement, "range" ));

        // parse & set sgnode's attributes
        setSGNodeAttributes ( tagElement, imposternode );

        // set drawablenode's attributes
        setDrawableNodeAttributes ( tagElement, imposternode );

        _scene->traverseNode ( tagElement, imposternode );
    }

}
