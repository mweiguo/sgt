#ifndef _ARRAY_NODE_H_
#define _ARRAY_NODE_H_

#include <algorithm>
#include "sgr_nodemgr.h"
#include "sgr_global.h"
namespace SGR
{

class SGR_DLL ArrayNode : public virtual SGNode
{
public:
    ArrayNode ();
    ArrayNode ( const ArrayNode& rhs ) : SGNode(rhs)
    {
        _rowCnt        = rhs._rowCnt;
        _columnCnt     = rhs._columnCnt;
        for ( int i=0; i<6; i++ ) {
            _levelHCnt[i] = rhs._levelHCnt[i];
            _levelVCnt[i] = rhs._levelVCnt[i];
            _marginx[i]   = rhs._marginx[i];
            _marginy[i]   = rhs._marginy[i];
        }
    }

    void setRowCnt ( int cnt )    { _rowCnt = cnt; }
    void setColumnCnt ( int cnt ) { _columnCnt = cnt; }
    int getRowCnt ()    { return _rowCnt; }
    int getColumnCnt () { return _columnCnt; }
    void setHLevels ( int idx, int cnt ) { _levelHCnt[idx] = cnt; }
    void setVLevels ( int idx, int cnt ) { _levelVCnt[idx] = cnt; }
    int getHLevels ( int idx ) { return _levelHCnt[idx]; }
    int getVLevels ( int idx ) { return _levelVCnt[idx]; }
    void setMarginX ( int idx, float margin ) { _marginx[idx] = margin; }
    void setMarginY ( int idx, float margin ) { _marginy[idx] = margin; }
    float getMarginX ( int idx ) { return _marginx[idx]; }
    float getMarginY ( int idx ) { return _marginy[idx]; }
    int rowCnt () const { return _rowCnt; }
    int columnCnt () const { return _columnCnt; }
    float getVOffset ( int i ) const;
    float getHOffset ( int i ) const;
    void getShapeGenParas (int index, int& s1, int& s2, int& s3, int& s4, int& s5, int& s6, int level0Cnt, int level1Cnt, int level2Cnt, int level3Cnt, int level4Cnt, int level5Cnt ) const;
    
    virtual void accept ( NodeVisitor& pvisitor ) { pvisitor.apply ( *this ); }
    virtual ~ArrayNode () {}
private:
    int _rowCnt, _columnCnt, _levelHCnt[6], _levelVCnt[6];
    float _marginx[6], _marginy[6];
};

inline ArrayNode::ArrayNode ()
{
    std::fill ( _levelHCnt, _levelHCnt+6, 0 );
    std::fill ( _levelVCnt, _levelVCnt+6, 0 );
    std::fill ( _marginx, _marginx+6, 0.f );
    std::fill ( _marginy, _marginy+6, 0.f );
}

inline float ArrayNode::getVOffset ( int i ) const
{
    int s0, s1, s2, s3, s4, s5;
    getShapeGenParas ( i, s0, s1, s2, s3, s4, s5, _levelVCnt[0], _levelVCnt[1], _levelVCnt[2], _levelVCnt[3], _levelVCnt[4], _levelVCnt[5] );
    return s0*_marginy[0] + s1*_marginy[1] + s2*_marginy[2] + s3*_marginy[3] + s4*_marginy[4] + s5*_marginy[5];
}

inline float ArrayNode::getHOffset ( int i ) const
{
    int s0, s1, s2, s3, s4, s5;
    getShapeGenParas ( i, s0, s1, s2, s3, s4, s5, _levelHCnt[0], _levelHCnt[1], _levelHCnt[2], _levelHCnt[3], _levelHCnt[4], _levelHCnt[5] );
    return s0*_marginx[0] + s1*_marginx[1] + s2*_marginx[2] + s3*_marginx[3] + s4*_marginx[4] + s5*_marginx[5];
}

inline void ArrayNode::getShapeGenParas (int index, int& s0, int& s1, int& s2, int& s3, int& s4, int& s5,
                                         int level0, int level1, int level2, int level3, int level4, int level5 ) const
{
    s5 = level4 ? index / level4 : 0;
    s4 = level3 ? index / level3 - s5 : 0;
    s3 = level2 ? index / level2 - s4 - s5 : 0;
    s2 = level1 ? index / level1 - s3 - s4 - s5 : 0;
    s1 = level0 ? index / level0 - s2 - s3 - s4 - s5 : 0;
    s0 = level0 ? index - s1 - s2 - s3 - s4 - s5 : 0;
    //// init
    //s1 = s2 = s3 = s4 = s5 = 0;

    //if ( 0 == level1Cnt ) {
    //    //s1 = index;
    //    return;
    //}
    //else
    //    s1 = index - index / level1Cnt;

    //if ( 0 == level2Cnt ) {
    //    //s2 = index / level1Cnt;
    //    return;
    //}
    //else
    //    s2 = index / level1Cnt - index / level2Cnt;

    //if ( 0 == level3Cnt ) {
    //    //s3 = index / level2Cnt;
    //    return;
    //}
    //else
    //    s3 = index / level2Cnt - index / level3Cnt;

    //if ( 0 == level4Cnt ) {
    //    //s4 = index / level3Cnt;
    //    return;
    //}
    //else
    //    s4 = index / level3Cnt - index / level4Cnt;

    //if ( 0 == level5Cnt ) {
    //    //s5 = index / level4Cnt;
    //    return;
    //}
    //else
    //    s5 = index / level4Cnt - index / level5Cnt;

    //s6 = index / level5Cnt;
}
}
//typedef NodeMgr<ArrayNode>        ArrayNodeMgr;
#endif
