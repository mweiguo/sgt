#ifndef _SGR_IMAGE_NODE_H_
#define _SGR_IMAGE_NODE_H_
#include <string>
#include "sgr_drawablenode.h"
#include "sgr_vec2.h"
#include <QPixmap>

namespace SGR
{

class SGR_DLL ImageNode : public DrawableNode
{
public:
    ImageNode ( const char* filePath="" );
    ImageNode ( unsigned int width, unsigned int height );
    ImageNode ( const ImageNode& rhs );
    std::string filePath () const;
    const std::string& filePath ();
    void filePath ( const char* filePath );

    SGR::vec2f size () const;
    const SGR::vec2f& size ();
    void size ( const SGR::vec2f& sz );
    float width () const;
    float height () const;

    int widthInPixel() const;
    int heightInPixel () const;

    void setqImage ( const QPixmap& );
    const QPixmap& qimage() const;
    QPixmap& qimage();
    virtual void computeBBox( const mat4f* mat=0 ) const;
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
private:
    std::string _filePath;
    SGR::vec2f _size; // this size is scene-size, not the size of image
    QPixmap _image;
};

}

#endif // _SGR_IMAGE_NODE_H_
