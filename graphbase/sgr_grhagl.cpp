#include "sgr_grhagl.h"
#include <stdexcept>
#include "sgr_kdtree.h"
#include "sgr_rectangle.h"
#include <map>
#include <iostream>
#include <utl_logger.h>
 
namespace SGR
{
    bool is_outside ( const vec3f& lhs, const BBox& rhs )
    {
        if (lhs.x() < rhs.minvec().x() || 
            lhs.y() < rhs.minvec().y() ||
            lhs.x() > rhs.maxvec().x() || 
            lhs.y() > rhs.maxvec().y() )
            return false;
	return true;
    }

    bool is_outside ( const vec3f& lhs, const Rectanglef& rhs )
    {
        if (lhs.x() < rhs.x() || 
            lhs.y() < rhs.y() ||
            lhs.x() > (rhs.x() + rhs.w()) || 
            lhs.y() > (rhs.y() + rhs.h()) )
            return false;
	return true;
    }

    // reference, http://give.zju.edu.cn/cad/new/book/2.5.1.htm
    bool is_intersect ( const Linef& lhs, const Linef& rhs )
    {
// 	LOG_INFO ( "LINEa p1(%f, %f), p2(%f, %f)", lhs.x1(), lhs.y1(), lhs.x2(), lhs.y2() );
// 	LOG_INFO ( "LINEb p1(%f, %f), p2(%f, %f)", rhs.x1(), rhs.y1(), rhs.x2(), rhs.y2() );
	vec2f out;
	int cnt = intersect ( lhs, rhs, out );
//	LOG_INFO ( "cnt = %d, cnt.x=%f, cnt.y=%f", cnt, out.x(), out.y() );
	return (cnt != 0) && (cnt!=-2);
/*
        vec3f A ( rhs.x1()-lhs.x1(), rhs.y1()-lhs.y1() );
        vec3f B ( rhs.x2()-lhs.x1(), rhs.y2()-lhs.y1() );
        vec3f t1 = cross ( A, B );
        // 点在线上
        if ( t1 == vec3f(0,0,0) )
	{
            LOG_INFO ("123132123");
	    return true;
	}
        vec3f C ( rhs.x1()-lhs.x2(), rhs.y1()-lhs.y2() );
        vec3f D ( rhs.x2()-lhs.x2(), rhs.y2()-lhs.y2() );
        vec3f t2 = cross ( C, D );
        // 点在线上
        if ( t1 == vec3f(0,0,0) )
	{
            LOG_INFO ("safasfasasf");
            return true;
	}

        if ( (t1.normal() + t2.normal()) == vec3f(0,0,0) )
	{
            LOG_INFO ("vxzm,vnzx,vmnxz,mvnsafasfasasf");
            return true;
	}
        return false;
*/
        /*
        vec3f A ( lhs.x1(), lhs.y1() );
        vec3f B ( lhs.x2()-lhs.x1(),  lhs.y2()-lhs.y1() );
        vec3f C ( rhs.x1(), rhs.y1() );
        vec3f D ( rhs.x2()-rhs.x1(),  rhs.y2()-rhs.y1() );
        if ( 0==lhs.x1() && 0==lhs.y1() )
        {
        A.xy (lhs.x2(), lhs.y2());
        B.xy (lhs.x1()-lhs.x2(), lhs.y1()-lhs.y2());
        }
        if ( 0==rhs.x1() && 0==rhs.y1() )
        {
        C.xy (rhs.x2(), rhs.y2());
        D.xy (rhs.x1()-rhs.x2(), rhs.y1()-rhs.y2());
        }

        vec3f E = cross ( A, B );
        vec3f F = cross ( C, D );
        float s;

        if ( E.mod () != 0 )
        {
        float tmp = -dot ( E, D );
        if ( tmp == 0 )
        return false;
        s = dot ( E, C ) / tmp;
        }
        else if ( F.mod() != 0 )
        {
        float tmp = -dot ( F, B );
        if ( tmp == 0 )
        return false;
        s = dot ( F, A ) / tmp;
        }
        else
        {
        return false;
        }



        if ( s >= 0 && s <= 1 )
        return true;
        return false;
        */
        /*
        double A1, B1, C1, A2, B2, C2, D;

        if ( lhs.x1()==lhs.x2() && lhs.y1()!=lhs.y2() ){
        A1 = 1;    B1 = 0;    C1 = lhs.x1();
        } else if ( lhs.y1()==lhs.y2() && lhs.x1()!=lhs.x2() ){
        A1 = 0;    B1 = 1;    C1 = lhs.y1();
        } else if( lhs.x1()!=lhs.x2() && lhs.y1()!=lhs.y2() ){
        A1 = lhs.y2()-lhs.y1();    B1 = lhs.x1()-lhs.x2();    C1 = lhs.x1()*lhs.y2() - lhs.x2()*lhs.y1();
        }else{
        throw std::invalid_argument ( "is_intersect: invalide parameters" );
        }

        if ( rhs.x1()==rhs.x2() && rhs.y1()!=rhs.y2() ){
        A2 = 1;    B2 = 0;    C2 = rhs.x1();
        }else if ( rhs.y1()==rhs.y2() ){
        A2 = 0;    B2 = 1;    C2 = rhs.y1();
        }else if( rhs.x1()!=rhs.x2() && rhs.y1()!=rhs.y2() ){
        A2 = rhs.y2()-rhs.y1();    B2 = rhs.x1()-rhs.x2();    C2 = rhs.x1()*rhs.y2() - rhs.x2()*rhs.y1();
        }else{
        throw std::invalid_argument ( "is_intersect: invalide parameters" );
        }

        D = A1*B2 - A2*B1;

        double x, y;
        if ( D!=0 ){
        x = C1*B2 - C2*B1;
        y = A1*C2 - A2*C1;
        x /= D;
        y /= D;
        }else{
        if ( C1==C2 )//overlap
        return true;
        else
        return false;
        }

        // check extent
        double minx, maxx, miny, maxy;
        minx = lhs.x1()<lhs.x2() ? (maxx=lhs.x2(),lhs.x1()) : (maxx=lhs.x1(),lhs.x2());
        miny = lhs.y1()<lhs.y2() ? (maxy=lhs.y2(),lhs.y1()) : (maxy=lhs.y1(),lhs.y2());
        if ( x<minx || x>maxx || y<miny || y>maxy )
        return false;

        return true;
        */
    }

