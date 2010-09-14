#ifndef _SGNODE_H_
#define _SGNODE_H_

#include <list>
#include <iterator>
#include <algorithm>
#include "sgr_mat4.h"

//#include "sgr_quickvector.h"
#include "sgr_nodevisitor.h"
using namespace std;

#include "sgr_bbox.h"
//class NodeVisitor;
//class DrawableNode;

namespace SGR
{

class SGR_DLL SGNode : public list<SGNode*>
{
public:
    SGNode() : _parent(0), _isBBoxDirty(true), _userData(0), _isVisible(true) {}
    SGNode( const SGNode& rhs )
    {
        copy ( rhs.begin(), rhs.end(), back_inserter(*this) );
        _parent = rhs._parent;
        _bb = rhs._bb;
        _isBBoxDirty = rhs._isBBoxDirty;
        _id = rhs._id;
        _userData = rhs._userData;
        _isVisible = rhs._isVisible;
    }
    // nodes relation operations
    virtual void addChild ( SGNode* pNode )
    { 
        pNode->_setParent ( this );
        _addChild ( pNode );
    }
    virtual void removeChild ( SGNode* pNode ) 
    {
        if ( find ( begin(), end(), pNode ) != end() )
        {
            pNode->_setParent ( NULL );
            _removeChild ( pNode );
        }
    }
    virtual void removeAllChild ()
    {
        for ( iterator pp=begin(); pp!=end(); ++pp )
            (*pp)->_setParent(NULL);
        clear();
    }
    SGNode* getParentNode () { return _parent; }
    virtual void setParentNode (SGNode* node)
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
    virtual void updateBBox( const mat4f& mat=mat4f() )
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
    virtual void accept ( NodeVisitor& pvisitor ) const { pvisitor.apply ( *this ); }
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~SGNode () {}

    // user data
    void setUserData ( void* data ) { _userData = data; }
    void* getUserData () { return _userData; }
    // id
    void setID ( int id ) { _id = id; }
    int getID () { return _id; }
    // bounding box
    const BBox& getBBox () const         { return _bb; }
    void setBBox ( const BBox& bbox )    { _bb = bbox; }
    bool isBBoxDirty () const            { return _isBBoxDirty; }
    void setBBoxDirty ( bool isDirty )   { _isBBoxDirty = isDirty; }
    // visible
    bool isVisible() { return _isVisible; }
    void setVisible(bool isVisible) { _isVisible = isVisible; }
private:
    void _removeChild ( SGNode* p ) { remove (p); }
    void _addChild ( SGNode* p ) { push_back (p); }
    void _setParent ( SGNode* p ) { _parent=p; }
protected:
    SGNode* _parent;
    void* _userData;
    int _id;
    BBox _bb;
    bool _isBBoxDirty;
    bool _isVisible;
    //list<SGNode*> _children;
};
}

#endif
