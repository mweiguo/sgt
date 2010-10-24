#include "loadmesh.h"

#ifndef _USE_XERCES3_
#include <xml_xerceshelper.h>
#else// _USE_XERCES3_
// #include <xercesc/parsers/XercesDOMParser.hpp>
// #include <xercesc/dom/DOM.hpp>
// #include <xercesc/sax/HandlerBase.hpp>
// #include <xercesc/util/XMLString.hpp>
// #include <xercesc/util/PlatformUtils.hpp>
#include <memory>
using namespace xercesc;
#endif// _USE_XERCES3_
#include <algorithm>
#include <iostream>
#include <time.h>

#include "scenemgr.h"
#include "arrayexpander.h"
#include "parentfinder.h"
#include "nodeleter.h"
#include "nodedumper.h"
#include "linenode.h"
#include "qDebug"
#include "attrset.h"
#include "scenenode.h"
#include "nodes.h"

using namespace std;
UnloadNode::UnloadNode ( SGNode* sgnode )
{
    sgnode->setParentNode ( NULL );

    Nodeleter deleter;
    deleter ( sgnode ); 

    LayerMgr::getInst().clear();
}

#ifndef _USE_XERCES3_
LoadScene::LoadScene ( const char* fileName, bool needExpand, bool needSceneManagement )// : _opt(opt)
{
    int clo = clock();
    XercesParser parser;
    XERCES_CPP_NAMESPACE::DOMDocument* doc = parser.parseFile ( fileName, false);
    if(doc == NULL)     
        throw logic_error("Fail to load Shape Template");

    DOMElement* root = doc->getDocumentElement();
    if (root == NULL) 
        throw logic_error("invalid Shape Template file:");
    //char* tagName = (char*)XercesHelper::getTagName (root);
    qDebug ( "parseFile TAKE %d clock, %f (s)", clock() - clo,  (1.0*(clock() - clo))/CLOCKS_PER_SEC );

    clo = clock();
    _root = new SGNode();
    traverseNode ( root, _root );
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

    _root->updateBBox();
    //NodeDumper dumper;
    //dumper ( _root );
    //qDebug ( "%s", dumper.dumpstring().c_str() );
}

