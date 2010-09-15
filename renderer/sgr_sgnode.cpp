#include "sgr_sgnode.h"

namespace SGR
{
    SGNode::SGNode() : _parent(0), _isBBoxDirty(true), _userData(0), _isVisible(true) 
    {
    }

    SGNode::SGNode( const SGNode& rhs )
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

    // nodes relation operations
    void SGNode::addChild ( SGNode* pNode )
    { 
        pNode->_setParent ( this );
        _addChild ( pNode );
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
    void SGNode::updateBBox( const mat4f& mat )
    {
        _bb.setInvalid();
        for ( iterator pp=begin(); pp!=end(); ++pp )
        {
            if ( (*pp)->isBBoxDirty () )
                (*pp)->updateBBox(mat);
            _bb = _bb.unionbox ( (*pp)->getBBox() );
        }
        setBBoxDirty ( false );
    }

    void SGNode::setBBoxDirty ( bool isDirty )
    {
        _isBBoxDirty = isDirty; 
    }

    void SGNode::setParentBBoxDirty ( bool isDirty )
    {
        SGNode* parent = this->getParentNode();
        if ( parent )
        {
            parent->setBBoxDirty  ( isDirty );
            parent->setParentBBoxDirty ( isDirty );
        }
    }
    
    void SGNode::setChildrenBBoxDirty ( bool isDirty )
    {
        for ( iterator pp=begin(); pp!=end(); ++pp )
        {
            (*pp)->setBBoxDirty ( isDirty );
            (*pp)->setChildrenBBoxDirty ( isDirty );
        }
    }
}