    bool is_intersect ( const BBox& lhs, const Linef& rhs )
    {
        //     2
        //  1    3
        //     4
        Linef line1 ( lhs.minvec().x(), lhs.minvec().y(), lhs.minvec().x(), lhs.maxvec().y() );
//	LOG_INFO ( "LINE 1 p1(%f, %f), p2(%f, %f)", lhs.minvec().x(), lhs.minvec().y(), lhs.minvec().x(), lhs.maxvec().y() );
        if ( is_intersect ( line1, rhs ) )
	{
//	    LOG_INFO ( "LINE1 1 bingo" );
            return true;
	}
        Linef line2 ( lhs.minvec().x(), lhs.maxvec().y(), lhs.maxvec().x(), lhs.maxvec().y() );
//	LOG_INFO ( "LINE 2 p1(%f, %f), p2(%f, %f)", lhs.minvec().x(), lhs.maxvec().y(), lhs.maxvec().x(), lhs.maxvec().y() );
        if ( is_intersect ( line2, rhs ) )
	{
//	    LOG_INFO ( "LINE 2 bingo" );
            return true;
	}
        Linef line3 ( lhs.maxvec().x(), lhs.maxvec().y(), lhs.maxvec().x(), lhs.minvec().y() );
//	LOG_INFO ( "LINE 3 p1(%f, %f), p2(%f, %f)", lhs.maxvec().x(), lhs.maxvec().y(), lhs.maxvec().x(), lhs.minvec().y() );
        if ( is_intersect ( line3, rhs ) )
	{
//	    LOG_INFO ( "LINE 3 bingo" );
            return true;
	}
        Linef line4 ( lhs.maxvec().x(), lhs.minvec().y(), lhs.minvec().x(), lhs.minvec().y() );
//	LOG_INFO ( "LINE 4 p1(%f, %f), p2(%f, %f)", lhs.maxvec().x(), lhs.minvec().y(), lhs.minvec().x(), lhs.minvec().y() );
        if ( is_intersect ( line4, rhs ) )
	{
//	    LOG_INFO ( "LINE 4 bingo" );
            return true;
	}
        return false;
    }

