#ifndef _LOD_NODE_H_
#define _LOD_NODE_H_

#include "sgr_sgnode.h"
#include "sgr_kdtree.h"
//#include "sgr_rendernodecollector.h"
#include <vector>
#include <time.h>
#include <algorithm>

using namespace std;
namespace SGR
{

class SGR_DLL LODNode : public virtual SGNode
{
public:
    typedef vector<float>::iterator diterator;
    typedef vector<float>::const_iterator const_diterator;
    LODNode () {}
    LODNode ( const LODNode& rhs ) : SGNode(rhs)
    {
	copy ( levelDelimiters.begin(), levelDelimiters.end(), back_inserter(levelDelimiters));
    }
    // in proj mode, v is dist
    // in ortho mode, v is scale
    SGNode* selectPresentation ( float v ) 
    {
        if ( levelDelimiters.size() == 0 && size() !=0 )
            return front();

        // validation check
        if ( (levelDelimiters.size()+1) != size() )
            return NULL;

        typedef vector<float> DefLevel;
        DefLevel::iterator pp = upper_bound ( levelDelimiters.begin(), levelDelimiters.end(), v );
        if ( pp != levelDelimiters.end() )
        {
            int cnt = pp - levelDelimiters.begin();
            iterator pp1=begin();
            while ( cnt--!=0 ) ++pp1;
            SGNode* sgnode = dynamic_cast<SGNode*>(*pp1);
            return sgnode;
        }
        else
            return back();
    }
    void setdelimiters ( const string& str ) 
    {
        vector<string> tokens;
        vector<float> floattokens;
        istringstream iss(str);
        copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
        for ( vector<string>::iterator pp=tokens.begin(); pp!=tokens.end(); ++pp )
        {
            float t;
            iss.clear ();
            iss.str ( *pp );
            iss >> t;
            floattokens.push_back ( t );
        }
        levelDelimiters.assign ( floattokens.begin(), floattokens.end() );
    }
    template < class Input >
    void assigndelimiters ( Input begin, Input end ) { levelDelimiters.assign ( begin, end); }
    //void buildsom ();
    
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~LODNode () {}
    diterator dbegin() { return levelDelimiters.begin(); }
    diterator dend() { return levelDelimiters.end(); }
    const_diterator dbegin() const { return levelDelimiters.begin(); }
    const_diterator dend() const { return levelDelimiters.begin(); }
private:
    // in proj mode, levelDelimiters is dist array
    // in ortho mode, levelDelimiters is scale array
    vector<float> levelDelimiters;
    //vector<GroupNode*> lodModels;
};

// build spatial objects management
//inline void LODNode::buildsom ()
//{
//    if ( lodModels.size() != (levelDelimiters.size()+1) ) {
//        lodModels.reserve ( levelDelimiters.size()+1 );
//        for ( size_t i=0; i<=levelDelimiters.size(); i++ )
//            lodModels.push_back ( new SpatialObjectMgr() );
//    }
//
//    int i =0;
//    for ( iterator pp=this->begin(); pp!=end(); ++pp, i++ )
//    {
//        lodModels[i]->reset();
//#ifdef _USESTATISTIC_
//        int clo = clock();
//#endif  
//        RenderNodeCollector< back_insert_iterator<SpatialObjectMgr> > collector( back_inserter(*lodModels[i]) );
//        (*pp)->accept ( collector );
//#ifdef _USESTATISTIC_
//        qDebug ( "collect nodes TAKE %d clock, %f (s)", clock() - clo,  (1.0*(clock() - clo))/CLOCKS_PER_SEC );
//        clo = clock();
//#endif          
//        BuildSpatialObjectMgr build ( *lodModels[i]/*, collector.begin(), collector.end()*/ );
//#ifdef _USESTATISTIC_
//        qDebug ( "%s", build.statistic().c_str() );
//        qDebug ( "%s", lodModels[i]->memstatistic().c_str() );
//#endif          
//    }
//}
//
//typedef NodeMgr<LODNode>        LODNodeMgr;

}
#endif
