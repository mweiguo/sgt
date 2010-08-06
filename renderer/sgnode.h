#ifndef _SGNODE_H_
#define _SGNODE_H_

#include <list>
#include <iterator>
#include <algorithm>
#include <mat4.h>
//#include "quickvector.h"
#include "nodevisitor.h"
using namespace std;

#include <bbox.h>
//class NodeVisitor;
//class DrawableNode;
class AGEF_EXPORT SGNode : public list<SGNode*>
{
public:
    SGNode() : _parent(0), _isBBoxDirty(true), _userData(0) {}
    SGNode( const SGNode& rhs )
    {
        copy ( rhs.begin(), rhs.end(), back_inserter(*this) );
        _parent = rhs._parent;
        _bb = rhs._bb;
        _isBBoxDirty = rhs._isBBoxDirty;
        _id = rhs._id;
        _userData = rhs._userData;
    }
    // nodes relation operations
    void addChild ( SGNode* pNode )
    { 
        pNode->_setParent ( this );
        _addChild ( pNode );
    }
    void removeChild ( SGNode* pNode ) 
    {
        if ( find ( begin(), end(), pNode ) != end() )
        {
            pNode->_setParent ( NULL );
            _removeChild ( pNode );
        }
    }
    void removeAllChild ()
    {
        for ( iterator pp=begin(); pp!=end(); ++pp )
            (*pp)->_setParent(NULL);
        clear();
    }
    SGNode* getParentNode () { return _parent; }
    void setParentNode (SGNode* node)
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
        _bb.init();
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
    bool isBBoxDirty () const            { return _isBBoxDirty; }
    void setBBoxDirty ( bool isDirty )   { _isBBoxDirty = isDirty; }
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
    //list<SGNode*> _children;
};

#endif
