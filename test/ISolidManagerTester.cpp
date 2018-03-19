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

    EXPECT_THROW(ISolidManager::makeModifiedSolid(mgr1, myBox5), std::runtime_error);
    EXPECT_THROW(ISolidManager::makeModifiedSolid(mgr2, myBox4), std::runtime_error);
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
