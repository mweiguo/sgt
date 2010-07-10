#ifndef _TRANSFORM_NODE_H_
#define _TRANSFORM_NODE_H_

#include "switchnode.h"
#include "mat4.h"
#include "nodemgr.h"

#include <vector>
#include <sstream>
#include <algorithm>


using namespace std;

class TransformNode : public virtual SwitchNode
{
public:
    TransformNode () { _mat.normalize(); }
    TransformNode ( const TransformNode& rhs ) : SwitchNode(rhs)
    {
        _mat = rhs._mat;
    }
    void setTranslate ( const string& str )
    {
        float floattokens[3];
        fill ( floattokens, floattokens+3, 0.f );

        vector<string> tokens;
        istringstream iss(str);
        copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
        int i=0;
        for ( vector<string>::iterator pp=tokens.begin(); pp!=tokens.end(); ++pp, i++ )
        {
            iss.clear();
            iss.str ( *pp );
            iss >> floattokens[i];
        }
        setTranslate ( floattokens[0], floattokens[1], floattokens[2]);
    }
    void setTranslate ( float dx, float dy, float dz ) { _mat.dx ( dx ); _mat.dy(dy); _mat.dz(dz); }
    void setScale ( const string& str ) 
    {
        float floattokens[3];
        fill ( floattokens, floattokens+3, 0.f );

        vector<string> tokens;
        istringstream iss(str);
        copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));

        int i=0;
        for ( vector<string>::iterator pp=tokens.begin(); pp!=tokens.end(); ++pp, i++ )
        {
            iss.clear ();
            iss.str ( *pp );
            iss >> floattokens[i];
            //             floattokens[i] = atof(pp->c_str() );
        }
        setScale ( floattokens[0], floattokens[1], floattokens[2]);
    }
    void setScale ( float sx, float sy, float sz ) { _mat.sx(sx); _mat.sy(sy); _mat.sz(sz); }
    mat4f& getMatrix () { return _mat; }
    const mat4f& getMatrix () const { return _mat; }
    virtual void updateBBox( const mat4f& mat=mat4f() )
    {
        _bb.init();
        mat4f m = mat * _mat;
        for ( iterator pp=begin(); pp!=end(); ++pp )
        {
            (*pp)->updateBBox(m);
            _bb = _bb.unionbox ( (*pp)->getBBox() );
        }
    }
    virtual void accept ( NodeVisitor& pvisitor ) const { pvisitor.apply ( *this ); }
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~TransformNode () {}
private:
    mat4f _mat;
};

//typedef NodeMgr<TransformNode>  TransformNodeMgr;
#endif