void LoadScene::traverseNode ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent )
{
    typedef vector<XERCES_CPP_NAMESPACE::DOMElement*> DOMElements;
    static string bygroupstr = "bygroup";

    DOMElements tagFonts = XercesHelper::getChildElementsByTagName ( pnode, "font" );
    for ( DOMElements::iterator pp=tagFonts.begin(); pp!=tagFonts.end(); ++pp ) {
        DOMElement* tagFont = static_cast<DOMElement*>(*pp);
        FontNode* fontnode = new FontNode();
        parent->addChild ( fontnode );

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
        LayerNode* layer = new LayerNode();
        parent->addChild ( layer );

        if ( XercesHelper::hasAttribute ( tagLayer, "isVisible" ) ) {
            string tmp = (char*)XercesHelper::getAttribute ( tagLayer, "isVisible" );
            if ( tmp == "1" ) layer->setVisible ( true );
            else if ( tmp == "0" ) layer->setVisible ( false );
        }
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
        PickableGroup* pick = new PickableGroup();
        parent->addChild ( pick );

        if ( XercesHelper::hasAttribute ( tagPickable, "name" ) )
            pick->name ( (char*)XercesHelper::getAttribute ( tagPickable, "name" ) );

        traverseNode ( tagPickable, pick );
    }

    DOMElements tagLods = XercesHelper::getChildElementsByTagName ( pnode, "lod" );
    for ( DOMElements::iterator pp=tagLods.begin(); pp!=tagLods.end(); ++pp ) {
        DOMElement* tagLod  = static_cast<DOMElement*>(*pp);
        LODNode* lod = new LODNode();
        parent->addChild ( lod );

        // set up lodnode's properties
        if ( XercesHelper::hasAttribute ( tagLod, "range" ) )
            lod->setdelimiters ( (char*)XercesHelper::getAttribute ( tagLod, "range" ) );

        traverseNode ( tagLod, lod );
    }

    DOMElements tagKdTrees = XercesHelper::getChildElementsByTagName ( pnode, "kdtree" );
    for ( DOMElements::iterator pp=tagKdTrees.begin(); pp!=tagKdTrees.end(); ++pp ) {
        DOMElement* tagKdTree  = static_cast<DOMElement*>(*pp);
        KdTreeNode* kdtreenode = new KdTreeNode();
        parent->addChild ( kdtreenode );

        ParentFinder<KdTreeNode> finder (kdtreenode);
        if ( NULL == finder.target () )
            _kdtreenodes.push_back ( kdtreenode );

        traverseNode ( tagKdTree, kdtreenode );
    }

    DOMElements tagTransforms = XercesHelper::getChildElementsByTagName ( pnode, "transform" );
    for ( DOMElements::iterator pp=tagTransforms.begin(); pp!=tagTransforms.end(); ++pp ) {
        DOMElement* tagTransform  = static_cast<DOMElement*>(*pp);
        TransformNode* trans = new TransformNode();
        parent->addChild ( trans );

        // set up transformnode's properties
        if ( XercesHelper::hasAttribute ( tagTransform, "translate" ) )
            trans->setTranslate ( (char*)XercesHelper::getAttribute ( tagTransform, "translate" ) );

        if ( XercesHelper::hasAttribute ( tagTransform, "scale" ) )
            trans->setScale ( (char*)XercesHelper::getAttribute ( tagTransform, "scale" ) );

        traverseNode ( tagTransform, trans );
    }

    DOMElements tagArrays = XercesHelper::getChildElementsByTagName ( pnode, "array" );
    for ( DOMElements::iterator pp=tagArrays.begin(); pp!=tagArrays.end(); ++pp ) {
        DOMElement* tagArray = static_cast<DOMElement*>( *pp );
        ArrayNode* arraynode = new ArrayNode();
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

        traverseNode ( tagArray, arraynode );
    }

    DOMElements tagRects = XercesHelper::getChildElementsByTagName ( pnode, "rect" );
    for ( DOMElements::iterator pp=tagRects.begin(); pp!=tagRects.end(); ++pp ) {
        DOMElement* tagRect  = static_cast<DOMElement*>(*pp);
        RectangleNodef* rect = new RectangleNodef();
        parent->addChild ( rect );

        // set up rectnode's properties
        float w = 1.f, h = 1.f;
        if ( XercesHelper::hasAttribute ( tagRect, "width" ) ) 
            w = atof((char*)XercesHelper::getAttribute( tagRect, "width" ));
        if ( XercesHelper::hasAttribute ( tagRect, "height" ) ) 
            h = atof((char*)XercesHelper::getAttribute( tagRect, "height" ));
        rect->dimention ( w, h );

        GColor *fgcolor, *bgcolor;
        ParentFinder<LayerNode> finder( rect );

        bgcolor = finder.target()->getBgColorPtr();
        if ( XercesHelper::hasAttribute ( tagRect, "bgcolor" ) )
        {
            string fillcolor = (const char*)(XercesHelper::getAttribute( tagRect, "bgcolor" ));
            if ( bygroupstr != fillcolor )
                bgcolor = new GColor(fillcolor);
        }

        fgcolor = finder.target()->getFgColorPtr();
        if ( XercesHelper::hasAttribute ( tagRect, "fgcolor" ) )
        {
            string bordercolor = (const char*)(XercesHelper::getAttribute( tagRect, "fgcolor" ));
            if ( bygroupstr != bordercolor )
                fgcolor = new GColor(bordercolor);
        }

        int renderOrder = LayerMgr::getInst().indexof ( finder.target() );
        AttrSet* set = new AttrSet ( renderOrder );
        set->setFgColor ( fgcolor );
        set->setBgColor ( bgcolor );
        rect->setAttrSet ( set );

        traverseNode ( tagRect, rect );
    }

    DOMElements tagTexts = XercesHelper::getChildElementsByTagName ( pnode, "text" );
    for ( DOMElements::iterator pp=tagTexts.begin(); pp!=tagTexts.end(); ++pp ) {
        DOMElement* tagText = static_cast<DOMElement*>(*pp);
        TextNode* textnode = new TextNode();
        parent->addChild ( textnode );

        if ( XercesHelper::hasAttribute ( tagText, "font" ) )
        {
            string def = (const char*)(XercesHelper::getAttribute ( tagText, "font" ));
            // get reference & call text_font ( id, fontid );
            map<string,SGNode*>::iterator pp = _defines.find (def);
            if ( pp != _defines.end() )
            {
                textnode->fontnode ( static_cast<FontNode*>(pp->second) );

                ParentFinder<LayerNode> finder( textnode );
                int renderOrder = LayerMgr::getInst().indexof ( finder.target() );
                AttrSet* set = new AttrSet ( renderOrder );
                set->setFont ( textnode->fontnode() );
                textnode->setAttrSet ( set );
            }
            else
            {
                // log error & skip this text node
                qDebug ("font %s can not be fonted", def );
                //text_delete ( id );
                continue;
            }
        }
        if ( XercesHelper::hasAttribute ( tagText, "anchor" ) )
            textnode->anchorValue ( atoi((const char*)XercesHelper::getAttribute ( tagText, "anchor" )));
        if ( XercesHelper::hasAttribute ( tagText, "justify" ) )
            textnode->setAlignFlag ( atoi((const char*)XercesHelper::getAttribute ( tagText, "justify" )));
        textnode->text ( (const char*)XercesHelper::getTextContent (tagText) );
        //if ( XercesHelper::hasAttribute ( tagText, "string" ) )
        //    textnode->text ( (const char*)XercesHelper::getAttribute ( tagText, "string" ));
        ParentFinder<LayerNode> finder( textnode );
        GColor *fgcolor = finder.target()->getFgColorPtr();
        if ( XercesHelper::hasAttribute ( tagText, "fgcolor" ) )
        {
            string fgcolorStr = (const char*)(XercesHelper::getAttribute( tagText, "fgcolor" ));
            if ( bygroupstr != fgcolorStr )
                fgcolor = new GColor(fgcolorStr);
        }
        AttrSet* set = textnode->getAttrSet ();
        set->setFgColor ( fgcolor );

        traverseNode ( tagText, textnode );
    }

    DOMElements tagGroups = XercesHelper::getChildElementsByTagName ( pnode, "group" );
    for ( DOMElements::iterator pp=tagGroups.begin(); pp!=tagGroups.end(); ++pp ) {
        DOMElement* tagGroup  = static_cast<DOMElement*>(*pp);
        GroupNode* groupnode = new GroupNode();
        parent->addChild ( groupnode );

        if ( XercesHelper::hasAttribute ( tagGroup, "name" ) )
            groupnode->name ( (char*)XercesHelper::getAttribute ( tagGroup, "name" ) );

        traverseNode ( tagGroup, groupnode );
    }

    DOMElements tagSwitchs = XercesHelper::getChildElementsByTagName ( pnode, "switch" );
    for ( DOMElements::iterator pp=tagSwitchs.begin(); pp!=tagSwitchs.end(); ++pp ) {
        DOMElement* tagSwitch  = static_cast<DOMElement*>(*pp);
        SwitchNode* switchnode = new SwitchNode();
        parent->addChild ( switchnode );

        if ( XercesHelper::hasAttribute ( tagSwitch, "isVisible" ) ) {
            string tmp = (char*)XercesHelper::getAttribute ( tagSwitch, "isVisible" );
            if ( tmp == "1" ) switchnode->setVisible ( true );
            else if ( tmp == "0" ) switchnode->setVisible ( false );
        }

        traverseNode ( tagSwitch, switchnode );
    }

    DOMElements tagLines = XercesHelper::getChildElementsByTagName ( pnode, "line" );
    for ( DOMElements::iterator pp=tagLines.begin(); pp!=tagLines.end(); ++pp ) {
        DOMElement* tagLine  = static_cast<DOMElement*>(*pp);
        LineNodef* linenode = new LineNodef();
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
        
        ParentFinder<LayerNode> finder(linenode);
        int renderOrder = LayerMgr::getInst().indexof ( finder.target() );
        AttrSet* pAttrSet = new AttrSet(renderOrder);
        GColor *fgcolor = finder.target()->getFgColorPtr();
        pAttrSet->setFgColor ( fgcolor );
        linenode->setAttrSet ( pAttrSet );

        traverseNode ( tagLine, linenode );
    }

    DOMElements tagScenes = XercesHelper::getChildElementsByTagName ( pnode, "scene" );
    for ( DOMElements::iterator pp=tagScenes.begin(); pp!=tagScenes.end(); ++pp ) {
        DOMElement* tagScene  = static_cast<DOMElement*>(*pp);
        SceneNode* sceneNode = new SceneNode();

        parent->addChild ( sceneNode );

        if ( XercesHelper::hasAttribute ( tagScene, "isVisible" ) ) {
            string tmp = (char*)XercesHelper::getAttribute ( tagScene, "isVisible" );
            if ( tmp == "1" ) sceneNode->setVisible ( true );
            else if ( tmp == "0" ) sceneNode->setVisible ( false );
        }

        if ( XercesHelper::hasAttribute ( tagScene, "translate" ) )
            sceneNode->setTranslate ( (char*)XercesHelper::getAttribute ( tagScene, "translate" ) );

        if ( XercesHelper::hasAttribute ( tagScene, "scale" ) )
            sceneNode->setScale ( (char*)XercesHelper::getAttribute ( tagScene, "scale" ) );

        traverseNode ( tagScene, sceneNode );
    }

    DOMElements tagMeshes = XercesHelper::getChildElementsByTagName ( pnode, "mesh" );
    for ( DOMElements::iterator pp=tagMeshes.begin(); pp!=tagMeshes.end(); ++pp ) {
        DOMElement* tagMesh  = static_cast<DOMElement*>(*pp);
        MeshNode3f* meshnode = new MeshNode3f();
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
            meshnode->setCoords ( &(tmp[0]), tmp.size() );
        }

        traverseNode ( tagMesh, meshnode );
    }

    DOMElements tagMeshelines = XercesHelper::getChildElementsByTagName ( pnode, "meshline" );
    for ( DOMElements::iterator pp=tagMeshelines.begin(); pp!=tagMeshelines.end(); ++pp ) {
        DOMElement* tagMeshline  = static_cast<DOMElement*>(*pp);
        MeshLineNode* meshlinenode = new MeshLineNode();
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

        // set attrset
        ParentFinder<LayerNode> finder(meshlinenode);
        int renderOrder = LayerMgr::getInst().indexof ( finder.target() );
        AttrSet* set = new AttrSet ( renderOrder );
        GColor *fgcolor = finder.target()->getFgColorPtr();
        set->setFgColor ( fgcolor );
        meshlinenode->setAttrSet ( set );

        traverseNode ( tagMeshline, meshlinenode );
    }
}

