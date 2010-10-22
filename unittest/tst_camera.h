#include <sgr_cameraortho.h>
#include <sgr_viewport.h>

using namespace SGR;


TEST ( Nodes, CameraOrtho )
{
    {
	CameraOrtho cam;
	cam.zoom ( 1.5 );
	cam.translate ( vec3f(2, 2, 0) );

	vec4f temp = cam.mvmatrix () * vec4f ( 1, 0, 0, 1);
	vec4f temp1 = cam.inversematrix() * temp;

	EXPECT_EQ ( 1, temp1.x() );
	EXPECT_EQ ( 0, temp1.y() );
	EXPECT_EQ ( 0, temp1.z() );
	EXPECT_EQ ( 1, temp1.w() );
    }

    {
	CameraOrtho cam;
	cam.zoom ( 1.5 );
	cam.translate ( vec3f(2, 2, 0) );

	Viewport vp;
	vp.viewportSize ( 400, 300 );

	vec4f temp = vp.vpmatrix () * cam.mvmatrix () * vec4f ( 1, 0, 0, 1);
	vec4f temp1 = cam.inversematrix() * vp.inversematrix() * temp;

	EXPECT_EQ ( 1, temp1.x() );
	EXPECT_EQ ( 0, temp1.y() );
	EXPECT_EQ ( 0, temp1.z() );
	EXPECT_EQ ( 1, temp1.w() );
    }
}

