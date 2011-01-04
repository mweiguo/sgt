#ifndef _SGR_LAYERCHANGGER_H_
#define _SGR_LAYERCHANGGER_H_

#include "sgr_childvisitor.h"

namespace SGR
{
    class GColor;
    class AttrSet;
    class FontNode;
    // these two class will update child nodes' attrset.
    class OnRemoveNodeFromLayer : public ChildVisitor
    {
    public:
        void doAction ( LayerNode* layer, SGNode* child );

        virtual void apply ( RectangleNodef& node );
        virtual void apply ( TextNode& node );
        virtual void apply ( LineNodef& node );
        virtual void apply ( MeshLineNode& node );
        virtual void apply ( PolylineNode2Df& node );
        virtual void apply ( PolyNode2Df& node );
        virtual void apply ( PointNode& node );
        virtual void apply ( MeshPointNode& node );
        virtual void apply ( CircleNode& node );
        virtual void apply ( ImageNode& node );
        virtual void apply ( ImposterNode& node );
    private:
        void updateNodeAttrSet ( DrawableNode& node );
    private:
        AttrSet* _attrset;
        GColor* _fgColor;
        GColor* _bgColor;
        FontNode* _fontnode;
    };

    class OnAddNodeToLayer : public ChildVisitor
    {
    public:
        virtual void apply ( RectangleNodef& node );
        virtual void apply ( TextNode& node );
        virtual void apply ( LineNodef& node );
        virtual void apply ( MeshLineNode& node );
        virtual void apply ( PolylineNode2Df& node );
        virtual void apply ( PolyNode2Df& node );
        virtual void apply ( PointNode& node );
        virtual void apply ( MeshPointNode& node );
        virtual void apply ( CircleNode& node );
        virtual void apply ( ImageNode& node );
        virtual void apply ( ImposterNode& node );
        
        void doAction ( SGNode* child, LayerNode* layer );
    private:
        void updateNodeAttrSet ( DrawableNode& node );
    private:
        AttrSet* _attrset;
        GColor* _fgColor;
        GColor* _bgColor;
        FontNode* _fontnode;
    };

}

#endif //_SGR_LAYERCHANGGER_H_
