#include <sgr_projection.h>

using namespace SGR;


TEST ( Nodes, Projection )
{
    Projection proj;

    vec4f temp = proj.projmatrix () * vec4f ( 1, 0, 0, 1);
    vec4f temp1 = proj.inversematrix() * temp;

    EXPECT_EQ ( 1, temp1.x() );
    EXPECT_EQ ( 0, temp1.y() );
    EXPECT_EQ ( 0, temp1.z() );
    EXPECT_EQ ( 1, temp1.w() );
}

