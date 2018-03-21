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
