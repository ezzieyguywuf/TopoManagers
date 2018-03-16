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

//TEST(CompoundSolidManager, getEdgeByIndex)
//{
    //// create the box
    //Occ::Box myBox(Occ::SolidMaker::makeBox(10, 10, 10));
    //// create the solid manager
    //CompoundSolidManager mgr(myBox);

    //// get an Edge index (note: in a real-world application, the user would select an edge
    //// with a mouse.)
    //Occ::Face front = myBox.getNamedFace(Occ::FaceName::front);
    //Occ::Face right = myBox.getNamedFace(Occ::FaceName::right);
    //Occ::Edge frontRightEdge = front.getCommonEdge(right);
    //uint frontRightEdgeIndex = mgr.getEdgeIndex(frontRightEdge);

    //// update the managed solid
    //Occ::Box newBox = Occ::SolidMaker::makeBox(5, 5, 5);
    //Occ::ModifiedSolid modSolid(myBox, newBox);
    //mgr.updateSolid(modSolid);

    //// retrieve the Edge using our index
    //Occ::Edge retrievedFrontRightEdge = mgr.getEdgeByIndex(frontRightEdgeIndex);

    //// Now, in order to check it, build it from scratch too
    //front = newBox.getNamedFace(Occ::FaceName::front);
    //right = newBox.getNamedFace(Occ::FaceName::right);
    //Occ::Edge checkFrontRightEdge = front.getCommonEdge(right);

    //EXPECT_NE(frontRightEdge, retrievedFrontRightEdge);
    //EXPECT_EQ(checkFrontRightEdge, retrievedFrontRightEdge);
//}
