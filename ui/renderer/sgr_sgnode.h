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
    SGNode();
    SGNode( const SGNode& rhs );
    virtual ~SGNode ();

    virtual SGNode* clone (); 
    /** nodes relation operations
     */
    virtual void addChild ( SGNode* pNode );
    virtual void pushfrontChild ( SGNode* pNode );
    virtual void removeChild ( SGNode* pNode );
    virtual void removeAllChild ();
    const SGNode* getParentNode () const { return _parent; }
    SGNode* getParentNode () { return _parent; }
    virtual void setParentNode (SGNode* node);
    /** virtual functions
     * this function is obslete, use computeBBox instead
     * this matrix is transform matrix from it's parents
     */
    virtual void updateBBox( const mat4f& mat=mat4f(), bool force=false );
    /** if mat == 0, mat should be computed inside function.
     */
    virtual void computeBBox( const mat4f* mat=0 ) const;
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    /** user data
     */
    void setUserData ( void* data ) { _userData = data; }
    void* getUserData () { return _userData; }
    /** id
     */
    void setID ( int id ) { _id = id; }
    int getID () { return _id; }
    /** bounding box
     */
    const BBox& getBBox () const;
    void setBBox ( const BBox& bbox )    { _bb = bbox; _isBBoxDirty=false; }
    bool isBBoxDirty () const            { return _isBBoxDirty; }
    virtual void setBBoxDirty ();
    //virtual void setParentBBoxDirty ( bool isDirty );
    virtual void setChildrenBBoxDirty ();

    /** visible
     */
    bool isVisible() { return _isVisible; }
    void setVisible(bool isVisible) { _isVisible = isVisible; }
protected:
    mat4f getParentTranMatrix () const;
private:
    void _removeChild ( SGNode* p ) { remove (p); }
    void _addChild ( SGNode* p ) { push_back (p); }
    void _pushfrontChild ( SGNode* p ) { push_front (p); }
    void _setParent ( SGNode* p ) { _parent=p; }
protected:
    SGNode* _parent;
    void* _userData;
    int _id;
    mutable BBox _bb;
    mutable bool _isBBoxDirty;
    bool _isVisible;
    //list<SGNode*> _children;
};
}

#endif
