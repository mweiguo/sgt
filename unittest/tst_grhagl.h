#include <sgr_grhagl.h>
#include <sgr_line.h>
using namespace SGR;

TEST ( Algorithm, IntersectLineLine )
{
    // intersect
    { vec2f out; EXPECT_EQ ( 1, intersect(Linef(0, 0, 10, 10), Linef(0, 10, 10, 0), out) ) << "should be intersect lines"; }
    { vec2f out; EXPECT_EQ ( 1, intersect(Linef(0, 0, 10, 10), Linef(0, 0, 10, 0), out) ) << "should be intersect lines"; }
    { vec2f out; EXPECT_EQ ( 1, intersect(Linef(-10, 0, 10, 0), Linef(0, -10, 0, 10), out) ) << "should be intersect lines"; }
    { vec2f out; EXPECT_EQ ( 1, intersect(Linef(0, 0, 10, 10), Linef(0, 0, 0, 10), out) ) << "should be intersect lines"; }
    { vec2f out; EXPECT_EQ ( 1, intersect(Linef(0, 0, 10, 10), Linef(0, 0, 10, 0), out) ) << "should be intersect lines"; }
    // coincident
    { vec2f out; EXPECT_EQ ( -1, intersect(Linef(0, 0, 10, 10), Linef(1, 1, 2, 2), out) ) << "should be coincident"; }
    { vec2f out; EXPECT_EQ ( -1, intersect(Linef(-10, 0, 10, 0), Linef(-9, 0, 11, 0), out) ) << "should be intersect lines"; }
    //parallel
    { vec2f out; EXPECT_EQ ( -2, intersect(Linef(0, 0, 10, 10), Linef(0, 10, 10, 20), out) ) << "should be parallel"; }
    { vec2f out; EXPECT_EQ ( -2, intersect(Linef(-10, 0, 10, 0), Linef(-10, 0.1, 10, 0.1), out) ) << "should be parallel"; }
    // not intersect
    { vec2f out; EXPECT_EQ ( 0, intersect(Linef(0, 0, 10, 10), Linef(5, 4.5, 10, 0), out) ) << "should be none intersect, (" << out.x() << ", " << out.y() << ")"; }
    { vec2f out; EXPECT_EQ ( 0, intersect(Linef(-10, 0, 10, 0), Linef(0, 0.1, 0, 10), out) ) << "should be none intersect, (" << out.x() << ", " << out.y() << ")"; }
}

TEST ( Algorithm, IntersectLineRect )
{
    // intersct
    {
        std::list<vec2f> out;
        EXPECT_EQ ( 2, intersect ( Linef(0,0,10,10), Rectanglef(1,0,6,6), out ) ) << "should be intersected\n";
    }
    {
        std::list<vec2f> out;
        EXPECT_EQ ( 1, intersect ( Linef(0,0,10,10), Rectanglef(5,0,5,5), out ) ) << "should be intersected\n";
        EXPECT_EQ ( 5, out.front().x() );
        EXPECT_EQ ( 5, out.front().y() );
    }
    {
        std::list<vec2f> out;
        EXPECT_EQ ( 2, intersect ( Linef(0,0,10,10), Rectanglef(0,0,5,5), out ) ) << "should be intersected\n";
        EXPECT_EQ ( 0, out.front().x() );
        EXPECT_EQ ( 0, out.front().y() );
        EXPECT_EQ ( 5, out.back().x()  );
        EXPECT_EQ ( 5, out.back().y()  );
    }
    {
        std::list<vec2f> out;
        EXPECT_EQ ( 1, intersect ( Linef(0,0,10,10), Rectanglef(-5,-5,10,10), out ) ) << "should be intersected\n";
        EXPECT_EQ ( 1u, out.size() );
        if ( out.size() == 1u )
        {
            EXPECT_EQ ( 5, out.back().x()  );
            EXPECT_EQ ( 5, out.back().y()  );
        }
    }
    {
        std::list<vec2f> out;
        EXPECT_EQ ( -5, intersect ( Linef(0,0,1,1), Rectanglef(-5,-5,10,10), out ) ) << "should be intersected\n";
        EXPECT_EQ ( 0u, out.size() );
    }

    // none intersct
    {
        std::list<vec2f> out;
        EXPECT_EQ ( 0, intersect ( Linef(0,0,10,10), Rectanglef(5,0,3,3), out ) ) << "should be no intersected\n";
    }

    // overlap
    {
        std::list<vec2f> out;
        int edge;
        EXPECT_GE ( -1, edge=intersect ( Linef(-10,0,10,0), Rectanglef(0,0,3,3), out ) ) << "should be overlapped\n";
        EXPECT_LE ( -4, edge=intersect ( Linef(-10,0,10,0), Rectanglef(0,0,3,3), out ) ) << "should be overlapped\n";
    }
}

