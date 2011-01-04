#include "sgr_sgnode.h"
#include "sgr_parentfinder.h"
#include "sgr_layerchangger.h"


namespace SGR
{
    SGNode::SGNode() : _parent(0), _userData(0), _isBBoxDirty(true), _isVisible(true) 
    {
    }

    SGNode::SGNode( const SGNode& rhs ) : list<SGNode*>(rhs)
    {
        copy ( rhs.begin(), rhs.end(), back_inserter(*this) );
        _parent = rhs._parent;
        _bb = rhs._bb;
        _isBBoxDirty = rhs._isBBoxDirty;
        _id = rhs._id;
        _userData = rhs._userData;
        _isVisible = rhs._isVisible;
    }
    
    SGNode::~SGNode ()
    {
    }

    SGNode* SGNode::clone ()
    {
        return new SGNode(*this);
    }

    // nodes relation operations
    void SGNode::addChild ( SGNode* pNode )
    { 
        pNode->_setParent ( this );
        _addChild ( pNode );
	
        ParentFinder<LayerNode> finder( this );
	if ( finder.target() )
	{
	    OnAddNodeToLayer onAddNodeToLayer;
	    onAddNodeToLayer.doAction ( pNode, finder.target() );
	}
    }
    
    void SGNode::pushfrontChild ( SGNode* pNode )
    { 
        pNode->_setParent ( this );
        _pushfrontChild ( pNode );
    }
    
    void SGNode::removeChild ( SGNode* pNode ) 
    {
        if ( find ( begin(), end(), pNode ) != end() )
        {
            pNode->_setParent ( NULL );
            _removeChild ( pNode );
        }
    }

    void SGNode::removeAllChild ()
    {
        for ( iterator pp=begin(); pp!=end(); ++pp )
            (*pp)->_setParent(NULL);
        clear();
    }

    void SGNode::setParentNode (SGNode* node)
    {
        if ( node )
        {
            if ( _parent )
                _parent->_removeChild (this);
            node->addChild ( this );
        } 
        else
        {
            if ( _parent )
                _parent->_removeChild (this);
            _setParent ( node );
        }
    }

    // virtual functions
    // this matrix is transform matrix from it's parents
    void SGNode::updateBBox( const mat4f& mat, bool force )
    {
        if ( force || isBBoxDirty()  )
        {
            _bb.setInvalid();
            for ( iterator pp=begin(); pp!=end(); ++pp )
            {
                if ( force || (*pp)->isBBoxDirty () )
                    (*pp)->updateBBox(mat, force);
                _bb = _bb.unionbox ( (*pp)->getBBox() );
            }
            _isBBoxDirty = false;
        }
    }

    void SGNode::computeBBox( const mat4f* mat ) const
    {
        if ( false == _isBBoxDirty )
            return;

        mat4f tmat;
        if ( 0 == mat )
        {
            mat = &tmat;
            tmat = getParentTranMatrix ();
        }

        _bb.setInvalid();
        for ( const_iterator pp=begin(); pp!=end(); ++pp )
        {
            (*pp)->computeBBox ( mat );
            _bb = _bb.unionbox ( (*pp)->_bb );
        }

        _isBBoxDirty = false;
    }

    const BBox& SGNode::getBBox () const
    {
        if ( _isBBoxDirty )
            computeBBox ();

        return _bb;
    }


    void SGNode::setBBoxDirty ()
    {
        _isBBoxDirty = true;

        // all it's parents's bbox are invalid
        SGNode* parent = _parent;
        while ( parent )
        {
            parent->_isBBoxDirty = true;
            parent = parent->_parent;
        }
    }

    //void SGNode::setParentBBoxDirty ( bool isDirty )
    //{
    //    SGNode* parent = this->getParentNode();
    //    if ( parent )
    //    {
    //        parent->setBBoxDirty  ( isDirty );
    //        parent->setParentBBoxDirty ( isDirty );
    //    }
    //}
    //
    void SGNode::setChildrenBBoxDirty ()
    {
        for ( iterator pp=begin(); pp!=end(); ++pp )
        {
            (*pp)->_isBBoxDirty = true;
            (*pp)->setChildrenBBoxDirty ();
        }
    }

    mat4f SGNode::getParentTranMatrix () const
    {
        mat4f tmat;
        ParentFinder<TransformNode> finder( this );
        TransformNode* p = finder.target();
        while ( p )
        {
            if ( p )
                tmat = p->getMatrix() * tmat;
            ParentFinder<TransformNode> finder( p );
            p = finder.target();
        }

        return tmat;
    }

}

