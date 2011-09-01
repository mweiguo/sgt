#include "gtest/gtest.h"
#include "bbox2d.h"
#include "sgr_math.h"

#include <iostream>


// ********************************************************************************

TEST ( LineOutSideCase1, row1 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 1
    { float line[] = { -1, -1, 0, 0 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {  0, -1, 1, 0 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {  1, -1, 2, 0 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {  2, -1, 3, 0 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {  3, -1, 4, 0 }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase1, row2 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 2
    { float line[] = { -1, 0, 0, 1 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {  0, 0, 1, 1 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {  1, 0, 2, 1 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {  2, 0, 3, 1 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {  3, 0, 4, 1 }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase1, row3 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 3
    { float line[] = { -1, 1, 0, 2 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {  0, 1, 1, 2 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {  1, 1, 2, 2 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {  2, 1, 3, 2 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {  3, 1, 4, 2 }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase1, row4 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 4
    { float line[] = { -1, 2, 0, 3 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {  0, 2, 1, 3 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {  1, 2, 2, 3 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {  2, 2, 3, 3 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {  3, 2, 4, 3 }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase1, row5 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 5
    { float line[] = { -1, 3, 0, 4 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {  0, 3, 1, 4 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {  1, 3, 2, 4 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {  2, 3, 3, 4 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {  3, 3, 4, 4 }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ********************************************************************************

TEST ( AABBOutsideCase1, row1 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 1
    { float bbox[] = { -1, -1, 0, 0 }; EXPECT_EQ ( true, aabb_outside(bbox,box) ); }
    { float bbox[] = {  0, -1, 1, 0 }; EXPECT_EQ ( true, aabb_outside(bbox,box) ); }
    { float bbox[] = {  1, -1, 2, 0 }; EXPECT_EQ ( true, aabb_outside(bbox,box) ); }
    { float bbox[] = {  2, -1, 3, 0 }; EXPECT_EQ ( true, aabb_outside(bbox,box) ); }
    { float bbox[] = {  3, -1, 4, 0 }; EXPECT_EQ ( true, aabb_outside(bbox,box) ); }
}

// ================================================================================

TEST ( AABBOutsideCase1, row2 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 2
    { float bbox[] = { -1, 0, 0, 1 }; EXPECT_EQ ( true, aabb_outside(bbox,box) ); }
    { float bbox[] = {  0, 0, 1, 1 }; EXPECT_EQ ( false, aabb_outside(bbox,box) ); }
    { float bbox[] = {  1, 0, 2, 1 }; EXPECT_EQ ( false, aabb_outside(bbox,box) ); }
    { float bbox[] = {  2, 0, 3, 1 }; EXPECT_EQ ( false, aabb_outside(bbox,box) ); }
    { float bbox[] = {  3, 0, 4, 1 }; EXPECT_EQ ( true, aabb_outside(bbox,box) ); }
}

// ================================================================================

TEST ( AABBOutsideCase1, row3 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 3
    { float bbox[] = { -1, 1, 0, 2 }; EXPECT_EQ ( true,  aabb_outside(bbox,box) ); }
    { float bbox[] = {  0, 1, 1, 2 }; EXPECT_EQ ( false, aabb_outside(bbox,box) ); }
    { float bbox[] = {  1, 1, 2, 2 }; EXPECT_EQ ( false, aabb_outside(bbox,box) ); }
    { float bbox[] = {  2, 1, 3, 2 }; EXPECT_EQ ( false, aabb_outside(bbox,box) ); }
    { float bbox[] = {  3, 1, 4, 2 }; EXPECT_EQ ( true,  aabb_outside(bbox,box) ); }
}

// ================================================================================

TEST ( AABBOutsideCase1, row4 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 4
    { float bbox[] = { -1, 2, 0, 3 }; EXPECT_EQ ( true,  aabb_outside(bbox,box) ); }
    { float bbox[] = {  0, 2, 1, 3 }; EXPECT_EQ ( false, aabb_outside(bbox,box) ); }
    { float bbox[] = {  1, 2, 2, 3 }; EXPECT_EQ ( false, aabb_outside(bbox,box) ); }
    { float bbox[] = {  2, 2, 3, 3 }; EXPECT_EQ ( false, aabb_outside(bbox,box) ); }
    { float bbox[] = {  3, 2, 4, 3 }; EXPECT_EQ ( true,  aabb_outside(bbox,box) ); }
}

// ================================================================================

TEST ( AABBOutsideCase1, row5 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 5
    { float bbox[] = { -1, 3, 0, 4 }; EXPECT_EQ ( true, aabb_outside(bbox,box) ); }
    { float bbox[] = {  0, 3, 1, 4 }; EXPECT_EQ ( true, aabb_outside(bbox,box) ); }
    { float bbox[] = {  1, 3, 2, 4 }; EXPECT_EQ ( true, aabb_outside(bbox,box) ); }
    { float bbox[] = {  2, 3, 3, 4 }; EXPECT_EQ ( true, aabb_outside(bbox,box) ); }
    { float bbox[] = {  3, 3, 4, 4 }; EXPECT_EQ ( true, aabb_outside(bbox,box) ); }
}

// ********************************************************************************

TEST ( LineOutSideCase2, row1 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 1
    { float line[] = {   0, -1,   0, 0 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {   1, -1,   1, 0 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = { 1.5, -1, 1.5, 0 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {   2, -1,   2, 0 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {   3, -1,   3, 0 }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase2, row2 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 2
    { float line[] = {   0, 0,   0, 1 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {   1, 0,   1, 1 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1.5, 0, 1.5, 1 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {   2, 0,   2, 1 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {   3, 0,   3, 1 }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase2, row3 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 2
    { float line[] = {   0, 0,   0, 1.5 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {   1, 0,   1, 1.5 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1.5, 0, 1.5, 1.5 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {   2, 0,   2, 1.5 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {   3, 0,   3, 1.5 }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase2, row4 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 2
    { float line[] = {   0, 1.25,   0, 1.75 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {   1, 1.25,   1, 1.75 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1.5, 1.25, 1.5, 1.75 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {   2, 1.25,   2, 1.75 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {   3, 1.25,   3, 1.75 }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase2, row5 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 2
    { float line[] = {   0, 1.5,   0, 2 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {   1, 1.5,   1, 2 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1.5, 1.5, 1.5, 2 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {   2, 1.5,   2, 2 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {   3, 1.5,   3, 2 }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase2, row6 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 2
    { float line[] = {   0, 1.5,   0, 2.5 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {   1, 1.5,   1, 2.5 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1.5, 1.5, 1.5, 2.5 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {   2, 1.5,   2, 2.5 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {   3, 1.5,   3, 2.5 }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase2, row7 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 2
    { float line[] = {   0, 2,   0, 3 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {   1, 2,   1, 3 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1.5, 2, 1.5, 3 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {   2, 2,   2, 3 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = {   3, 2,   3, 3 }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase2, row8 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // row 2
    { float line[] = {   0, 2.5,   0, 3 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {   1, 2.5,   1, 3 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = { 1.5, 2.5, 1.5, 3 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {   2, 2.5,   2, 3 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = {   3, 2.5,   3, 3 }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ********************************************************************************

TEST ( LineOutSideCase3, col1 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // column 1
    { float line[] = { -1, 0, 	0, 0   }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = { -1, 1, 	0, 1   }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = { -1, 1.5, 0, 1.5 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = { -1, 2,   0, 2   }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = { -1, 3,   0, 3   }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase3, col2 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // column 2
    { float line[] = { 0, 0,   1, 0   }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = { 0, 1,   1, 1   }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 0, 1.5, 1, 1.5 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 0, 2,   1, 2   }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 0, 3,   1, 3   }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase3, col3 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // column 3
    { float line[] = { 1, 0,   1.5, 0   }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = { 1, 1,   1.5, 1   }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1, 1.5, 1.5, 1.5 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1, 2,   1.5, 2   }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1, 3,   1.5, 3   }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase3, col4 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // column 4
    { float line[] = { 1.25, 0,   1.5, 0   }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = { 1.25, 1,   1.5, 1   }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1.25, 1.5, 1.5, 1.5 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1.25, 2,   1.5, 2   }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1.25, 3,   1.5, 3   }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase3, col5 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // column 5
    { float line[] = { 1.5, 0,   2, 0   }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = { 1.5, 1,   2, 1   }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1.5, 1.5, 2, 1.5 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1.5, 2,   2, 2   }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1.5, 3,   2, 3   }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase3, col6 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // column 6
    { float line[] = { 1.5, 0,   2.5, 0   }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = { 1.5, 1,   2.5, 1   }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1.5, 1.5, 2.5, 1.5 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1.5, 2,   2.5, 2   }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 1.5, 3,   2.5, 3   }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase3, col7 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // column 7
    { float line[] = { 2, 0,   2.5, 0   }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = { 2, 1,   2.5, 1   }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 2, 1.5, 2.5, 1.5 }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 2, 2,   2.5, 2   }; EXPECT_EQ ( false, line_outside(line,box) ); }
    { float line[] = { 2, 3,   2.5, 3   }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================

TEST ( LineOutSideCase3, col8 ) {
    BBox2d box;
    box.init     ( vec2f( 1, 1) );
    box.expandby ( vec2f( 2, 2) );

    // column 8
    { float line[] = { 3, 0,   4, 0   }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = { 3, 1,   4, 1   }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = { 3, 1.5, 4, 1.5 }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = { 3, 2,   4, 2   }; EXPECT_EQ ( true, line_outside(line,box) ); }
    { float line[] = { 3, 3,   4, 3   }; EXPECT_EQ ( true, line_outside(line,box) ); }
}

// ================================================================================
