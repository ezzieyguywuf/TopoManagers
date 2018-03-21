#include <ISolidManager.h>
#include <PrimitiveSolidManager.h>
#include <OccSolidMaker.h>
#include <OccSolidModifier.h>
#include <OccModifiedSolid.h>
#include "gtest/gtest.h"

TEST(ISolidManager, makeModifiedSolidInvalidFirstSolid)
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

    EXPECT_THROW(ISolidManager::makeModifiedSolid(mgr1, mgr2), std::runtime_error);
    EXPECT_THROW(ISolidManager::makeModifiedSolid(mgr2, mgr1), std::runtime_error);
}

TEST(ISolidManager, makeModifiedSolid)
{
    Occ::Box myBox1(Occ::SolidMaker::makeBox(10, 10, 10));
    Occ::Box myBox2(Occ::SolidMaker::makeBox(5, 10, 10));
    Occ::Box myBox3(Occ::SolidMaker::makeBox(10, 5, 10));
    Occ::Box myBox4(Occ::SolidMaker::makeBox(10, 10, 5));
    Occ::Box myBox5(Occ::SolidMaker::makeBox(5, 10, 5));

    // create the solid managers
    PrimitiveSolidManager mgr1(myBox1);
    PrimitiveSolidManager mgr2(myBox1);

    // modify one more than the other
    Occ::ModifiedSolid mod1(myBox1, myBox2);
    Occ::ModifiedSolid mod2(myBox2, myBox3);
    Occ::ModifiedSolid mod3(myBox3, myBox4);

    mgr1.updateSolid(mod1);

    mgr2.updateSolid(mod1);
    mgr2.updateSolid(mod2);
    mgr2.updateSolid(mod3);

    // generate a ModifiedSolid using ISolidManager as well as manually (to check
    // the generated one)
    Occ::ModifiedSolid genVal = ISolidManager::makeModifiedSolid(mgr1, mgr2);
    const Occ::ModifiedSolid checkVal(myBox2, myBox4);

    EXPECT_EQ(genVal, checkVal);
}

TEST(ISolidManager, getAnyEdgeByIndex)
{
    // create the box
    Occ::Box myBox(Occ::SolidMaker::makeBox(10, 10, 10));
    // create the solid manager
    PrimitiveSolidManager mgr(myBox);

    EXPECT_NO_THROW(mgr.getEdgeIndex(myBox.getEdges()[0]));
    EXPECT_NO_THROW(mgr.getEdgeIndex(myBox.getEdges()[1]));
    EXPECT_NO_THROW(mgr.getEdgeIndex(myBox.getEdges()[2]));
    EXPECT_NO_THROW(mgr.getEdgeIndex(myBox.getEdges()[3]));
    EXPECT_NO_THROW(mgr.getEdgeIndex(myBox.getEdges()[4]));
    EXPECT_NO_THROW(mgr.getEdgeIndex(myBox.getEdges()[5]));
    EXPECT_NO_THROW(mgr.getEdgeIndex(myBox.getEdges()[6]));
    EXPECT_NO_THROW(mgr.getEdgeIndex(myBox.getEdges()[7]));
    EXPECT_NO_THROW(mgr.getEdgeIndex(myBox.getEdges()[8]));
    EXPECT_NO_THROW(mgr.getEdgeIndex(myBox.getEdges()[9]));
    EXPECT_NO_THROW(mgr.getEdgeIndex(myBox.getEdges()[10]));
    EXPECT_NO_THROW(mgr.getEdgeIndex(myBox.getEdges()[11]));
}

TEST(ISolidManager, getEdgeByIndex)
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

//TEST(ISolidManager, FilletRetreivedEdge)
//{
    //// TODO finish writing test?
    //Occ::Box myBox(Occ::SolidMaker::makeBox(10, 10, 10));
    //// create the solid manager
    //PrimitiveSolidManager mgr(myBox);
//}
