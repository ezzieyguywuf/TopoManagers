#include <PrimitiveSolidManager.h>
#include <OccSolidMaker.h>
#include <OccSolidModifier.h>
#include <OccModifiedSolid.h>
#include "gtest/gtest.h"

TEST(PrimitiveSolidManager, Create){
    Occ::Box myBox = Occ::SolidMaker::makeBox(10, 10, 10);
    PrimitiveSolidManager mgr(myBox);
    uint i = mgr.getFaceIndex(myBox.getNamedFace(Occ::FaceName::top));
    EXPECT_GE(i, 0);
}

TEST(PrimitiveSolidManager, getEdgeByIndex)
{
    // create the box
    Occ::Box myBox(Occ::SolidMaker::makeBox(10, 10, 10));
    // create the solid manager
    PrimitiveSolidManager mgr(myBox);

    // get an Edge index (note: in a real-world application, the user would select an edge
    // with a mouse.)
    Occ::Face front = myBox.getNamedFace(Occ::FaceName::front);
    Occ::Face right = myBox.getNamedFace(Occ::FaceName::right);
    Occ::Edge frontRightEdge = front.getCommonEdge(right);
    uint frontRightEdgeIndex = mgr.getEdgeIndex(frontRightEdge);

    // update the managed solid
    Occ::Box newBox = Occ::SolidMaker::makeBox(5, 5, 5);
    Occ::ModifiedSolid modSolid(myBox, newBox);
    mgr.updateSolid(modSolid);

    // retrieve the Edge using our index
    vector<Occ::Edge> retrievedFrontRightEdges = mgr.getEdgeByIndex(frontRightEdgeIndex);
    Occ::Edge retrievedFrontRightEdge = retrievedFrontRightEdges[0];

    // Now, in order to check it, build it from scratch too
    front = newBox.getNamedFace(Occ::FaceName::front);
    right = newBox.getNamedFace(Occ::FaceName::right);
    Occ::Edge checkFrontRightEdge = front.getCommonEdge(right);

    EXPECT_NE(frontRightEdge, retrievedFrontRightEdge);
    EXPECT_EQ(checkFrontRightEdge, retrievedFrontRightEdge);
    EXPECT_EQ(retrievedFrontRightEdges.size(), 1);
}

//TEST(PrimitiveSolidManager, FilletRetreivedEdge)
//{
    //// TODO finish writing test?
    //Occ::Box myBox(Occ::SolidMaker::makeBox(10, 10, 10));
    //// create the solid manager
    //PrimitiveSolidManager mgr(myBox);
//}

TEST(PrimitiveSolidManager, makeModifiedSolidInvalidFirstSolid)
{
    Occ::Box myBox1(Occ::SolidMaker::makeBox(10, 10, 10));
    Occ::Box myBox2(Occ::SolidMaker::makeBox(5, 10, 10));
    Occ::Box myBox3(Occ::SolidMaker::makeBox(10, 5, 10));
    Occ::Box myBox4(Occ::SolidMaker::makeBox(10, 10, 5));
    Occ::Box myBox5(Occ::SolidMaker::makeBox(5, 10, 5));

    // create the solid manager
    PrimitiveSolidManager mgr1(myBox1);

    // modify it a few times
    Occ::ModifiedSolid mod1(myBox1, myBox2);
    Occ::ModifiedSolid mod2(myBox2, myBox3);
    Occ::ModifiedSolid mod3(myBox3, myBox4);

    mgr1.updateSolid(mod1);
    mgr1.updateSolid(mod2);
    mgr1.updateSolid(mod3);

    // create a different solid manager
    PrimitiveSolidManager mgr2(myBox5);

    EXPECT_THROW(mgr1.makeModifiedSolid(myBox5), std::runtime_error);
    EXPECT_THROW(mgr2.makeModifiedSolid(myBox4), std::runtime_error);
}
