#ifndef _SAVEPICKABLE_H_
#define _SAVEPICKABLE_H_

#include "sgr_childvisitor.h"
#include "sgr_parentfinder.h"
#include <sstream>
#include <fstream>
using namespace std;
namespace SGR
{

class SGR_DLL SavePickable : public ChildVisitor
{
public:
    SavePickable( const string& filename, SGNode* node );
    virtual void apply ( RectangleNodef& node );
    virtual void apply ( TransformNode& node );
    virtual void apply ( ArrayNode& node );
    virtual void apply ( PickableGroup& node );
private:
    stringstream _xmlContent;
    mat4f _curmat;
    int _cidx0, _ridx0, _cidx1, _ridx1, _columns/*second array node's column*/;
    PickableGroup* _curpick;
};

inline SavePickable::SavePickable( const string& filename, SGNode* node ) : 
    _cidx0(0), _ridx0(0), _cidx1(0), _ridx1(0), _curpick(NULL)
{
    _xmlContent.str("");
    node->accept ( *this );
    ofstream out;
    out.open ( filename.c_str() );
    out << _xmlContent.str();
    out.close();
}

inline void SavePickable::apply ( RectangleNodef& node )
{
    if ( _curpick )
    {
        vec2f pos = (_curmat * vec4f (0,0,0,1)).xy();
        _xmlContent << pos.x() << ' ' << pos.y() << ' ' << node.w() << ' ' << node.h() << ' ' << _ridx0 
            << ' ' << _cidx0 << ' ' << _curpick->name() << ' ' << _ridx1 * _columns + _cidx1 << endl;
    }

    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );
}

inline void SavePickable::apply ( TransformNode& node )
{
    const mat4f& m = node.getMatrix(), oldmat = _curmat;
    _curmat = m * _curmat;

    for ( SGNode::iterator pp=node.begin(); pp!=node.end(); ++pp )
        (*pp)->accept ( *this );

    _curmat = oldmat;
}

inline void SavePickable::apply ( ArrayNode& node )
{
    // if node is topmost arraynode, use ridx0, cidx0;
    // else,, use ridx1, cidx1;
    ParentFinder<ArrayNode> finder;
    ArrayNode* parent = finder ( &node );
    int *ridx, *cidx, oldridx, oldcidx, oldcolumns;
    if ( parent ) {
        ridx = &_ridx1;
        oldridx = *ridx;
        cidx = &_cidx1;
        oldcidx = *cidx;
        _columns = node.getColumnCnt();
        oldcolumns = _columns;
    } else {
        ridx = &_ridx0;
        cidx = &_cidx0;
        oldridx = *ridx;
        oldcidx = *cidx;
    }

    // check it's matrix
    for ( int j=0; j<node.rowCnt(); j++ ) {
        float y = node.getVOffset ( j );
        *ridx = j;
        for ( int k=0; k<node.columnCnt(); k++ ) {
            float x = node.getHOffset ( k );
            *cidx = k;
            // generate new matrix & use this matrix
            mat4f m = mat4f::translate_matrix ( x, y, 0 ), oldmat = _curmat;
            _curmat = m * _curmat;
            // set matrix
            ChildVisitor::apply ( node );
            _curmat = oldmat;
        }
    }

    *ridx = oldridx;
    *cidx = oldcidx;
    _columns = oldcolumns;
}

inline void SavePickable::apply ( PickableGroup& node )
{
    PickableGroup* oldpick = _curpick;

    _curpick = &node;
    ChildVisitor::apply ( node );
    _curpick = oldpick;
}

}

#endif

