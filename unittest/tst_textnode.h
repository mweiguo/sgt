#include <sgr_textnode.h>
using namespace SGR;

TEST ( Nodes, TextNode )
{
    TextNode node;
    node.anchorValue (1);
    EXPECT_EQ ( true, node.isAnchorLeft () );
    EXPECT_EQ ( false, node.isAnchorHCenter () );
    EXPECT_EQ ( false, node.isAnchorRight () );
    EXPECT_EQ ( true, node.isAnchorTop () );
    EXPECT_EQ ( false, node.isAnchorVCenter () );
    EXPECT_EQ ( false, node.isAnchorBottom () );

    node.anchorValue (2);
    EXPECT_EQ ( false, node.isAnchorLeft () );
    EXPECT_EQ ( true, node.isAnchorHCenter () );
    EXPECT_EQ ( false, node.isAnchorRight () );
    EXPECT_EQ ( true, node.isAnchorTop () );
    EXPECT_EQ ( false, node.isAnchorVCenter () );
    EXPECT_EQ ( false, node.isAnchorBottom () );

    node.anchorValue (3);
    EXPECT_EQ ( false, node.isAnchorLeft () );
    EXPECT_EQ ( false, node.isAnchorHCenter () );
    EXPECT_EQ ( true, node.isAnchorRight () );
    EXPECT_EQ ( true, node.isAnchorTop () );
    EXPECT_EQ ( false, node.isAnchorVCenter () );
    EXPECT_EQ ( false, node.isAnchorBottom () );

    node.anchorValue (4);
    EXPECT_EQ ( true, node.isAnchorLeft () );
    EXPECT_EQ ( false, node.isAnchorHCenter () );
    EXPECT_EQ ( false, node.isAnchorRight () );
    EXPECT_EQ ( false, node.isAnchorTop () );
    EXPECT_EQ ( true, node.isAnchorVCenter () );
    EXPECT_EQ ( false, node.isAnchorBottom () );

    node.anchorValue (5);
    EXPECT_EQ ( false, node.isAnchorLeft () );
    EXPECT_EQ ( true, node.isAnchorHCenter () );
    EXPECT_EQ ( false, node.isAnchorRight () );
    EXPECT_EQ ( false, node.isAnchorTop () );
    EXPECT_EQ ( true, node.isAnchorVCenter () );
    EXPECT_EQ ( false, node.isAnchorBottom () );

    node.anchorValue (6);
    EXPECT_EQ ( false, node.isAnchorLeft () );
    EXPECT_EQ ( false, node.isAnchorHCenter () );
    EXPECT_EQ ( true, node.isAnchorRight () );
    EXPECT_EQ ( false, node.isAnchorTop () );
    EXPECT_EQ ( true, node.isAnchorVCenter () );
    EXPECT_EQ ( false, node.isAnchorBottom () );

    node.anchorValue (7);
    EXPECT_EQ ( true, node.isAnchorLeft () );
    EXPECT_EQ ( false, node.isAnchorHCenter () );
    EXPECT_EQ ( false, node.isAnchorRight () );
    EXPECT_EQ ( false, node.isAnchorTop () );
    EXPECT_EQ ( false, node.isAnchorVCenter () );
    EXPECT_EQ ( true, node.isAnchorBottom () );

    node.anchorValue (8);
    EXPECT_EQ ( false, node.isAnchorLeft () );
    EXPECT_EQ ( true, node.isAnchorHCenter () );
    EXPECT_EQ ( false, node.isAnchorRight () );
    EXPECT_EQ ( false, node.isAnchorTop () );
    EXPECT_EQ ( false, node.isAnchorVCenter () );
    EXPECT_EQ ( true, node.isAnchorBottom () );

    node.anchorValue (9);
    EXPECT_EQ ( false, node.isAnchorLeft () );
    EXPECT_EQ ( false, node.isAnchorHCenter () );
    EXPECT_EQ ( true, node.isAnchorRight () );
    EXPECT_EQ ( false, node.isAnchorTop () );
    EXPECT_EQ ( false, node.isAnchorVCenter () );
    EXPECT_EQ ( true, node.isAnchorBottom () );
}

