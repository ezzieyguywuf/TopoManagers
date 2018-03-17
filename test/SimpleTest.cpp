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
    //uint i = mgr.getFaceIndex(myBox.getNamedFace(Occ::FaceName::top));
    //std::cout << "i = " << i << std::endl;
    std::cout << mgr.getSolid().getFaces().size() << std::endl;

    return 0;
}
