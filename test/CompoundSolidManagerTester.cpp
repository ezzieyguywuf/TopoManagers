#include <CompoundSolidManager.h>
#include <OccSolidMaker.h>
#include <OccSolidModifier.h>
#include <OccModifiedSolid.h>
#include <OccBooleanSolid.h>

#include "gtest/gtest.h"

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
