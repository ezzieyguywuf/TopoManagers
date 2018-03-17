#include <CompoundSolidManager.h>
#include <OccSolidMaker.h>
#include <OccSolidModifier.h>
#include <OccModifiedSolid.h>
#include <OccBooleanSolid.h>

#include "gtest/gtest.h"

#include <stdexcept>

TEST(CompoundSolidManager, Create){
    Occ::Box myBox = Occ::SolidMaker::makeBox(10, 10, 10);
    Occ::Cylinder myCyl = Occ::SolidMaker::makeCylinder(2.5, 10);
    Occ::BooleanSolid myFuse = Occ::SolidModifier::makeFusion(myBox, myCyl);
    CompoundSolidManager mgr(myFuse);
    uint i = mgr.getFaceIndex(myFuse.getFaces()[3]);
    EXPECT_GE(i, 0);
}

TEST(CompoundSolidManager, updateSolid)
{
    // Create the original fusion
    Occ::Box myBox = Occ::SolidMaker::makeBox(10, 10, 10);
    Occ::Cylinder myCyl = Occ::SolidMaker::makeCylinder(2.5, 10);
    Occ::BooleanSolid myFuse = Occ::SolidModifier::makeFusion(myBox, myCyl);
    CompoundSolidManager mgr(myFuse);

    // Get a constant reference. We know from testing that Edge[9] is the one between the
    // cubic "top" and cubic "front" on the fused solid
    uint i = mgr.getEdgeIndex(mgr.getSolid().getEdges()[9]);

    // Create the updated fusion
    Occ::Cylinder myCyl2 = Occ::SolidMaker::makeCylinder(2.5, 5);
    Occ::BooleanSolid myUpdatedFuse = Occ::SolidModifier::makeFusion(myBox, myCyl2);

    // Prepare the information to update our CompoundSolidManager
    Occ::ModifiedSolid cylMod(myCyl, myCyl2);

    // update our CompoundSolidManager
    mgr.updateSolid(myUpdatedFuse, {cylMod});

    // retrieve our desired Edge, as well as the original (for comparison).
    const Occ::Edge& originalEdge = myFuse.getEdges()[9];
    const Occ::Edge& retreivedEdge = mgr.getEdgeByIndex(i);

    EXPECT_TRUE(originalEdge.overlaps(retreivedEdge));
    EXPECT_NE(originalEdge, retreivedEdge);
}

TEST(CompoundSolidManager, updateSolidInvalidModifiedSolid)
{
    // Create the original fusion
    Occ::Box myBox = Occ::SolidMaker::makeBox(10, 10, 10);
    Occ::Cylinder myCyl = Occ::SolidMaker::makeCylinder(2.5, 10);
    Occ::BooleanSolid myFuse1 = Occ::SolidModifier::makeFusion(myBox, myCyl);
    CompoundSolidManager mgr(myFuse1);

    // Create the updated fusion
    Occ::Cylinder myCyl2 = Occ::SolidMaker::makeCylinder(2.5, 5);
    Occ::BooleanSolid myFuse2 = Occ::SolidModifier::makeFusion(myBox, myCyl2);

    // Prepare incorrect information for updateSolid
    Occ::Cylinder myCyl3 = Occ::SolidMaker::makeCylinder(2.5, 10);
    Occ::ModifiedSolid cylMod(myCyl3, myCyl2); // myCyl3 is not in myFuse1

    EXPECT_THROW(mgr.updateSolid(myFuse2, {cylMod}), std::runtime_error);
}