#else// _USE_XERCES3_
#include <tinyLog.h>
LoadMesh::LoadMesh ( const char* fileName, bool needExpand, bool needSceneManagement )// : _opt(opt)
{
    LOG_INFO ( "LoadMesh::LoadMesh\n" );
    try
    {
	LOG_INFO ( "before clock\n" );
	int clo = clock();
	LOG_INFO ( "clock = %d\n", clo );
	XercesDOMParser parser;
	LOG_INFO ( "XercesDOMParser parser ok\n" );
	parser.setValidationScheme(XercesDOMParser::Val_Always);

	LOG_INFO ( "parser.setValidationScheme ok" );
	parser.parse ( fileName );

	LOG_INFO ( "parser.parse ok" );
	DOMDocument* doc = parser.getDocument();
	DOMElement* element = doc->getDocumentElement();


	LOG_INFO ( "parseFile TAKE %d clock, %f (s)", clock() - clo,  (1.0*(clock() - clo))/CLOCKS_PER_SEC );

	clo = clock();
	traverseNode ( element, 0 );
	LOG_INFO ( "traverseNode TAKE %d clock, %f (s)", clock() - clo,  (1.0*(clock() - clo))/CLOCKS_PER_SEC );

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

	_root->updateBBox();
    }
    catch (const XMLException& toCatch) 
    {
	char* message = XMLString::transcode(toCatch.getMessage());
	cout << "Exception message is: \n"
	     << message << "\n";
	XMLString::release(&message);
    }
    catch (const DOMException& toCatch) {
	char* message = XMLString::transcode(toCatch.msg);
	cout << "Exception message is: \n"
	     << message << "\n";
	XMLString::release(&message);
    }
    catch (...) {
	cout << "Unexpected Exception \n" ;
    }
}

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

