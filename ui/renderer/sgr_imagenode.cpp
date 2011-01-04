#include "sgr_imagenode.h"
#include "sgr_vec4.h"

namespace SGR
{

    ImageNode::ImageNode ( const char* filePath ) : DrawableNode()
    {
	_size.xy( 0, 0 );
	_filePath = filePath;
	_image.load ( filePath );
    }

    ImageNode::ImageNode ( unsigned int width, unsigned int height ) : DrawableNode(), _image(QSize(width,height)/*, QImage::Format_ARGB32*/)
    {
	_size.xy( 0, 0 );
	_filePath = "";
    }

    ImageNode::ImageNode ( const ImageNode& rhs ) : 
	DrawableNode(rhs),
	_filePath(rhs._filePath),
	_size(rhs._size),
	_image(rhs._image)
    {
    }

    std::string ImageNode::filePath () const
    {
	return _filePath;
    }

    const std::string& ImageNode::filePath ()
    {
	return _filePath;
    }

    void ImageNode::filePath ( const char* path )
    {
	_filePath = path;
	_image.load ( _filePath.c_str() );
    }

    SGR::vec2f ImageNode::size () const
    {
	return _size;
    }

    const SGR::vec2f& ImageNode::size ()
    {
	return _size;
    }

    void ImageNode::size ( const SGR::vec2f& sz )
    {
	_size = sz;
    }

    float ImageNode::width () const
    {
	return _size.x();
    }

    float ImageNode::height () const
    {
	return _size.y();
    }

    int ImageNode::widthInPixel() const
    {
	return _image.width();
    }

    int ImageNode::heightInPixel () const
    {
	return _image.height();
    }

    void ImageNode::setqImage ( const QPixmap& img )
    {
	_image = img;
    }

    const QPixmap& ImageNode::qimage() const
    {
	return _image;
    }

    QPixmap& ImageNode::qimage()
    {
	return _image;
    }

    void ImageNode::computeBBox( const mat4f* mat ) const
    {
	if ( false == _isBBoxDirty )
	    return;

	mat4f tmat;
	if ( 0 == mat )
	{
	    mat = &tmat;
	    tmat = getParentTranMatrix ();
	}
	DrawableNode::computeBBox ( mat );

	BBox bb ( vec3f(0,0,0), vec3f(_size.x(), _size.y(), 0) );

	vec4f min = (*mat) * vec4f ( bb.minvec() );
	vec4f max = (*mat) * vec4f ( bb.maxvec() );
	bb.setminmax ( min.xyz(), max.xyz() );

	_bb = _bb.unionbox ( bb );
    }

}
