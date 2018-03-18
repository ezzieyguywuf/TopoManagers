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
    Occ::Box myBox = Occ::SolidMaker::makeBox(10, 10, 10);
    Occ::Cylinder myCyl = Occ::SolidMaker::makeCylinder(2.5, 10);
    Occ::BooleanSolid myFuse = Occ::SolidModifier::makeFusion(myBox, myCyl);
    CompoundSolidManager mgr(myFuse);
    uint i = mgr.getEdgeIndex(myFuse.getEdges()[9]);
    std::cout << "i = " << i << std::endl;

    Occ::Cylinder myCyl2 = Occ::SolidMaker::makeCylinder(2.5, 5);
    Occ::BooleanSolid myFuse2 = Occ::SolidModifier::makeFusion(myBox, myCyl2);
    Occ::ModifiedSolid cylMod(myCyl, myCyl2);
    mgr.updateSolid(myFuse2, {cylMod});

    return 0;
}
