

#include "sgr_loadscene.h"

#ifndef _USE_XERCES3_
#include <xml_xerceshelper.h>
#endif// _USE_XERCES3_

#include <algorithm>
#include <time.h>

#include "sgr_scenemgr.h"
#include "sgr_arrayexpander.h"
#include "sgr_parentfinder.h"
#include "sgr_nodeleter.h"
#include "sgr_nodedumper.h"
#include "sgr_linenode.h"
#include "qDebug"
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
        _maxid = 0;
        if ( useBaseId )
            _baseId = SeedGenerator::getInst().maxseed ();

        int clo = clock();
#ifndef _USE_XERCES3_
        XercesParser parser;
        ::XERCES_CPP_NAMESPACE::DOMDocument* doc = parser.parseFile ( fileName, false);
#else   //_USE_XERCES3_
        XercesDOMParser parser;
        parser.parse ( fileName );
        DOMDocument* doc = parser.getDocument();
#endif  //_USE_XERCES3_
        if(doc == NULL)     
            throw logic_error("Fail to load Shape Template");

        DOMElement* root = doc->getDocumentElement();
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

        if ( _maxid > SeedGenerator::getInst().maxseed () )
        {
            if ( 0 == _baseId )
                SeedGenerator::getInst().maxseed ( _maxid+1 );
            else
                SeedGenerator::getInst().maxseed ( _baseId + _maxid + 1 );
        }

        _root->updateBBox();
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

    void LoadScene::traverseNode ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent )
    {
        typedef vector<XERCES_CPP_NAMESPACE::DOMElement*> DOMElements;
        static string bygroupstr = "bygroup";

        DOMElements tagScenes = XercesHelper::getChildElementsByTagName ( pnode, "scene" );
        for ( DOMElements::iterator pp=tagScenes.begin(); pp!=tagScenes.end(); ++pp ) {
            DOMElement* tagScene  = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagScene, "id" ) )
                throw std::invalid_argument ( "scene's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagScene, "id" ));
            
            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;
            scene_create ( id );

            SceneNode* sceneNode = NodeMgr::getInst().getNodePtr<SceneNode> (id);
            if ( 0 == parent )
                _root = sceneNode;

            // parse & set sgnode's attributes
            setSGNodeAttributes ( tagScene, sceneNode );

            if ( XercesHelper::hasAttribute ( tagScene, "translate" ) )
                sceneNode->setTranslate ( (char*)XercesHelper::getAttribute ( tagScene, "translate" ) );

            if ( XercesHelper::hasAttribute ( tagScene, "scale" ) )
                sceneNode->setScale ( (char*)XercesHelper::getAttribute ( tagScene, "scale" ) );

            traverseNode ( tagScene, sceneNode );
        }

        DOMElements tagFonts = XercesHelper::getChildElementsByTagName ( pnode, "font" );
        for ( DOMElements::iterator pp=tagFonts.begin(); pp!=tagFonts.end(); ++pp ) {
            DOMElement* tagFont = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagFont, "id" ) )
                throw std::invalid_argument ( "font's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagFont, "id" ));

            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;
            font_create ( id );

            FontNode* fontnode = NodeMgr::getInst().getNodePtr<FontNode> (id);
            if ( 0 == parent )
                _root = fontnode;
            else
                parent->addChild ( fontnode );
            //fontnode->setID ( atoi((const char*)XercesHelper::getAttribute ( tagFont, "id" )) );

            if ( XercesHelper::hasAttribute ( tagFont, "def" ) )
            {
                string def = (const char*)XercesHelper::getAttribute ( tagFont, "def" );
                _defines[def] = fontnode;
                fontnode->defName ( def );
            }
            if ( XercesHelper::hasAttribute ( tagFont, "family" ) )
                fontnode->family ( (const char*)XercesHelper::getAttribute ( tagFont, "family" ) );
            if ( XercesHelper::hasAttribute ( tagFont, "size" ) )
                fontnode->size ( atof((const char*)XercesHelper::getAttribute ( tagFont, "size" )) );
            if ( XercesHelper::hasAttribute ( tagFont, "style" ) )
            {
                string style = (const char*)XercesHelper::getAttribute ( tagFont, "style" );
                int istyle = 1;
                if ( style == "bold" )
                    istyle = 2;
                else if ( style == "italic" )
                    istyle = 3;
                else if ( style == "bolditalic" )
                    istyle = 4;
                //font_style ( id, istyle );
            }

            traverseNode ( tagFont, fontnode );
        }

        DOMElements tagLayers = XercesHelper::getChildElementsByTagName ( pnode, "layer" );
        for ( DOMElements::iterator pp=tagLayers.begin(); pp!=tagLayers.end(); ++pp ) {
            DOMElement* tagLayer  = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagLayer, "id" ) )
                throw std::invalid_argument ( "layer's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagLayer, "id" ));

            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;
            layer_create ( id );

            LayerNode* layer = NodeMgr::getInst().getNodePtr<LayerNode> (id);
            if ( 0 == parent )
                _root = layer;
            else
                parent->addChild ( layer );

            // parse & set sgnode's attributes
            setSGNodeAttributes ( tagLayer, layer );

            if ( XercesHelper::hasAttribute ( tagLayer, "name" ) )
                layer->name ( (char*)XercesHelper::getAttribute ( tagLayer, "name" ) );
            if ( XercesHelper::hasAttribute ( tagLayer, "fgcolor" ) )
                layer->setFgColor ( GColor((char*)XercesHelper::getAttribute ( tagLayer, "fgcolor" )) );
            if ( XercesHelper::hasAttribute ( tagLayer, "bgcolor" ) )
                layer->setBgColor ( GColor((char*)XercesHelper::getAttribute ( tagLayer, "bgcolor" )) );

            traverseNode ( tagLayer, layer );
        }

        DOMElements tagPickables = XercesHelper::getChildElementsByTagName ( pnode, "pickablegroup" );
        for ( DOMElements::iterator pp=tagPickables.begin(); pp!=tagPickables.end(); ++pp ) {
            DOMElement* tagPickable  = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagPickable, "id" ) )
                throw std::invalid_argument ( "pickablegroup's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagPickable, "id" ));

            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;
            pickablegroup_create ( id );

            PickableGroup* pick = NodeMgr::getInst().getNodePtr<PickableGroup> (id);
            if ( 0 == parent )
                _root = pick;
            else
                parent->addChild ( pick );

            if ( XercesHelper::hasAttribute ( tagPickable, "name" ) )
                pick->name ( (char*)XercesHelper::getAttribute ( tagPickable, "name" ) );

            // parse & set sgnode's attributes
            setSGNodeAttributes ( tagPickable, pick );

            traverseNode ( tagPickable, pick );
        }

        DOMElements tagLods = XercesHelper::getChildElementsByTagName ( pnode, "lod" );
        for ( DOMElements::iterator pp=tagLods.begin(); pp!=tagLods.end(); ++pp ) {
            DOMElement* tagLod  = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagLod, "id" ) )
                throw std::invalid_argument ( "lod's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagLod, "id" ));

            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;
            lod_create ( id );

            LODNode* lod = NodeMgr::getInst().getNodePtr<LODNode> (id);
            if ( 0 == parent )
                _root = lod;
            else
                parent->addChild ( lod );

            // parse & set sgnode's attributes
            setSGNodeAttributes ( tagLod, lod );

            // set up lodnode's properties
            if ( XercesHelper::hasAttribute ( tagLod, "range" ) )
                lod->setdelimiters ( (char*)XercesHelper::getAttribute ( tagLod, "range" ) );

            traverseNode ( tagLod, lod );
        }

        DOMElements tagKdTrees = XercesHelper::getChildElementsByTagName ( pnode, "kdtree" );
        for ( DOMElements::iterator pp=tagKdTrees.begin(); pp!=tagKdTrees.end(); ++pp ) {
            DOMElement* tagKdTree  = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagKdTree, "id" ) )
                throw std::invalid_argument ( "kdtree's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagKdTree, "id" ));

            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;
            kdtreenode_create ( id );

            KdTreeNode* kdtreenode = NodeMgr::getInst().getNodePtr<KdTreeNode> (id);
            if ( 0 == parent )
                _root = kdtreenode;
            else
                parent->addChild ( kdtreenode );

            ParentFinder<KdTreeNode> finder (kdtreenode);
            if ( NULL == finder.target () )
                _kdtreenodes.push_back ( kdtreenode );

            // parse & set sgnode's attributes
            setSGNodeAttributes ( tagKdTree, kdtreenode );

            traverseNode ( tagKdTree, kdtreenode );
        }

        DOMElements tagTransforms = XercesHelper::getChildElementsByTagName ( pnode, "transform" );
        for ( DOMElements::iterator pp=tagTransforms.begin(); pp!=tagTransforms.end(); ++pp ) {
            DOMElement* tagTransform  = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagTransform, "id" ) )
                throw std::invalid_argument ( "transform's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagTransform, "id" ));

            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;
            transform_create ( id );

            TransformNode* trans = NodeMgr::getInst().getNodePtr<TransformNode> (id);
            if ( 0 == parent )
                _root = trans;
            else
                parent->addChild ( trans );

            // set up transformnode's properties
            if ( XercesHelper::hasAttribute ( tagTransform, "translate" ) )
                trans->setTranslate ( (char*)XercesHelper::getAttribute ( tagTransform, "translate" ) );

            if ( XercesHelper::hasAttribute ( tagTransform, "scale" ) )
                trans->setScale ( (char*)XercesHelper::getAttribute ( tagTransform, "scale" ) );

            // parse & set sgnode's attributes
            setSGNodeAttributes ( tagTransform, trans );

            traverseNode ( tagTransform, trans );
        }

        DOMElements tagArrays = XercesHelper::getChildElementsByTagName ( pnode, "array" );
        for ( DOMElements::iterator pp=tagArrays.begin(); pp!=tagArrays.end(); ++pp ) {
            DOMElement* tagArray = static_cast<DOMElement*>( *pp );

            if ( !XercesHelper::hasAttribute ( tagArray, "id" ) )
                throw std::invalid_argument ( "array's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagArray, "id" ));

            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;
            array_create ( id );

            ArrayNode* arraynode = NodeMgr::getInst().getNodePtr<ArrayNode> (id);
            if ( 0 == parent )
                _root = arraynode;
            else
                parent->addChild ( arraynode );

            if ( XercesHelper::hasAttribute ( tagArray, "cnty" ) ) arraynode->setRowCnt ( atoi((char*)XercesHelper::getAttribute( tagArray, "cnty" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "cntx" ) ) arraynode->setColumnCnt ( atoi((char*)XercesHelper::getAttribute( tagArray, "cntx" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "level0hcnt" ) ) arraynode->setHLevels ( 0, atoi((char*)XercesHelper::getAttribute( tagArray, "level0hcnt" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "level1hcnt" ) ) arraynode->setHLevels ( 1, atoi((char*)XercesHelper::getAttribute( tagArray, "level1hcnt" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "level2hcnt" ) ) arraynode->setHLevels ( 2, atoi((char*)XercesHelper::getAttribute( tagArray, "level2hcnt" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "level3hcnt" ) ) arraynode->setHLevels ( 3, atoi((char*)XercesHelper::getAttribute( tagArray, "level3hcnt" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "level4hcnt" ) ) arraynode->setHLevels ( 4, atoi((char*)XercesHelper::getAttribute( tagArray, "level4hcnt" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "level5hcnt" ) ) arraynode->setHLevels ( 5, atoi((char*)XercesHelper::getAttribute( tagArray, "level5hcnt" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "level0vcnt" ) ) arraynode->setVLevels ( 0, atoi((char*)XercesHelper::getAttribute( tagArray, "level0vcnt" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "level1vcnt" ) ) arraynode->setVLevels ( 1, atoi((char*)XercesHelper::getAttribute( tagArray, "level1vcnt" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "level2vcnt" ) ) arraynode->setVLevels ( 2, atoi((char*)XercesHelper::getAttribute( tagArray, "level2vcnt" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "level3vcnt" ) ) arraynode->setVLevels ( 3, atoi((char*)XercesHelper::getAttribute( tagArray, "level3vcnt" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "level4vcnt" ) ) arraynode->setVLevels ( 4, atoi((char*)XercesHelper::getAttribute( tagArray, "level4vcnt" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "level5vcnt" ) ) arraynode->setVLevels ( 5, atoi((char*)XercesHelper::getAttribute( tagArray, "level5vcnt" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "spacex1" ) ) arraynode->setMarginX ( 0, atof((char*)XercesHelper::getAttribute( tagArray, "spacex1" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "spacex2" ) ) arraynode->setMarginX ( 1, atof((char*)XercesHelper::getAttribute( tagArray, "spacex2" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "spacex3" ) ) arraynode->setMarginX ( 2, atof((char*)XercesHelper::getAttribute( tagArray, "spacex3" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "spacex4" ) ) arraynode->setMarginX ( 3, atof((char*)XercesHelper::getAttribute( tagArray, "spacex4" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "spacex5" ) ) arraynode->setMarginX ( 4, atof((char*)XercesHelper::getAttribute( tagArray, "spacex5" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "spacex6" ) ) arraynode->setMarginX ( 5, atof((char*)XercesHelper::getAttribute( tagArray, "spacex6" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "spacey1" ) ) arraynode->setMarginY ( 0, atof((char*)XercesHelper::getAttribute( tagArray, "spacey1" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "spacey2" ) ) arraynode->setMarginY ( 1, atof((char*)XercesHelper::getAttribute( tagArray, "spacey2" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "spacey3" ) ) arraynode->setMarginY ( 2, atof((char*)XercesHelper::getAttribute( tagArray, "spacey3" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "spacey4" ) ) arraynode->setMarginY ( 3, atof((char*)XercesHelper::getAttribute( tagArray, "spacey4" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "spacey5" ) ) arraynode->setMarginY ( 4, atof((char*)XercesHelper::getAttribute( tagArray, "spacey5" )) );
            if ( XercesHelper::hasAttribute ( tagArray, "spacey6" ) ) arraynode->setMarginY ( 5, atof((char*)XercesHelper::getAttribute( tagArray, "spacey6" )) );

            ParentFinder<ArrayNode> finder(arraynode);
            if ( NULL == finder.target() )
                _arraynodes.push_back ( arraynode );
            //        _allarraynodes.push_back ( arraynode );

            traverseNode ( tagArray, arraynode );
        }

        DOMElements tagRects = XercesHelper::getChildElementsByTagName ( pnode, "rect" );
        for ( DOMElements::iterator pp=tagRects.begin(); pp!=tagRects.end(); ++pp ) {
            DOMElement* tagRect  = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagRect, "id" ) )
                throw std::invalid_argument ( "rect's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagRect, "id" ));

            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;
            rectangle_create ( id );

            RectangleNodef* rect = NodeMgr::getInst().getNodePtr<RectangleNodef> (id);
            if ( 0 == parent )
                _root = rect;
            else
                parent->addChild ( rect );
            //Rectanglef* rect = new Rectanglef();
            //_temp.push_back ( rect );

            // set up rectnode's properties
            float w = 1.f, h = 1.f;
            if ( XercesHelper::hasAttribute ( tagRect, "width" ) ) 
                w = atof((char*)XercesHelper::getAttribute( tagRect, "width" ));
            if ( XercesHelper::hasAttribute ( tagRect, "height" ) ) 
                h = atof((char*)XercesHelper::getAttribute( tagRect, "height" ));
            rect->dimention ( w, h );

            // parse & set sgnode's attributes
            setSGNodeAttributes ( tagRect, rect );

            // set drawablenode's attributes
            setDrawableNodeAttributes ( tagRect, rect );

            traverseNode ( tagRect, rect );
        }

        DOMElements tagTexts = XercesHelper::getChildElementsByTagName ( pnode, "text" );
        for ( DOMElements::iterator pp=tagTexts.begin(); pp!=tagTexts.end(); ++pp ) {
            DOMElement* tagText = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagText, "id" ) )
                throw std::invalid_argument ( "text's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagText, "id" ));

            if ( _baseId )
                id += _baseId;

            _maxid = id > _maxid ? id : _maxid;
            text_create ( id );

            TextNode* textnode = NodeMgr::getInst().getNodePtr<TextNode> (id);
            if ( 0 == parent )
                _root = textnode;
            else
                parent->addChild ( textnode );


            if ( XercesHelper::hasAttribute ( tagText, "anchor" ) )
                textnode->anchorValue ( atoi((const char*)XercesHelper::getAttribute ( tagText, "anchor" )));
            if ( XercesHelper::hasAttribute ( tagText, "justify" ) )
                textnode->setAlignFlag ( atoi((const char*)XercesHelper::getAttribute ( tagText, "justify" )));
            textnode->text ( (const char*)XercesHelper::getTextContent (tagText) );

            // parse & set sgnode's attributes
            setSGNodeAttributes ( tagText, textnode );

            // set drawablenode's attributes
            setDrawableNodeAttributes ( tagText, textnode );

            traverseNode ( tagText, textnode );
        }

        DOMElements tagGroups = XercesHelper::getChildElementsByTagName ( pnode, "group" );
        for ( DOMElements::iterator pp=tagGroups.begin(); pp!=tagGroups.end(); ++pp ) {
            DOMElement* tagGroup  = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagGroup, "id" ) )
                throw std::invalid_argument ( "group's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagGroup, "id" ));

            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;
            groupnode_create ( id );

            GroupNode* groupnode = NodeMgr::getInst().getNodePtr<GroupNode> (id);
            if ( 0 == parent )
                _root = groupnode;
            else
                parent->addChild ( groupnode );

            // parse & set sgnode's attributes
            setSGNodeAttributes ( tagGroup, groupnode );

            if ( XercesHelper::hasAttribute ( tagGroup, "name" ) )
                groupnode->name ( (char*)XercesHelper::getAttribute ( tagGroup, "name" ) );

            traverseNode ( tagGroup, groupnode );
        }

        DOMElements tagSwitchs = XercesHelper::getChildElementsByTagName ( pnode, "switch" );
        for ( DOMElements::iterator pp=tagSwitchs.begin(); pp!=tagSwitchs.end(); ++pp ) {
            DOMElement* tagSwitch  = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagSwitch, "id" ) )
                throw std::invalid_argument ( "switch's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagSwitch, "id" ));

            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;
            switchnode_create ( id );

            SwitchNode* switchnode = NodeMgr::getInst().getNodePtr<SwitchNode> (id);
            if ( 0 == parent )
                _root = switchnode;
            else
                parent->addChild ( switchnode );

            // parse & set sgnode's attributes
            setSGNodeAttributes ( tagSwitch, switchnode );

            if ( XercesHelper::hasAttribute ( tagSwitch, "selection" ) )
                switchnode->selection ( atoi((const char*)XercesHelper::getAttribute ( tagSwitch, "selection" )) );

            traverseNode ( tagSwitch, switchnode );
        }

        DOMElements tagLines = XercesHelper::getChildElementsByTagName ( pnode, "line" );
        for ( DOMElements::iterator pp=tagLines.begin(); pp!=tagLines.end(); ++pp ) {
            DOMElement* tagLine  = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagLine, "id" ) )
                throw std::invalid_argument ( "line's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagLine, "id" ));

            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;

            line_create ( id );

            LineNodef* linenode = NodeMgr::getInst().getNodePtr<LineNodef> (id);
            if ( 0 == parent )
                _root = linenode;
            else
                parent->addChild ( linenode );

            float x1, y1, x2, y2;
            if ( XercesHelper::hasAttribute ( tagLine, "x1" ) )
                x1 = atof ( (char*)XercesHelper::getAttribute ( tagLine, "x1" ) );
            if ( XercesHelper::hasAttribute ( tagLine, "y1" ) )
                y1 = atof ( (char*)XercesHelper::getAttribute ( tagLine, "y1" ) );
            if ( XercesHelper::hasAttribute ( tagLine, "x2" ) )
                x2 = atof ( (char*)XercesHelper::getAttribute ( tagLine, "x2" ) );
            if ( XercesHelper::hasAttribute ( tagLine, "y2" ) )
                y2 = atof ( (char*)XercesHelper::getAttribute ( tagLine, "y2" ) );
            linenode->setPoints ( x1, y1, x2, y2 );

            // parse & set sgnode's attributes
            setSGNodeAttributes ( tagLine, linenode );

            // set drawablenode's attributes
            setDrawableNodeAttributes ( tagLine, linenode );

            traverseNode ( tagLine, linenode );
        }

        DOMElements tagMeshes = XercesHelper::getChildElementsByTagName ( pnode, "mesh" );
        for ( DOMElements::iterator pp=tagMeshes.begin(); pp!=tagMeshes.end(); ++pp ) {
            DOMElement* tagMesh  = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagMesh, "id" ) )
                throw std::invalid_argument ( "mesh's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagMesh, "id" ));

            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;

            mesh_create ( id );

            MeshNode3f* meshnode = NodeMgr::getInst().getNodePtr<MeshNode3f> (id);
            if ( 0 == parent )
                _root = meshnode;
            else
                parent->addChild ( meshnode );

            if ( XercesHelper::hasAttribute ( tagMesh, "coords" ) )
            {
                istringstream iss((char*)XercesHelper::getAttribute ( tagMesh, "coords" ));
                vector<string> tokens;
                copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
                vector<vec3f> tmp;
                for ( int i=0; i<tokens.size(); i+=3 )
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
            setSGNodeAttributes ( tagMesh, meshnode );

            traverseNode ( tagMesh, meshnode );
        }

        DOMElements tagMeshelines = XercesHelper::getChildElementsByTagName ( pnode, "meshline" );
        for ( DOMElements::iterator pp=tagMeshelines.begin(); pp!=tagMeshelines.end(); ++pp ) {
            DOMElement* tagMeshline  = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagMeshline, "id" ) )
                throw std::invalid_argument ( "meshline's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagMeshline, "id" ));

            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;

            meshline_create ( id );

            MeshLineNode* meshlinenode = NodeMgr::getInst().getNodePtr<MeshLineNode> (id);
            if ( 0 == parent )
                _root = meshlinenode;
            else
                parent->addChild ( meshlinenode );

            if ( XercesHelper::hasAttribute ( tagMeshline, "type" ) )
            {
                int t = atoi ((char*)XercesHelper::getAttribute ( tagMeshline, "type" ));
                meshlinenode->type ( t );
            }

            // set index data
            istringstream iss((const char*)XercesHelper::getTextContent (tagMeshline));
            vector<string> tokens;
            copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
            vector<int> tmp;
            for ( int i=0; i<tokens.size(); i++ )
            {
                tmp.push_back ( atoi ( tokens[i].c_str() ) );
            }
            meshlinenode->setData ( &(tmp[0]), tmp.size() );

            // parse & set sgnode's attributes
            setSGNodeAttributes ( tagMeshline, meshlinenode );

            // set drawablenode's attributes
            setDrawableNodeAttributes ( tagMeshline, meshlinenode );

            traverseNode ( tagMeshline, meshlinenode );
        }

        DOMElements tagPolylines = XercesHelper::getChildElementsByTagName ( pnode, "polyline" );
        for ( DOMElements::iterator pp=tagPolylines.begin(); pp!=tagPolylines.end(); ++pp ) {
            DOMElement* tagPolyline  = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagPolyline, "id" ) )
                throw std::invalid_argument ( "polyline's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagPolyline, "id" ));

            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;

            polyline_create ( id );

            PolylineNode2Df* linenode = NodeMgr::getInst().getNodePtr<PolylineNode2Df> (id);
            if ( 0 == parent )
                _root = linenode;
            else
                parent->addChild ( linenode );

            // get coords
            vector<string> tokens;
            istringstream iss( (const char*)XercesHelper::getTextContent (tagPolyline) );
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
            setSGNodeAttributes ( tagPolyline, linenode );

            // set drawablenode's attributes
            setDrawableNodeAttributes ( tagPolyline, linenode );

            traverseNode ( tagPolyline, linenode );
        }

        DOMElements tagPolys = XercesHelper::getChildElementsByTagName ( pnode, "poly" );
        for ( DOMElements::iterator pp=tagPolys.begin(); pp!=tagPolys.end(); ++pp ) {
            DOMElement* tagPolyline  = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagPolyline, "id" ) )
                throw std::invalid_argument ( "polyline's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagPolyline, "id" ));

            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;

            poly_create ( id );

            PolylineNode2Df* polynode = NodeMgr::getInst().getNodePtr<PolylineNode2Df> (id);
            if ( 0 == parent )
                _root = polynode;
            else
                parent->addChild ( polynode );

            // get coords
            vector<string> tokens;
            istringstream iss( (const char*)XercesHelper::getTextContent (tagPolyline) );
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
            setSGNodeAttributes ( tagPolyline, polynode );

            // set drawablenode's attributes
            setDrawableNodeAttributes ( tagPolyline, polynode );

            traverseNode ( tagPolyline, polynode );
        }

        DOMElements tagPoints = XercesHelper::getChildElementsByTagName ( pnode, "point" );
        for ( DOMElements::iterator pp=tagPoints.begin(); pp!=tagPoints.end(); ++pp ) {
            DOMElement* tagPoint  = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagPoint, "id" ) )
                throw std::invalid_argument ( "point's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagPoint, "id" ));

            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;

            point_create ( id );

            PointNode* pointnode = NodeMgr::getInst().getNodePtr<PointNode> (id);
            if ( 0 == parent )
                _root = pointnode;
            else
                parent->addChild ( pointnode );

            if ( XercesHelper::hasAttribute ( tagPoint, "isVisible" ) ) {
                string tmp = (char*)XercesHelper::getAttribute ( tagPoint, "isVisible" );
                if ( tmp == "1" ) pointnode->setVisible ( true );
                else if ( tmp == "0" ) pointnode->setVisible ( false );
            }

            // get coords
            vector<string> tokens;
            istringstream iss( (const char*)XercesHelper::getTextContent (tagPoint) );
            copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens) );
            float x, y;
            stringstream ss ( tokens[0] );
            ss >> x;
            ss.clear ();
            ss.str ( tokens[1] );
            ss >> y;
            pointnode->xy ( x, y );

            // parse & set sgnode's attributes
            setSGNodeAttributes ( tagPoint, pointnode );

            // set drawablenode's attributes
            setDrawableNodeAttributes ( tagPoint, pointnode );

            traverseNode ( tagPoint, pointnode );
        }

        DOMElements tagMeshPoints = XercesHelper::getChildElementsByTagName ( pnode, "meshpoint" );
        for ( DOMElements::iterator pp=tagMeshPoints.begin(); pp!=tagMeshPoints.end(); ++pp ) {
            DOMElement* tagMeshPoint  = static_cast<DOMElement*>(*pp);

            if ( !XercesHelper::hasAttribute ( tagMeshPoint, "id" ) )
                throw std::invalid_argument ( "meshpoint's attribute id is required, every node should define a unique id" );
            int id = atoi((const char*)XercesHelper::getAttribute ( tagMeshPoint, "id" ));

            if ( !XercesHelper::hasAttribute ( tagMeshPoint, "idx" ) )
                throw std::invalid_argument ( "meshpoint's attribute idx is required, every meshpoint node should define a coord idx" );
            int idx = atoi((const char*)XercesHelper::getAttribute ( tagMeshPoint, "ixd" ));

            if ( _baseId )
                id += _baseId;
            _maxid = id > _maxid ? id : _maxid;

            meshpoint_create ( id, idx );

            // parent == 0 means this node is top node
            MeshPointNode* pointnode = NodeMgr::getInst().getNodePtr<MeshPointNode> (id);
            if ( 0 == parent )
                _root = pointnode;
            else
                parent->addChild ( pointnode );

            // parse & set sgnode's attributes
            setSGNodeAttributes ( tagMeshPoint, pointnode );

            // set drawablenode's attributes
            setDrawableNodeAttributes ( tagMeshPoint, pointnode );

            traverseNode ( tagMeshPoint, pointnode );
        }
    }

    void LoadScene::setSGNodeAttributes ( DOMElement* tagNode, SGNode* node )
    {
        if ( XercesHelper::hasAttribute ( tagNode, "isVisible" ) ) {
            string tmp = (char*)XercesHelper::getAttribute ( tagNode, "isVisible" );
            if ( tmp == "1" ) node->setVisible ( true );
            else if ( tmp == "0" ) node->setVisible ( false );
        }

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
                node->setBBoxDirty ( false );
            }
        }
    }

    void LoadScene::setDrawableNodeAttributes ( DOMElement* tagNode, DrawableNode* node )
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

            map<string,SGNode*>::iterator pp = _defines.find (def);
            if ( pp != _defines.end() )
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
                AttrSet* set = new AttrSet ( finder.target()->getRenderOrder() );
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
            AttrSet* set = new AttrSet ( -1 );
            node->setAttrSet ( set );
            if ( NULL != bgcolor )
                set->setBgColor ( bgcolor );

            if ( NULL != fgcolor )
                set->setFgColor ( fgcolor );

            if ( NULL != fontnode )
                set->setFont ( fontnode );
        }

    }

    void LoadScene::getShapeGenParas (int index, int& s1, int& s2, int& s3, int& s4, int& s5, int& s6, int level0Cnt, int level1Cnt, int level2Cnt, int level3Cnt, int level4Cnt, int level5Cnt )
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


}
