#include <sgr_viewport.h>

using namespace SGR;


TEST ( Nodes, Viewport )
{
    Viewport vp;
    vp.viewportSize ( 400, 300 );

    vec4f temp  = vp.vpmatrix () * vec4f ( 1, 0, 0, 1);
    vec4f temp1 = vp.inversematrix() * temp;

    EXPECT_EQ ( 1, temp1.x() );
    EXPECT_EQ ( 0, temp1.y() );
    EXPECT_EQ ( 0, temp1.z() );
    EXPECT_EQ ( 1, temp1.w() );
}

