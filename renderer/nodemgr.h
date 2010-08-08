#ifndef _NODEMGR_H_
#define _NODEMGR_H_

#include <map>
#include <vector>
#include "sgnode.h"
using namespace std;

class AGEF_EXPORT SeedGenerator
{
public:
    static SeedGenerator& getInst ()
    {
        static SeedGenerator inst;
        return inst;
    }
    int seed() { return _seed++; }
private:
    SeedGenerator()
    {
        _seed = 0;
    }
    int _seed;

};

// id ==> SGNode    :   nodemgr[id]
// SGNode ==> id    :   nodemgr - begin()
class AGEF_EXPORT NodeMgr : public std::map<int, SGNode*>
{
public:
    static NodeMgr& getInst ()
    {
        static NodeMgr* inst = new NodeMgr();
        return *inst;
    }

    template < class T >
    int addNode ()
    {
        int seed = SeedGenerator::getInst().seed();
        (*this)[seed] = _lastNode = new T();
        _lastNode->setID ( seed );
        return seed;
    }

    template < class T, class T1 >
    int addNode ( T1 t1 )
    {
        int seed = SeedGenerator::getInst().seed();
        (*this)[seed] = _lastNode = new T(t1);
        _lastNode->setID ( seed );
        return seed;
    }

    template < class T >
    int addNode ( T node )
    {
        int seed = SeedGenerator::getInst().seed();
        (*this)[seed] = node;
        node->setID ( seed );
        return seed;
    }

    template < class T >
    T* getNodePtr ( int id )
    {
        iterator pp=find ( id );
        if ( pp == end() )
            return NULL;
        T* p = dynamic_cast<T*>(pp->second);
        return p;
    }
    
    void deleteNode ( int id )
    {
	iterator pp = find ( id );
	if ( pp != end() )
	{
	    SGNode* node = pp->second;
	    node->removeAllChild ();
	    node->setParentNode ( NULL );
	    delete node;
	    NodeMgr::getInst().erase ( id );
	}
    }

    // this id is for respond to interface.h 's id
    int getID ( SGNode* p )
    {
        return p->getID();
    }

    SGNode* root() { return _root; }
private:
    NodeMgr () 
    {
        addNode<SGNode>();
        _root = (*this)[0];
    }
    SGNode* _lastNode, *_root;
};

#endif
