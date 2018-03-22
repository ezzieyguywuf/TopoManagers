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
#include <BRepAlgoAPI_Cut.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopExp.hxx>
#include <BRepFeat_Builder.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>

#include <iostream>

int main(void)
{
    Occ::Box myBox1(Occ::SolidMaker::makeBox(10, 10, 10));
    Occ::Box myBox2(Occ::SolidMaker::makeBox(6, 3, 4));
    myBox2.translate(-1, 3.5, 8);

    BRepAlgoAPI_Cut mkCut(myBox1.getShape(), myBox2.getShape());
    mkCut.Build();
    //BRepFeat_Builder mkCut;
    //mkCut.Init(myBox1.getShape(), myBox2.getShape());
    //mkCut.SetOperation(0);
    //mkCut.Perform();


    Occ::Solid cutSolid(mkCut.Shape());

    myBox1.writeFile("box1.brep");
    myBox2.writeFile("box2.brep");
    cutSolid.writeFile("cutSolid.brep");

    std::cout << "using TopExp_EXplorer" << std::endl;
    TopExp_Explorer expl(mkCut.Shape2(), TopAbs_FACE);
    uint i=0;
    for(; expl.More(); expl.Next())
    {
        const TopoDS_Shape& aFace = expl.Current();
        const TopTools_ListOfShape& modified = mkCut.Modified(aFace);
        std::cout << "i = " << i << ", " << "numGenerated = " << modified.Extent() << std::endl;
        i++;
    }

    i = 0;
    Occ::Solid aSolid(mkCut.Shape2());
    aSolid.writeFile("mkCut.Shape2.brep");
    std::cout << "using remade solid" << std::endl;
    for (const Occ::Face& aFace : aSolid.getFaces())
    {
        TopTools_ListOfShape generated = mkCut.Modified(aFace.getShape());
        std::cout << "i = " << i << ", " << "numGenerated = " << generated.Extent() << std::endl;
        i++;
    }

    std::cout << "using orig solid" << std::endl;
    i = 0;
    for (const Occ::Face& aFace : myBox2.getFaces())
    {
        TopTools_ListOfShape generated = mkCut.Modified(aFace.getShape());
        std::cout << "i = " << i << ", " << "numGenerated = " << generated.Extent() << std::endl;
        i++;
    }
    
    i = 0;
    std::cout << "looping result" << std::endl;
    for (const Occ::Face& aFace : cutSolid.getFaces())
    {
        TopTools_ListOfShape generated = mkCut.Modified(aFace.getShape());
        std::cout << "i = " << i << ", " << "numGenerated = " << generated.Extent() << std::endl;
        i++;
    }
    //TopTools_ListIteratorOfListOfShape it(Modified);

    return 0;
}
