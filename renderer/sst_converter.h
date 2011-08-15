
#ifndef _SST_CONVERTER_H_
#define _SST_CONVERTER_H_

#include "sgr_statesetbuilder.h"

class SSTConverter
{
public:
    void convert ( const char* lcfilename, const char* ssfilename );
    void parseAppearance ();
//    void parseTransform ();
    void traverse ();

    LC lc;
    SSTBuilder ssbuilder;
    map<string,int> texManager;
};

#endif // _SST_CONVERTER_H_
