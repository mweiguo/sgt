#ifndef _LAYOUT_DOCUMENT_H_
#define _LAYOUT_DOCUMENT_H_
#include <document.h>
#include <map>

class SLCPrimitiveNode;
class SLCTextNode;
struct ShapeInfo
{
    SLCPrimitiveNode* node;
    SLCTextNode* cntnode;
    int primitiveCount;
};

class SLCRectNode;
class SLCMaterial;
class LayoutDocument : public Document
{
public:
    LayoutDocument ();
    virtual ~LayoutDocument();
    
    /** viewer interface
     */
    void openLayoutScene ( const char* filename );
    int layoutSceneId;

    /** eidtor interface
     */
    void buildDemoShapes(); // will be replaced by follow two functions
    void clearShapes ();

//     int addShape ( const char* filename );
//     void removeShape ( int shapeid );
    void setShapeCount ( int shapeid, int count );
    void setPlateSize ( float width, float height );
    void saveShapeFile ( const char* filename, const char* slcFileName );
    void savePlateFile ( const char* filename );

    std::map<int, ShapeInfo> shapesInfo;
    SLCRectNode* plate;

    SLCMaterial* objmat;
    SLCMaterial* platemat;
};




#endif// _LAYOUT_DOCUMENT_H_
