#include "sgr_seedgen.h"

namespace SGR
{
    SeedGenerator seedGenerator;
    SeedGenerator& SeedGenerator::getInst ()
    {
        return seedGenerator;
    }
    int SeedGenerator::minseed()
    { 
        return _minseed--; 
    }
    int SeedGenerator::maxseed()
    { 
        return _maxseed++; 
    }
    void SeedGenerator::minseed ( int seed ) 
    { 
        _minseed = seed; 
    }
    void SeedGenerator::maxseed ( int seed ) 
    { 
        _maxseed = seed; 
    }

    SeedGenerator::SeedGenerator()
    {
        minseed ( -2 );
        maxseed ( 1 );
    }

}