    bool is_intersect ( const BBox& lhs, const BBox& rhs )
    {
//	static int cnt = 0;
        if ( lhs.maxvec().x() < rhs.minvec().x() || 
            lhs.maxvec().y() < rhs.minvec().y() ||
            lhs.minvec().x() > rhs.maxvec().x() || 
            lhs.minvec().y() > rhs.maxvec().y() )
            return false;
// 	stringstream ss;
// 	ss << "( " << lhs.minvec().x() << ", " << lhs.minvec().y() << ", " << lhs.maxvec().x() << ", " << lhs.maxvec().y() << " )";
// 	ss << "    ( " << rhs.minvec().x() << ", " << rhs.minvec().y() << ", " << rhs.maxvec().x() << ", " << rhs.maxvec().y() << " )" << cnt++;
// 	qDebug ( ss.str().c_str() );
        return true;
    }

    bool is_contain ( const BBox& lhs, const BBox& rhs )
    {
        if ( lhs.maxvec().x() >= rhs.maxvec().x() && 
            lhs.minvec().x() <= rhs.minvec().x() &&
            lhs.maxvec().y() >= rhs.maxvec().y() &&
            lhs.minvec().y() <= rhs.minvec().y() )
            return true;
        return false;
    }

    bool is_contain ( const BBox& lhs, const vec3f& rhs )
    {
        if ( rhs.x() >= lhs.minvec().x() && 
            rhs.y() >= lhs.minvec().y() &&
            rhs.x() <= lhs.maxvec().x() &&
            rhs.y() <= lhs.maxvec().y() )
            return true;
        return false; 
    }

    bool SGR_DLL is_contain ( const Rectanglef& lhs, const vec2f& rhs )
    {
        if ( rhs.x() >= lhs.x() && 
            rhs.y() >= lhs.y() &&
            rhs.x() <= lhs.x() + lhs.w() &&
            rhs.y() <= lhs.y() + lhs.h() )
            return true;
        return false; 
    }


    bool is_separate ( const BBox& lhs, const BBox& rhs )
    {
        if ( lhs.maxvec().x() < rhs.minvec().x() || 
            lhs.minvec().x() > rhs.maxvec().x() || 
            lhs.minvec().y() > rhs.maxvec().y() || 
            lhs.maxvec().y() < rhs.minvec().y())
            return true;
        
        return false;
    }

    //bool is_separate ( const BBox& lhs, const vec3f& rhs )
    //{
    //}
    int intersect ( const Linef& lhs, const Rectanglef& rhs, std::list<vec2f>& out )
    {
	Linef ltrb[4];
	ltrb[0] = rhs.leftLine();
	ltrb[1] = rhs.topLine();
	ltrb[2] = rhs.rightLine();
	ltrb[3] = rhs.bottomLine();
	std::list<vec2f> temp;

	if ( is_contain ( rhs, lhs.point1() ) && is_contain ( rhs, lhs.point2() ) )
	    return -5;

	for ( int i=0; i<4; i++ )
	{
	    vec2f pnt;
	    int icode = intersect ( lhs, ltrb[i], pnt );
	    if ( icode == -1 )  // overlap
	    {
		return -(i+1);
	    }
	    else if ( icode > 0 )
	    {
		temp.push_back ( pnt );
	    }
	}

	// unique
	for ( std::list<vec2f>::iterator pp=temp.begin(); pp!=temp.end(); ++pp )
	{
	    if ( std::find ( out.begin(), out.end(), *pp ) == out.end() )
		out.push_back ( *pp );
	}
	
// 	for ( std::list<vec2f>::iterator pp=out.begin(); pp!=out.end(); ++pp )
// 	    std::cout << "( " << (*pp).x() << ", " << (*pp).y() << " )   ";
// 	std::cout << std::endl;

	// reorder intersection points
	if ( 2 == out.size () )
	{
	    float dist1 = dist ( lhs.point1(), out.front() );
	    float dist2 = dist ( lhs.point2(), out.back () );
	    if ( dist1 > dist2 )
		out.reverse ();
	}
	return out.size();
    }

    // reference: http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
    int intersect ( const Linef& lhs, const Linef& rhs, vec2f& out )
    {
	float x1 = lhs.x1(), y1 = lhs.y1();
	float x2 = lhs.x2(), y2 = lhs.y2();
	float x3 = rhs.x1(), y3 = rhs.y1();
	float x4 = rhs.x2(), y4 = rhs.y2();

	float denominator = (y4-y3)*(x2-x1) - (x4-x3)*(y2-y1);
	float numerator1  = (x4-x3)*(y1-y3) - (y4-y3)*(x1-x3);
	float numerator2  = (x2-x1)*(y1-y3) - (y2-y1)*(x1-x3);

	if ( 0==denominator && (0==numerator1 || 0==numerator2) )
	    return -1;
	else if ( 0==denominator )
	    return -2;
	
	float u1 = numerator1 / denominator;
	float u2 = numerator2 / denominator;
	
	if ( u1<0 || u1>1 || u2<0 || u2>1 )
	    return 0;

	out = lhs.point1() + (lhs.point2() - lhs.point1()) * u1;
	return 1;
    }

