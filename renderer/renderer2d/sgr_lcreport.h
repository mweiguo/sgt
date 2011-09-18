#ifndef _LC_REPORT_H_
#define _LC_REPORT_H_

#include "sgr_lc.h"

class LCReport
{
public:
    LCReport ( LC& lc, bool dumpTree=false );
    void printCounter ();
private:
    void counter ( int type );
    void traverse ( LC& lc );
    string getContent ( int type, int idx, LC& lc, int gidx );

    int cscene;
    int cmat;
    int clayer;
    int clod;
    int clodpage;
    int cpline;
    int cpoly;
    int cline;
    int ctri;
    int cquad;
    int ctext;

    LC* _lc;
    bool _dumpTree;
};

#endif //_LC_REPORT_H_
