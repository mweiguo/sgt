

#include "loadmesh.h"

#include <xml_xerceshelper.h>

#include <algorithm>
#include <time.h>

#include "scenemgr.h"
#include "arrayexpander.h"
#include "parentfinder.h"
#include "nodeleter.h"
#include "nodedumper.h"
#include "linenode.h"
#include "qDebug"
#include "attrset.h"

UnloadNode::UnloadNode ( SGNode* sgnode )
{
    sgnode->setParentNode ( NULL );

    Nodeleter deleter;
    deleter ( sgnode ); 

    LayerMgr::getInst().clear();
}

LoadMesh::LoadMesh ( const char* fileName, bool needExpand, bool needSceneManagement )// : _opt(opt)
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

    _root->updateBBox();
    //NodeDumper dumper;
    //dumper ( _root );
    //qDebug ( "%s", dumper.dumpstring().c_str() );
}

void LoadMesh::traverseNode ( XERCES_CPP_NAMESPACE::DOMElement* pnode, SGNode* parent )
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
        //Rectanglef* rect = new Rectanglef();
        //_temp.push_back ( rect );

        // set up rectnode's properties
        float w = 1.f, h = 1.f;
        if ( XercesHelper::hasAttribute ( tagRect, "width" ) ) 
            w = atof((char*)XercesHelper::getAttribute( tagRect, "width" ));
        if ( XercesHelper::hasAttribute ( tagRect, "height" ) ) 
            h = atof((char*)XercesHelper::getAttribute( tagRect, "height" ));
        rect->dimention ( w, h );

        GColor *fgcolor, *bgcolor;
        ParentFinder<LayerNode> finder( rect );

        if ( XercesHelper::hasAttribute ( tagRect, "bgcolor" ) )
        {
            string fillcolor = (const char*)(XercesHelper::getAttribute( tagRect, "bgcolor" ));
            if ( bygroupstr == fillcolor )
            {
                bgcolor = finder.target()->getBgColorPtr();
            }
            else
                bgcolor = new GColor(fillcolor);
        }
        if ( XercesHelper::hasAttribute ( tagRect, "fgcolor" ) )
        {
            string bordercolor = (const char*)(XercesHelper::getAttribute( tagRect, "fgcolor" ));
            if ( bygroupstr == bordercolor )
            {
                fgcolor = finder.target()->getFgColorPtr();
            }
            else
                fgcolor = new GColor(bordercolor);
        }
        //if ( XercesHelper::hasAttribute ( tagRect, "fillcolor" ) )
        //{
        //    string fillcolor = (const char*)(XercesHelper::getAttribute( tagRect, "fillcolor" ));
        //    string bygroupstr = "bygroup";
        //    if ( bygroupstr == fillcolor )
        //        rect->useLayerFillColor ();
        //    else
        //        rect->setFillColor ( GColor(fillcolor) );
        //}
        //if ( XercesHelper::hasAttribute ( tagRect, "bordercolor" ) )
        //{
        //    string bordercolor = (const char*)(XercesHelper::getAttribute( tagRect, "bordercolor" ));
        //    string bygroupstr = "bygroup";
        //    if ( bygroupstr == bordercolor )
        //        rect->useLayerBorderColor ();
        //    else
        //        rect->setBorderColor ( GColor(bordercolor) );
        //}

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
        if ( XercesHelper::hasAttribute ( tagText, "fgcolor" ) )
        {
            GColor *fgcolor;
            ParentFinder<LayerNode> finder( textnode );
            string fgcolorStr = (const char*)(XercesHelper::getAttribute( tagText, "fgcolor" ));
            if ( bygroupstr == fgcolorStr )
                fgcolor = finder.target()->getFgColorPtr();
            else
                fgcolor = new GColor(fgcolorStr);
            AttrSet* set = textnode->getAttrSet ();
            set->setFgColor ( fgcolor );
        }
        

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
        pAttrSet->setFgColor ( new GColor("#000000FF") );
        linenode->setAttrSet ( pAttrSet );

        traverseNode ( tagLine, linenode );
    }

    DOMElements tagMeshes = XercesHelper::getChildElementsByTagName ( pnode, "mesh" );
    for ( DOMElements::iterator pp=tagMeshes.begin(); pp!=tagMeshes.end(); ++pp ) {
        DOMElement* tagMesh  = static_cast<DOMElement*>(*pp);
        MeshNode* meshnode = new MeshNode();

        if ( parent == NULL )
            _root = meshnode;
        else
             parent->addChild ( meshnode );

        traverseNode ( tagMesh, meshnode );
    }
}

void LoadMesh::getShapeGenParas (int index, int& s1, int& s2, int& s3, int& s4, int& s5, int& s6, int level0Cnt, int level1Cnt, int level2Cnt, int level3Cnt, int level4Cnt, int level5Cnt )
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



