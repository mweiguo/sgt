#include "sgr_imposter.h"
#include "sgr_rendercontext.h"
#include <stdexcept>
#include <QPainter>
#include <QPixmap>
#include "sgr_renderfunctor.h"
#include "sgr_interface.h"
#include "sgr_coordflater.h"

namespace SGR
{
    ImposterNode::ImposterNode ( const char* delis ) : DrawableNode()
    {
	setLodDelimiters ( delis );
    }

    ImposterNode::ImposterNode ( const ImposterNode& rhs ) : 
	DrawableNode(rhs),
	_levelDelimiters(rhs._levelDelimiters),
	_imposters(rhs._imposters)
    {
    }

    ImposterNode::~ImposterNode()
    {
    }

    void ImposterNode::setLodDelimiters ( const char* delis )
    {
	string str = delis;
	if ( "" != str )
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
	    _levelDelimiters.assign ( floattokens.begin(), floattokens.end() );
	}

	// create correspond image node
	for ( unsigned int i=0; i<_levelDelimiters.size(); ++i )
	{
	    IMPOSTERS_ITERATOR pp = _imposters.find ( i );
	    if ( pp == _imposters.end() )
	    {
		_imposters[i] = pair<bool,ImageNode>(true, ImageNode(1024,768));
	    }
	}
    }

    float ImposterNode::getLodDelimiter ( unsigned int idx )
    {
	return _levelDelimiters.at(idx);
    }

    unsigned int ImposterNode::getLodDelimiterCount ()
    {
	return _levelDelimiters.size();
    }

    void ImposterNode::setImageDirty ( unsigned int idx, bool dirty )
    {
	IMPOSTERS_ITERATOR pp = _imposters.find ( idx );
	if ( pp == _imposters.end() )
	{
	    stringstream ss;
	    ss << "ImposterNode::setImageDirty: idx(" << idx << ") is invalid";
	    throw std::invalid_argument ( ss.str() );
	}
	pp->second.first = dirty;
    }

    bool ImposterNode::isImageDirty ( unsigned int idx )
    {
	IMPOSTERS_ITERATOR pp = _imposters.find ( idx );
	if ( pp == _imposters.end() )
	{
	    stringstream ss;
	    ss << "ImposterNode::setImageDirty: idx(" << idx << ") is invalid";
	    throw invalid_argument ( ss.str() );
	}
	return pp->second.first;
    }

    void ImposterNode::updateImage ( unsigned int idx, RenderOption& opt )
    {
	IMPOSTERS_ITERATOR pp = _imposters.find ( idx );
	if ( pp != _imposters.end() )
	{
	    QMatrix mat = opt.painter->worldMatrix();

	    // calculate image's size in pixel
	    vec4f imageSize = opt.matrix * vec4f(_bb.dimension());
	    qDebug ( "ImposterNode::updateImage idx = %d, imageSize(%f, %f)", idx, imageSize.x(), imageSize.y() );

	    opt.painter->end ();
	    QPixmap outImage ( QSize(512, 512)/*, QImage::Format_ARGB32*/ );
	    outImage.fill ( QColor(0,0,0,0) );
	    opt.painter->setRedirected ( opt.orgpaintdevice, &outImage );
	    opt.painter->begin ( &outImage );

	    // calculate the scale, in order to full fill content in 512 x 512 pixels
	    float maxDimension = _bb.dimension().x() > _bb.dimension().y() ? _bb.dimension().x() : _bb.dimension().y();
	    

	    QMatrix toOrigin;
	    toOrigin = toOrigin.scale ( 512 / maxDimension, 512 / maxDimension );
	    opt.painter->setWorldMatrix ( toOrigin );

	    QtRenderVisitor rv ( const_cast<RenderOption*>(&opt) );
	    /** here, we should render all children without clipped, because 
	     * this image may be used in more small scale, we should show some 
	     * hiden parts in this image.
	     * 1. collect all children
	     * 2. Render
	     */
	    CoordFlater flater;
	    flater.doAction ( opt.camera->getID(), this );
	    
	    AttrSet* lastAttrset = NULL;
	    typedef list<DrawableNode*>::iterator DrawableListIterator;
	    
	    for ( DrawableListIterator pp1=flater.result.begin(); pp1!=flater.result.end(); ++pp1 )
	    {
		if ( (*pp1)->getAttrSet() != lastAttrset )
		{
		    // switch all state in AttrSet
		    QtStateChanger changeState ( &opt, (*pp1)->getAttrSet() );
		    lastAttrset = (*pp1)->getAttrSet();
		}
		(*pp1)->accept ( rv );
	    }

	    opt.painter->end ();
	    opt.painter->restoreRedirected ( opt.orgpaintdevice );

	    opt.painter->begin (opt.orgpaintdevice);
	    opt.painter->setWorldMatrix ( mat );

	    pp->second.second.setqImage ( outImage );
	    pp->second.second.setBBox ( _bb );
	    pp->second.first = false;
//	    outImage.save ( "out.png" );
	}
    }

    const ImageNode& ImposterNode::getImage ( unsigned int idx ) const
    {
	map<unsigned int, pair<bool,ImageNode> >::const_iterator pp = _imposters.find ( idx );
	if ( pp == _imposters.end() )
	{
	    stringstream ss;
	    ss << "ImposterNode::setImageDirty: idx(" << idx << ") is invalid";
	    throw invalid_argument ( ss.str() );
	}
	return pp->second.second;
    }

    int ImposterNode::getLodDelimiterIndex ( float v ) const
    {
	if ( _levelDelimiters.empty() ||
//	     v < _levelDelimiters.front() ||
	     v >= _levelDelimiters.back() )
	    return -1;

	vector<float>::const_iterator pp=_levelDelimiters.begin(), pp1=_levelDelimiters.begin();
	++pp1;
	int idx = 0;

	while ( (pp1 != _levelDelimiters.end()) && (v<*pp || v>=*pp1) )
	{
	    ++pp;
	    ++pp1;
	    ++idx;
	}

	return idx;
    }
}
