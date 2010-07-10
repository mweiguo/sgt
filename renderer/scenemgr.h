#ifndef _SCENEMGR_H_
#define _SCENEMGR_H_


#include "layernode.h"
#include <vector>
using namespace std;

/* #include <map> */
/* #include <utility> */

//class SceneMgr : public vector<LayerNode*>
//{
//public:
//    /* typedef vector<LayerNode*>::iterator iterator; */
//    /* typedef vector<LayerNode*>::const_iterator const_iterator; */
//
//    /* iterator begin() { return _layers.begin(); } */
//    /* iterator end() { return _layers.end(); } */
//    /* const_iterator begin() const { return _layers.begin(); } */
//    /* const_iterator end() const { return _layers.end(); } */
//    /* bool empty () const { return _layers.empty(); } */
//    static SceneMgr& getInst()
//    {
//        static SceneMgr inst;
//        return inst;
//    }
//private:
//    SceneMgr() {}
//    /* private: */
//    /*    vector<LayerNode*> _layers; */
//};

/* class LayerMgr */
/* { */
/*  public: */
/*     typedef vector<SpatialObjectMgr*>::iterator iterator; */
/*     typedef vector<SpatialObjectMgr*>::const_iterator const_iterator; */
/*     typedef vector<float> DefLevel; */

/*     // if in ortho project mode, v should be scale value */
/*     // if in project mode, v should be distance from eye to object */
/*     SpatialObjectMgr* selectPresentation ( float v )  */
/*     { */
/*         DefLevel::iterator pp = upper_bound ( levelDelimiters.begin(), levelDelimiters.end(), v ); */
/*         return lodModels[ (pp - levelDelimiters.begin()) ]; */
/*     } */

/*     static LodMgr& getInst() { */
/*         static LodMgr inst; */
/*         return inst; */
/*     } */

/*     iterator begin() { return lodModels.begin(); } */
/*     iterator end() { return lodModels.end(); } */
/*     const_iterator begin() const { return lodModels.begin(); } */
/*     const_iterator end() const { return lodModels.end(); } */
/*     bool empty () const { return lodModels.empty(); } */
/*  private: */
/*     LodMgr() {} */
/*  private: */
/*     vector<SpatialObjectMgr*> lodModels; */
/*     // in proj mode, this is dist array */
/*     // in ortho mode, this is scale array */
/*     vector<float> levelDelimiters; */
/* }; */


#endif