    float dist ( const vec2f& lhs, const vec2f& rhs )
    {
	return sqrt ( (lhs.x()-rhs.x())*(lhs.x()-rhs.x()) + (lhs.y()-rhs.y())*(lhs.y()-rhs.y()) );
    }

#define KDT_RECTANGLE 1

    template <class T>
    class GetRECTBBox
    {
    public:
        SGR::BBox operator() ( T t ) { return t->boundingbox(); }
    };

    class KDTHelper
    {
    public:
        KDTHelper ( int objType=KDT_RECTANGLE ) : _objType (objType)
        {
            _data = NULL;
            if ( KDT_RECTANGLE == _objType )
            {
                _data = new KdTree<Rectanglef*>();
            }
        }
        ~KDTHelper ()
        {
            if ( NULL == _data )
                return;

            if ( KDT_RECTANGLE == _objType )
            {
                delete (KdTree<SGR::Rectanglef*>*)(_data);
            }
        }
        void addElements ( void* objptr, int objN )
        {
            if ( KDT_RECTANGLE == _objType )
            {
                vec4f* coords = (vec4f*)objptr;
                KdTree<Rectanglef*>* pData = (KdTree<Rectanglef*>*)(_data);
                for ( int i=0; i<objN; i++, ++coords )
                {
                    Rectanglef* tmp = new Rectanglef ( coords->x(), coords->y(), coords->z(), coords->w() );
                    pData->push_back ( tmp );
                }
            }
        }
        void buildIdx ()
        {
            if ( KDT_RECTANGLE == _objType )
            {
                KdTree<Rectanglef*>* pData = (KdTree<Rectanglef*>*)(_data);
                KdTree<Rectanglef*>::BuildKdTree<GetRECTBBox<Rectanglef*> > build (*pData, GetRECTBBox<Rectanglef*>() );
            }
        }
        void clear()
        {
            if ( KDT_RECTANGLE == _objType )
            {
                KdTree<Rectanglef*>* pData = (KdTree<Rectanglef*>*)(_data);
                KdTree<Rectanglef*>::iterator pp, end=pData->end();
                for ( pp=pData->begin(); pp!=end; ++pp )
                    delete (*pp);
            }
        }
        bool intersect ( vec3f mincoord, vec3f maxcoord, void* objptr, unsigned int& objN )
        {
            if ( KDT_RECTANGLE == _objType )
            {
                KdTree<Rectanglef*>* pData = (KdTree<Rectanglef*>*)(_data);

                vector<Rectanglef*> out;

                pData->intersect ( 
                    BBox(mincoord, vec3f(maxcoord)), 
                    back_inserter(out), 
                    GetRECTBBox<Rectanglef*>()
                );

                Rectanglef* tmp = (Rectanglef*)objptr;
                for ( size_t i=0; i<objN; i++, ++tmp )
                    tmp->setRect ( out[i]->x(), out[i]->y(), out[i]->w(), out[i]->h() );

                objN = objN > out.size() ? out.size() : objN;
            }

            return objN!=0;
        }
    private:
        int _objType;
        void* _data;
    };

    map< int, KDTHelper > kdt_mgr;
    void kdtree_create ( int id, int objtype )
    {
        KDTHelper kdt ( objtype );
        kdt_mgr[id] = kdt;
    }

    void kdtree_add ( int id, void* objptr, int objN )
    {
        KDTHelper& kdt = kdt_mgr[id];
        kdt.addElements ( objptr, objN );
    }

    void kdtree_buildidx ( int id )
    {
        KDTHelper& kdt = kdt_mgr[id];
        kdt.buildIdx ();
    }

    void kdtree_clearall ( int id )
    {
        KDTHelper& kdt = kdt_mgr[id];
        kdt.clear ();
    }

    bool kdtree_intersect ( int id, float minx, float miny, float minz, float maxx, float maxy, float maxz, void* objptr, unsigned int& objN )
    {
        KDTHelper& kdt = kdt_mgr[id];
        return kdt.intersect ( vec3f(minx, miny, minz), vec3f(maxx, maxy, maxz), objptr, objN );
    }

}