void LoadMesh::traverseNode ( DOMElement* element, SGNode* parent )
{
    typedef vector<DOMElement*> DOMElements;
    static string bygroupstr = "bygroup";

    LOG_INFO ("begin");
    for ( DOMElement* p=element->getFirstElementChild();
	  p!=element->getLastElementChild();
	  p=p->getNextElementSibling() )
    {
	string tagName ( ChWrapper( p->getTagName () ));
	if ( tagName == "font" ) 
	{
	    FontNode* fontnode = new FontNode();
	    parent->addChild ( fontnode );

	    if ( XercesHelper::hasAttribute ( p, "def" ) )
	    {
		_defines[XercesHelper::getAttribute ( p, "def" ) ] = fontnode;
	    }

	    if ( XercesHelper::hasAttribute ( p, "family" ) )
	    {
		fontnode->family ( XercesHelper::getAttribute ( p, "family" ) );
	    }

	    if ( XercesHelper::hasAttribute ( p, "size" ) )
	    {
		stringstream ss ( XercesHelper::getAttribute ( p, "size" ) );
		int sz;
		ss >> sz;
		fontnode->size ( sz );
	    }

	    if ( XercesHelper::hasAttribute ( p, "style" ) )
	    {
		string style = XercesHelper::getAttribute ( p, "style" );
		int istyle = 1;
		if ( style == "bold" )
		    istyle = 2;
		else if ( style == "italic" )
		    istyle = 3;
		else if ( style == "bolditalic" )
		    istyle = 4;
		//font_style ( id, istyle );
	    }

	    traverseNode ( p, fontnode );
	} 
	else if ( tagName == "layer" ) 
	{
	    LayerNode* layer = new LayerNode();
	    parent->addChild ( layer );

	    if ( XercesHelper::hasAttribute ( p, "isVisible" ) )
	    {
		string tmp = XercesHelper::getAttribute ( p, "isVisible" );
		if ( tmp == "1" ) layer->setVisible ( true );
		else if ( tmp == "0" ) layer->setVisible ( false );
	    }

	    if ( XercesHelper::hasAttribute ( p, "name" ) )
	    {
		layer->name ( XercesHelper::getAttribute ( p, "name" ) );
	    }

	    if ( XercesHelper::hasAttribute ( p, "fgcolor" ) )
	    {
		layer->setFgColor ( GColor(XercesHelper::getAttribute ( p, "fgcolor" )));
	    }
 
	    traverseNode ( p, layer );
	}
	else if ( tagName == "pickablegroup" ) 
	{
	    PickableGroup* pick = new PickableGroup();
	    parent->addChild ( pick );

	    if ( XercesHelper::hasAttribute ( p, "name" ) )
	    {
		pick->name ( XercesHelper::getAttribute ( p, "name" ) );
	    }

	    traverseNode ( p, pick );
	}
	else if ( tagName == "lod" ) 
	{
	    LODNode* lod = new LODNode();
	    parent->addChild ( lod );

	    // set up lodnode's properties
	    if ( XercesHelper::hasAttribute ( p, "range" ) )
	    {
		lod->setdelimiters ( XercesHelper::getAttribute ( p, "range" ) );
	    }

	    traverseNode ( p, lod );
	} 
	else if ( tagName == "kdtree" ) 
	{
	    KdTreeNode* kdtreenode = new KdTreeNode();
	    parent->addChild ( kdtreenode );

	    ParentFinder<KdTreeNode> finder (kdtreenode);
	    if ( NULL == finder.target () )
		_kdtreenodes.push_back ( kdtreenode );

	    traverseNode ( p, kdtreenode );
	} 
	else if ( tagName == "transform" ) 
	{
	    TransformNode* trans = new TransformNode();
	    parent->addChild ( trans );

	    // set up transformnode's properties
	    if ( XercesHelper::hasAttribute ( p, "translate" ) )
		trans->setTranslate ( XercesHelper::getAttribute ( p, "translate" ) );

	    if ( XercesHelper::hasAttribute ( p, "translate" ) )
		trans->setScale ( XercesHelper::getAttribute ( p, "translate" ) );

	    traverseNode ( p, trans );

	} else if ( tagName == "array" ) {
	    ArrayNode* arraynode = new ArrayNode();
	    parent->addChild ( arraynode );


	    if ( XercesHelper::hasAttribute ( p, "cnty" ) )
		arraynode->setRowCnt ( atoi( XercesHelper::getAttribute( p, "cnty").c_str()) );

	    if ( XercesHelper::hasAttribute ( p, "cntx" ) ) 
		arraynode->setColumnCnt ( atoi( XercesHelper::getAttribute( p, "cntx").c_str()) );

	    if ( XercesHelper::hasAttribute ( p, "level0hcnt" ) )
		arraynode->setHLevels ( 0, atoi( XercesHelper::getAttribute( p, "level0hcnt").c_str()) );

	    if ( XercesHelper::hasAttribute ( p, "level1hcnt" ) ) 
		arraynode->setHLevels ( 1, atoi( XercesHelper::getAttribute( p, "level1hcnt").c_str()) );

	    if ( XercesHelper::hasAttribute ( p, "level2hcnt" ) )
		arraynode->setHLevels ( 2, atoi( XercesHelper::getAttribute( p, "level2hcnt").c_str()) );

	    if ( XercesHelper::hasAttribute ( p, "level3hcnt" ) ) 
		arraynode->setHLevels ( 3, atoi(XercesHelper::getAttribute( p, "level3hcnt").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "level4hcnt" ) ) 
		arraynode->setHLevels ( 4, atoi(XercesHelper::getAttribute( p, "level4hcnt").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "level5hcnt" ) ) 
		arraynode->setHLevels ( 5, atoi(XercesHelper::getAttribute( p, "level5hcnt").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "level0vcnt" ) ) 
		arraynode->setVLevels ( 0, atoi(XercesHelper::getAttribute( p, "level0vcnt").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "level1vcnt" ) ) 
		arraynode->setVLevels ( 1, atoi(XercesHelper::getAttribute( p, "level1vcnt").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "level2vcnt" ) ) 
		arraynode->setVLevels ( 2, atoi(XercesHelper::getAttribute( p, "level2vcnt").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "level3vcnt" ) ) 
		arraynode->setVLevels ( 3, atoi(XercesHelper::getAttribute( p, "level3vcnt").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "level4vcnt" ) ) 
		arraynode->setVLevels ( 4, atoi(XercesHelper::getAttribute( p, "level4vcnt").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "level5vcnt" ) ) 
		arraynode->setVLevels ( 5, atoi(XercesHelper::getAttribute( p, "level5vcnt").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "spacex1" ) ) 
		arraynode->setMarginX ( 0, atof(XercesHelper::getAttribute( p, "spacex1").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "spacex2" ) ) 
		arraynode->setMarginX ( 1, atof(XercesHelper::getAttribute( p, "spacex2").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "spacex3" ) ) 
		arraynode->setMarginX ( 2, atof(XercesHelper::getAttribute( p, "spacex3").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "spacex4" ) ) 
		arraynode->setMarginX ( 3, atof(XercesHelper::getAttribute( p, "spacex4").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "spacex5" ) ) 
		arraynode->setMarginX ( 4, atof(XercesHelper::getAttribute( p, "spacex5").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "spacex6" ) ) 
		arraynode->setMarginX ( 5, atof(XercesHelper::getAttribute( p, "spacex6").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "spacey1" ) ) 
		arraynode->setMarginY ( 0, atof(XercesHelper::getAttribute( p, "spacey1").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "spacey2" ) ) 
		arraynode->setMarginY ( 1, atof(XercesHelper::getAttribute( p, "spacey2").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "spacey3" ) ) 
		arraynode->setMarginY ( 2, atof(XercesHelper::getAttribute( p, "spacey3").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "spacey4" ) ) 
		arraynode->setMarginY ( 3, atof(XercesHelper::getAttribute( p, "spacey4").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "spacey5" ) ) 
		arraynode->setMarginY ( 4, atof(XercesHelper::getAttribute( p, "spacey5").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "spacey6" ) ) 
		arraynode->setMarginY ( 5, atof(XercesHelper::getAttribute( p, "spacey6").c_str() ));


	    ParentFinder<ArrayNode> finder(arraynode);
	    if ( NULL == finder.target() )
		_arraynodes.push_back ( arraynode );

	    traverseNode ( p, arraynode );

	} else if ( tagName == "rect" ) {
	    RectangleNodef* rect = new RectangleNodef();
	    parent->addChild ( rect );

	    // set up rectnode's properties
	    float w = 1.f, h = 1.f;

	    if ( XercesHelper::hasAttribute ( p, "width" ) )
		w = atof ( XercesHelper::getAttribute( p, "width") );

	    if ( XercesHelper::hasAttribute ( p, "height" ) ) 
		h = atof( XercesHelper::getAttribute( p, "height") );

	    rect->dimention ( w, h );

	    GColor *fgcolor, *bgcolor;
	    ParentFinder<LayerNode> finder( rect );

	    if ( XercesHelper::hasAttribute ( p, "bgcolor" ) )
	    {
		string fillcolor = XercesHelper::getAttribute ( p, "bgcolor" );
		if ( bygroupstr == fillcolor )
		    bgcolor = finder.target()->getBgColorPtr();
		else
		    bgcolor = new GColor(fillcolor);
	    }

	    if ( XercesHelper::hasAttribute ( p, "fgcolor" ) )
	    {
		string bordercolor = XercesHelper::getAttribute( p, "fgcolor" );
		if ( bygroupstr == bordercolor )
		    fgcolor = finder.target()->getFgColorPtr();
		else
		    fgcolor = new GColor(bordercolor);
	    }

	    if ( NULL!=fgcolor || NULL!=bgcolor )
	    {
		int renderOrder = LayerMgr::getInst().indexof ( finder.target() );
		AttrSet* set = new AttrSet ( renderOrder );
		if ( fgcolor )
		    set->setFgColor ( fgcolor );
		if ( bgcolor )
		    set->setBgColor ( bgcolor );
		rect->setAttrSet ( set );
	    }

	    traverseNode ( p, rect );

	} else if ( tagName == "text" ) {
	    TextNode* textnode = new TextNode();
	    parent->addChild ( textnode );

	    {
		if ( XercesHelper::hasAttribute ( p, "font" ) )
		{
		    string def = XercesHelper::getAttribute ( p, "font");
		    // get reference & call text_font ( id, fontid );
		    map<string,SGNode*>::iterator pp = _defines.find (def);
		    if ( pp != _defines.end() )
		    {
			textnode->fontnode ( static_cast<FontNode*>(pp->second) );

			ParentFinder<LayerNode> finder( textnode );
			int renderOrder = LayerMgr::getInst().indexof ( finder.target() );
			AttrSet* set = new AttrSet ( renderOrder );
			set->setFont ( textnode->fontnode() );
			textnode->setAttrSet ( set );
		    }
		    else
		    {
			// log error & skip this text node
			qDebug ("font %s can not be fonted", def );
			//text_delete ( id );
			continue;
		    }
		}
	    }

	    if ( XercesHelper::hasAttribute ( p, "anchor" ) )
		textnode->anchorValue ( atoi( XercesHelper::getAttribute ( p, "anchor").c_str() ));

	    if ( XercesHelper::hasAttribute ( p, "justify" ) )
		textnode->setAlignFlag ( atoi( XercesHelper::getAttribute ( p, "justify").c_str() ));

	    textnode->text ( (char*)ChWrapper(p->getTextContent ()) );

	    if ( XercesHelper::hasAttribute ( p, "fgcolor" ) )
	    {
		GColor *fgcolor;
		ParentFinder<LayerNode> finder( textnode );
		string fgcolorStr = XercesHelper::getAttribute( p, "fgcolor");
		if ( bygroupstr == fgcolorStr )
		    fgcolor = finder.target()->getFgColorPtr();
		else
		    fgcolor = new GColor(fgcolorStr);
		AttrSet* set = textnode->getAttrSet ();
		set->setFgColor ( fgcolor );
	    }

	    traverseNode ( p, textnode );

	} 
	else if ( tagName == "group" ) 
	{
	    GroupNode* groupnode = new GroupNode();
	    parent->addChild ( groupnode );

	    if ( XercesHelper::hasAttribute ( p, "name" ) )
		groupnode->name ( XercesHelper::getAttribute ( p, "name"));

	    traverseNode ( p, groupnode );

	} 
	else if ( tagName == "switch" ) 
	{
	    SwitchNode* switchnode = new SwitchNode();
	    parent->addChild ( switchnode );

	    if ( XercesHelper::hasAttribute ( p, "isVisible" ) ) {
		string tmp = XercesHelper::getAttribute ( p, "isVisible" );
		if ( tmp == "1" ) switchnode->setVisible ( true );
		else if ( tmp == "0" ) switchnode->setVisible ( false );
	    }


	    traverseNode ( p, switchnode );

	} 
	else if ( tagName == "line" ) 
	{
	    LineNodef* linenode = new LineNodef();
	    parent->addChild ( linenode );

	    float x1, y1, x2, y2;

	    if ( XercesHelper::hasAttribute ( p, "x1" ) )
		x1 = atof ( XercesHelper::getAttribute ( p, "x1" ).c_str() );

	    if ( XercesHelper::hasAttribute ( p, "y1" ) )
		y1 = atof ( XercesHelper::getAttribute ( p, "y1" ).c_str() );

	    if ( XercesHelper::hasAttribute ( p, "x2" ) )
		x2 = atof ( XercesHelper::getAttribute ( p, "x2" ).c_str() );

	    if ( XercesHelper::hasAttribute ( p, "y2" ) )
		y2 = atof ( XercesHelper::getAttribute ( p, "y2" ).c_str() );

	    linenode->setPoints ( x1, y1, x2, y2 );
        
	    ParentFinder<LayerNode> finder(linenode);
	    int renderOrder = LayerMgr::getInst().indexof ( finder.target() );
	    AttrSet* pAttrSet = new AttrSet(renderOrder);
	    pAttrSet->setFgColor ( new GColor("#000000FF") );
	    linenode->setAttrSet ( pAttrSet );

	    traverseNode ( p, linenode );
	    
	} else if ( tagName == "mesh" ) {
	    MeshNode* meshnode = new MeshNode();

	    if ( parent == NULL )
		_root = meshnode;
	    else
		parent->addChild ( meshnode );

	    traverseNode ( p, meshnode );
	} 
    }

}
#endif// _USE_XERCES3_

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



