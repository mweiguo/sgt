#ifndef _BBOX_H_
#define _BBOX_H_

#include "bbox.h"

class bboxTest : public BBox
{
public:
    bboxTest () {
        minmaxTest();
    }
private:
    void minmaxTest () {
        init ( vec3f(1,1,1) );
        expandby ( vec3f(0,2,2) );
        if ( min().x()!=0 || min().y()!=1 || min().z()!=1 || 
             max().x()!=1 || max().y()!=2 || max().z()!=2 )
            throw logic_error ("bboxTest minmax failed");
    }
    void centerTest() {
        init ( vec3f(2,1,1) );
        expandby ( vec3f(0,3,3) );
        if ( center().x()!=1 || center().y()!=2 || center().z()!=2 )
            throw logic_error ("bboxTest centerTest failed");
    }
    void unionboxTest() {
        init ( vec3f(0,0,0) );
        expandby ( vec3f(2,2,2) );

        BBox t1;
        t1.init ( vec3f(1,1,1) );
        t1.expandby ( vec3f(4,4,4) );

        BBox t2 = unionbox ( t1 );

        if ( t2.min().x()!=0 || t2.min().y()!=0 || t2.min().z()!=0 || 
             t2.max().x()!=4 || t2.max().y()!=4 || t2.max().z()!=4 )
            throw logic_error ("bboxTest unionboxTest failed");
    }

};

#endif
