
#include "sgr_layerchangger.h"
#include "sgr_attrset.h"

namespace SGR
{
    void OnRemoveNodeFromLayer::doAction ( LayerNode* layer, SGNode* child )
    {
        _attrset  = layer->getAttrSet();
        _fgColor  = _attrset->getFgColor();
        _bgColor  = _attrset->getBgColor();
        _fontnode = _attrset->getFont();

        child->accept ( *this );
    }

    void OnRemoveNodeFromLayer::updateNodeAttrSet ( DrawableNode& node )
    {
        AttrSet* attrset = node.getAttrSet();
        if ( attrset == _attrset )
            node.setAttrSet ( 0 );
        else
        {
            GColor* fgColor    = attrset->getFgColor();
            GColor* bgColor    = attrset->getBgColor();
            FontNode* fontnode = attrset->getFont();
            if ( fgColor == _fgColor )
                attrset->setFgColor ( 0 );
            if ( bgColor == _bgColor )
                attrset->setBgColor ( 0 );
            if ( fontnode == _fontnode )
                attrset->setFont ( 0 );
        }
    }

    void OnRemoveNodeFromLayer::apply ( RectangleNodef& node )
    {
        updateNodeAttrSet ( node );
    }

    void OnRemoveNodeFromLayer::apply ( TextNode& node )
    {
        updateNodeAttrSet ( node );
    }

    void OnRemoveNodeFromLayer::apply ( LineNodef& node )
    {
        updateNodeAttrSet ( node );
    }

    void OnRemoveNodeFromLayer::apply ( MeshLineNode& node )
    {
        updateNodeAttrSet ( node );
    }

    void OnRemoveNodeFromLayer::apply ( PolylineNode2Df& node )
    {
        updateNodeAttrSet ( node );
    }

    void OnRemoveNodeFromLayer::apply ( PolyNode2Df& node )
    {
        updateNodeAttrSet ( node );
    }

    void OnRemoveNodeFromLayer::apply ( PointNode& node )
    {
        updateNodeAttrSet ( node );
    }

    void OnRemoveNodeFromLayer::apply ( MeshPointNode& node )
    {
        updateNodeAttrSet ( node );
    }

    // ******************************************************************

    void OnAddNodeToLayer::doAction ( SGNode* node, LayerNode* layer )
    {
        _attrset  = layer->getAttrSet();
        _fgColor  = _attrset->getFgColor();
        _bgColor  = _attrset->getBgColor();
        _fontnode = _attrset->getFont();

        node->accept ( *this );
    }

    void OnAddNodeToLayer::updateNodeAttrSet ( DrawableNode& node )
    {
        AttrSet* attrset = node.getAttrSet();
        if ( NULL == attrset )
            node.setAttrSet ( _attrset );
        else
        {
            GColor* fgColor    = attrset->getFgColor();
            GColor* bgColor    = attrset->getBgColor();
            FontNode* fontnode = attrset->getFont();
            if ( NULL == fgColor )
                attrset->setFgColor ( _fgColor );
            if ( NULL == bgColor )
                attrset->setBgColor ( _bgColor );
            if ( NULL == fontnode  )
                attrset->setFont ( _fontnode );
        }
    }

    void OnAddNodeToLayer::apply ( RectangleNodef& node )
    {
        updateNodeAttrSet ( node );
    }

    void OnAddNodeToLayer::apply ( TextNode& node )
    {
        updateNodeAttrSet ( node );
    }

    void OnAddNodeToLayer::apply ( LineNodef& node )
    {
        updateNodeAttrSet ( node );
    }

    void OnAddNodeToLayer::apply ( MeshLineNode& node )
    {
        updateNodeAttrSet ( node );
    }

    void OnAddNodeToLayer::apply ( PolylineNode2Df& node )
    {
        updateNodeAttrSet ( node );
    }

    void OnAddNodeToLayer::apply ( PolyNode2Df& node )
    {
        updateNodeAttrSet ( node );
    }

    void OnAddNodeToLayer::apply ( PointNode& node )
    {
        updateNodeAttrSet ( node );
    }

    void OnAddNodeToLayer::apply ( MeshPointNode& node )
    {
        updateNodeAttrSet ( node );
    }
}

