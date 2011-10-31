#ifndef _LAYOUT_DOCUMENT_H_
#define _LAYOUT_DOCUMENT_H_
#include <document.h>
#include <map>
#include <string>

class SLCPrimitiveNode;
class SLCTextNode;
class SLCMaterial;

struct PlateInfo
{
    PlateInfo()
    {
        userateNode = NULL;
        sizeNode = NULL;
        marginNode = NULL;
        colorNode = NULL;
        userateValueNode = NULL;
        sizeValueNode = NULL;
        marginValueNode = NULL;
        colorValueNode = NULL;
    }
    SLCTextNode* userateNode;
    SLCTextNode* sizeNode;
    SLCTextNode* marginNode;
    SLCTextNode* colorNode;

    SLCTextNode* userateValueNode;
    SLCTextNode* sizeValueNode;
    SLCTextNode* marginValueNode;
    SLCPrimitiveNode* colorValueNode;
};

struct ShapeInfo
{
    ShapeInfo()
    {
        node = 0;
        cntnode = 0;
        sizenode = 0;
        colornode = 0;
//        primitiveCount = 0;
    }
    SLCPrimitiveNode* node;
    SLCTextNode* cntnode;
    SLCTextNode* sizenode;
    SLCPrimitiveNode* colornode;
//    int primitiveCount;
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
    void openLayoutScene ( GLWidget* widget, const char* filename );
    void closeLayoutScene ();
    int layoutSceneId;

    /** eidtor interface
     */
    void buildDemoShapes(); // will be replaced by follow two functions
    void clearShapes ();

    //     int addShape ( const char* filename );
    //     void removeShape ( int shapeid );
    void setShapeCount ( int shapeid, int count );
    void setPlateSize ( float width, float height );
    void saveShapeXMLFile ( const char* filename );
    void saveShapeSLCFile ( const char* filename );
    void savePlateFile ( const char* filename );

    std::map<std::string,SLCMaterial*> materials;
    std::map<int, ShapeInfo> shapesInfo;
    PlateInfo plateInfo;
    SLCRectNode* plate;

    SLCMaterial* objmat;
    SLCMaterial* objmat1;
    SLCMaterial* objmat2;
    SLCMaterial* objmat3;
    SLCMaterial* objmat4;
    SLCMaterial* platemat;
};




#endif// _LAYOUT_DOCUMENT_H_
