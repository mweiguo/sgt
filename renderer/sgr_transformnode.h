#ifndef _TRANSFORM_NODE_H_
#define _TRANSFORM_NODE_H_

#include "sgr_sgnode.h"
#include "sgr_mat4.h"
#include "sgr_nodemgr.h"

#include <vector>
#include <sstream>
#include <algorithm>


using namespace std;
namespace SGR
{

class TransformNode : public SGNode
{
public:
    TransformNode () { _mat.normalize(); }
    TransformNode ( const TransformNode& rhs ) : SGNode(rhs)
    {
        _mat = rhs._mat;
    }
    virtual SGNode* clone ()
    {
        return new TransformNode(*this);
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
        setBBoxDirty ();
        //setParentBBoxDirty ( true );
        setChildrenBBoxDirty ();
    }
    void setTranslate ( float dx, float dy, float dz ) 
    {
        _mat.dx ( dx );
        _mat.dy ( dy );
        _mat.dz ( dz ); 

        setBBoxDirty ();
        //setParentBBoxDirty ( true );
        setChildrenBBoxDirty ();
    }
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

        setBBoxDirty ();
        //setParentBBoxDirty ( true );
        setChildrenBBoxDirty ();
    }
    void setScale ( float sx, float sy, float sz )
    {
        _mat.sx(sx);
        _mat.sy(sy);
        _mat.sz(sz); 

        setBBoxDirty ();
        //setParentBBoxDirty ( true );
        setChildrenBBoxDirty ();
    }
    mat4f& getMatrix () { return _mat; }
    const mat4f& getMatrix () const { return _mat; }
    void setMatrix ( const mat4f& rhs )
    {
        if ( _mat == rhs )
            return;

        _mat = rhs; 
        setBBoxDirty ();
        //setParentBBoxDirty ( true );
        setChildrenBBoxDirty ();

    }
    virtual void updateBBox( const mat4f& mat=mat4f(), bool force=false )
    {
        if ( force || isBBoxDirty()  )
        {
            _bb.setInvalid();
            mat4f m = mat * _mat;
            for ( iterator pp=begin(); pp!=end(); ++pp )
            {
                if ( force || (*pp)->isBBoxDirty () )
                    (*pp)->updateBBox(m, force);
                _bb = _bb.unionbox ( (*pp)->getBBox() );
            }

            _isBBoxDirty = false;
        }
    }
    
    virtual void computeBBox( const mat4f* mat=0 ) const
    {
        if ( false == _isBBoxDirty )
            return;

        mat4f tmat;
        if ( 0 == mat )
        {
            mat = &tmat;
            tmat = getParentTranMatrix ();
        }
        mat4f m = (*mat) * _mat;

        SGNode::computeBBox ( &m );
    }

    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~TransformNode () {}
private:
    mat4f _mat;
};

}
//typedef NodeMgr<TransformNode>  TransformNodeMgr;
#endif
