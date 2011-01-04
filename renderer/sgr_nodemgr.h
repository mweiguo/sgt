#ifndef _NODEMGR_H_
#define _NODEMGR_H_

#include <map>
#include <vector>
#include "sgr_sgnode.h"
using namespace std;

namespace SGR
{

//class SGR_DLL SeedGenerator
//{
//public:
//    static SeedGenerator& getInst ()
//    {
//        static SeedGenerator inst;
//        return inst;
//    }
//    int minseed() { return _minseed--; }
//    int maxseed() { return _maxseed++; }
//    void minseed ( int seed ) { _minseed = seed; }
//    void maxseed ( int seed ) { _maxseed = seed; }
//private:
//    SeedGenerator()
//    {
//        minseed ( -1 );
//        maxseed ( 1 );
//    }
//    int _minseed;
//    int _maxseed;
//
//};

// id ==> SGNode    :   nodemgr[id]
// SGNode ==> id    :   nodemgr - begin()
class SGR_DLL NodeMgr : public std::map<int, SGNode*>
{
public:
    static NodeMgr& getInst ();

    template < class T >
    T* addNode ( int id )
    {
        iterator pp=find ( id );
        if ( pp != end() )
            return NULL;

        updateMinMaxId ( id );

        T* t = new T();
        t->setID ( id );
        (*this)[id] = t;
        return t;
        //int seed = SeedGenerator::getInst().seed();
        //(*this)[seed] = _lastNode = new T();
        //_lastNode->setID ( seed );
        //return seed;
    }

    template < class T, class T1 >
    T* addNode ( int id, T1 t1 )
    {
        iterator pp=find ( id );
        if ( pp != end() )
            return NULL;

        updateMinMaxId ( id );

        T* t = new T ( t1 );
        t->setID ( id );
        (*this)[id] = t;
        return t;
        //int seed = SeedGenerator::getInst().seed();
        //(*this)[seed] = _lastNode = new T(t1);
        //_lastNode->setID ( seed );
        //return seed;
    }

    //template < class T >
    //int addNode ( T node )
    //{
    //    int seed = SeedGenerator::getInst().seed();
    //    (*this)[seed] = node;
    //    node->setID ( seed );
    //    return seed;
    //}

    template < class T >
    T* getNodePtr ( int id )
    {
        iterator pp=find ( id );
        if ( pp == end() )
            return NULL;
        T* p = dynamic_cast<T*>(pp->second);
        return p;
    }
    
    void deleteNode ( int id );
    void deleteNode ( SGNode* node );
    void clearAll ();

    // this id is for respond to interface.h 's id
    int getID ( SGNode* p );

    SGNode* root();
    NodeMgr ();
private:
    void updateMinMaxId ( int id );

    SGNode* _lastNode, *_root;
    int _minId, _maxId;
};

}
#endif
