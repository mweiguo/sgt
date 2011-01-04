#include "sgr_nodemgr.h"
#include "sgr_seedgen.h"

using namespace std;
#include <QMessageBox>
#include <QString>

namespace SGR
{

NodeMgr inst;

NodeMgr& NodeMgr::getInst ()
{
    return inst;
}

void NodeMgr::deleteNode ( int id )
{
    if ( empty() )
        return;
    iterator pp = find ( id );
    if ( pp != end() )
    {
        SGNode* node = pp->second;
        node->removeAllChild ();
        node->setParentNode ( NULL );
        erase ( id );
        delete node;
    }
}

void NodeMgr::deleteNode ( SGNode* node )
{
    node->removeAllChild ();
    node->setParentNode ( NULL );
    erase ( node->getID() );
    delete node;
}

void NodeMgr::clearAll ()
{
    //for ( map<int, SGNode*>::iterator pp=begin(); pp!=end(); ++pp )
    //{
    //    if ( pp->first != 0 )
    //        delete pp->second;
    //}
    //SGNode* pp = (*this)[0];
    //clear();

    //(*this)[0] = pp;
    //SeedGenerator::getInst().minseed (-1);
    //SeedGenerator::getInst().maxseed (1);

}

// this id is for respond to interface.h 's id
int NodeMgr::getID ( SGNode* p )
{
    return p->getID();
}

SGNode* NodeMgr::root() { return _root; }


NodeMgr::NodeMgr () 
{
    _root = addNode<SGNode>(0);
    //QMessageBox::information (0, QString("message box"), QString("message box") );
}

void NodeMgr::updateMinMaxId ( int id )
{
    if ( size () < 1 )
    {
        _minId = _maxId = id;
    }
    else
    {
        _minId = _minId < id ? _minId : id;
        _maxId = _minId < id ? _minId : id;
    }
}


}

