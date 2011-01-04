#ifndef _IMPOSTER_H_
#define _IMPOSTER_H_

#include "sgr_drawablenode.h"
#include <vector>
#include <map>

namespace SGR
{

    struct RenderOption;
    class SGR_DLL ImposterNode : public DrawableNode
    {
    public:
	ImposterNode ( const char* delis="" );
	ImposterNode ( const ImposterNode& rhs );
	virtual ~ImposterNode();
    
	virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }

	void setLodDelimiters ( const char* delis );

	// iterate functions
	float getLodDelimiter ( unsigned int idx );
	unsigned int getLodDelimiterCount ();

	/** set level "i" to dirty in imposter
	    when level "i" is rendered.
	    imposter node will update the inner presentation for level i,
	    and render the imposter image instead.
	*/
	void setImageDirty ( unsigned int idx, bool dirty );
	bool isImageDirty ( unsigned int idx );
	void updateImage ( unsigned int idx, RenderOption& opt );
	const ImageNode& getImage ( unsigned int idx ) const;
	// some convience interfaces
	/**
	   if argument "v" is in last range, return -1, means imposter directly return it's children when culling
	*/
	int getLodDelimiterIndex ( float v ) const;
    public:
	vector<float> _levelDelimiters;

	typedef map<unsigned int, pair<bool,ImageNode> >::iterator IMPOSTERS_ITERATOR;
	map<unsigned int/*index*/, pair<bool/*dirtyOrNot*/,ImageNode/*ImageNode*/> > _imposters;
    };

}

#endif // _IMPOSTER_H_

