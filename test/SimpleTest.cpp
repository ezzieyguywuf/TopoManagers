#include <PrimitiveSolidManager.h>
#include <CompoundSolidManager.h>
#include <OccSolidMaker.h>
#include <OccSolidModifier.h>
#include <OccBooleanSolid.h>
#include <OccShape.h>
#include <OccBox.h>
#include <OccCylinder.h>
#include <OccEdge.h>
#include <TopoDS_Shape.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>

#include <iostream>

int main(void)
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
    mgr1.updateSolid(mod2);
    mgr1.updateSolid(mod3);

    mgr2.updateSolid(mod1);

    // this will be the expected output
    Occ::ModifiedSolid genVal = PrimitiveSolidManager::makeModifiedSolid(mgr1, mgr2);
    const Occ::ModifiedSolid checkVal(myBox2, myBox4);
    std::cout << "check?" << (genVal == checkVal) << std::endl;
    return 0;
}
