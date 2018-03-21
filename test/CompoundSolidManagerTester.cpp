#include <CompoundSolidManager.h>
#include <OccSolidMaker.h>
#include <OccSolidModifier.h>
#include <OccModifiedSolid.h>
#include <OccBooleanSolid.h>

#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Cut.hxx>

#include "gtest/gtest.h"

#include <stdexcept>

TEST(CompoundSolidManager, Create){
    Occ::Box myBox = Occ::SolidMaker::makeBox(10, 10, 10);
    Occ::Cylinder myCyl = Occ::SolidMaker::makeCylinder(2.5, 10);
    BRepAlgoAPI_Fuse mkFuse(myBox.getShape(), myCyl.getShape());
    Occ::BooleanSolid myFuse = Occ::SolidMaker::makeBoolean(mkFuse);
    CompoundSolidManager mgr(myFuse);
    uint i = mgr.getFaceIndex(myFuse.getFaces()[3]);
    EXPECT_GE(i, 0);
}

TEST(CompoundSolidManager, updateSolidWithBooleanFuse)
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
    vector<Occ::Edge> retreivedEdges = mgr.getEdgeByIndex(i);
    const Occ::Edge& retreivedEdge = retreivedEdges[0];

    EXPECT_TRUE(originalEdge.overlaps(retreivedEdge));
    EXPECT_EQ(retreivedEdges.size(), 1);
}

TEST(CompoundSolidManager, updateSolidWithBooleanCutTranslated)
{
    //// Create the original fusion
    //Occ::Box myBox1 = Occ::SolidMaker::makeBox(10, 10, 10);
    //Occ::Box myBox2 = Occ::SolidMaker::makeBox(10, 10, 10);
    //Occ::BooleanSolid myFuse = Occ::SolidModifier::makeFusion(myBox1, myCyl);
    //CompoundSolidManager mgr(myFuse);

    //// Get a constant reference. We know from testing that Edge[9] is the one between the
    //// cubic "top" and cubic "front" on the fused solid
    //uint i = mgr.getEdgeIndex(mgr.getSolid().getEdges()[9]);

    //// Create the updated fusion
    //Occ::Cylinder myCyl2 = Occ::SolidMaker::makeCylinder(2.5, 5);
    //Occ::BooleanSolid myUpdatedFuse = Occ::SolidModifier::makeFusion(myBox, myCyl2);

    //// Prepare the information to update our CompoundSolidManager
    //Occ::ModifiedSolid cylMod(myCyl, myCyl2);

    //// update our CompoundSolidManager
    //mgr.updateSolid(myUpdatedFuse, {cylMod});

    //// retrieve our desired Edge, as well as the original (for comparison).
    //const Occ::Edge& originalEdge = myFuse.getEdges()[9];
    //vector<Occ::Edge> retreivedEdges = mgr.getEdgeByIndex(i);
    //const Occ::Edge& retreivedEdge = retreivedEdges[0];

    //EXPECT_TRUE(originalEdge.overlaps(retreivedEdge));
    //EXPECT_EQ(retreivedEdges.size(), 1);
}

TEST(CompoundSolidManager, updateSolidWithBooleanCut)
{
    // Create the original fusion
    Occ::Box myBox = Occ::SolidMaker::makeBox(10, 10, 10);
    Occ::Cylinder myCyl = Occ::SolidMaker::makeCylinder(2.5, 10);
    BRepAlgoAPI_Cut mkCut(myBox.getShape(), myCyl.getShape());
    Occ::BooleanSolid myCut = Occ::SolidMaker::makeBoolean(mkCut);
    CompoundSolidManager mgr(myCut);

    // Get a constant reference. We know from testing that Edge[5] is the one between the
    // cubic "top" and cubic "front" on the Cutd solid
    uint i = mgr.getEdgeIndex(mgr.getSolid().getEdges()[5]);

    // Create the updated fusion
    Occ::Cylinder myCyl2 = Occ::SolidMaker::makeCylinder(2.5, 5);
    BRepAlgoAPI_Cut mkCut2(myBox.getShape(), myCyl2.getShape());
    Occ::BooleanSolid myUpdatedCut = Occ::SolidMaker::makeBoolean(mkCut2);

    // Prepare the information to update our CompoundSolidManager
    Occ::ModifiedSolid cylMod(myCyl, myCyl2);

    // update our CompoundSolidManager
    mgr.updateSolid(myUpdatedCut, {cylMod});

    // retrieve our desired Edge, as well as the original (for comparison).
    const Occ::Edge& originalEdge = myCut.getEdges()[5];
    vector<Occ::Edge> retreivedEdges = mgr.getEdgeByIndex(i);
    const Occ::Edge& retreivedEdge = retreivedEdges[0];

    EXPECT_TRUE(originalEdge.overlaps(retreivedEdge));
    EXPECT_EQ(retreivedEdges.size(), 1);

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

TEST(CompoundSolidManager, updateSolidMisMatchedModifiedSolid)
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
    Occ::ModifiedSolid cylMod(myCyl, myCyl3); // myCyl3 is not in myFuse2

    EXPECT_THROW(mgr.updateSolid(myFuse2, {cylMod}), std::runtime_error);
}
