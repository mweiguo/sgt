#ifndef _SEEDGEN_H_
#define _SEEDGEN_H_

#include "sgr_global.h"

namespace SGR
{

    class SGR_DLL SeedGenerator
    {
    public:
        static SeedGenerator& getInst ();
        int minseed();
        int maxseed();
        void minseed ( int seed );
        void maxseed ( int seed );
        SeedGenerator();
    private:
        int _minseed;
        int _maxseed;
    };

}

#endif
