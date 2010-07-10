#ifndef _ARRAY_EXPANDER_H_
#define _ARRAY_EXPANDER_H_

#include "nodevisitor.h"
#include <vector>
using namespace std;
#include "agef_global.h"

class AGEF_EXPORT ArrayExpander : public NodeVisitor
{
public:
    ArrayExpander ( SGNode* parent, bool isExport=false );
    virtual void apply ( SGNode& /*node*/ );
    virtual void apply ( LayerNode& /*node*/ );
    virtual void apply ( RectangleNodef& /*node*/ );
    virtual void apply ( TransformNode& /*node*/ );
    virtual void apply ( ArrayNode& /*node*/ );
    virtual void apply ( LODNode& /*node*/ );
    virtual void apply ( PickableGroup& /*node*/ );
    virtual void apply ( KdTreeNode& /*node*/ );
    virtual void apply ( TextNode& /*node*/ );
    virtual void apply ( LineNodef& /*node*/ );
    void operator () ( ArrayNode& node );

    vector<KdTreeNode*>::iterator kdbegin() { return _kdtreenodes.begin(); }
    vector<KdTreeNode*>::iterator kdend() { return _kdtreenodes.end(); }
private:
    SGNode* _parent, * _curparent;
    vector<KdTreeNode*> _kdtreenodes;
    bool _isExport; // this parameter decide whethear raid items exported to UserModule
};

#endif // _ARRAY_EXPANDER_H_

