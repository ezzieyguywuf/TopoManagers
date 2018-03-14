#include <PrimitiveSolidManager.h>
#include <OccSolidMaker.h>
#include <OccShape.h>
#include <OccBox.h>
#include <OccEdge.h>
#include <TopoDS_Shape.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>

#include <iostream>

int main(void)
{
    // create manager
    Occ::Box myBox(Occ::SolidMaker::makeBox(10, 10, 10));
    PrimitiveSolidManager mgr(myBox);

    Occ::Box box1 = static_cast<Occ::Box>(mgr.getSolid());
    Occ::Box box2(Occ::SolidMaker::makeBox(10, 10, 5));
    Occ::ModifiedSolid modSolid(box1, box2);

    //// find edge using OCC
    //BRepFilletAPI_MakeFillet mkFillet(myBox.getShape());
    //TopTools_IndexedMapOfShape mapOfShape;
    //TopExp::MapShapes(mgr.getSolid().getShape(), TopAbs_EDGE, mapOfShape);
    //TopoDS_Edge anEdge = TopoDS::Edge(mapOfShape.FindKey(6));

    //// get index for this Edge
    //unsigned int id = mgr.getEdgeIndex(anEdge);
    //std::cout << "retreived ID = " << id << std::endl;

    //// retrieve the Edge using the ID
    //Occ::Edge retreivedEdge = mgr.getEdgeByIndex(id);

    //// add edge to the fillet operation
    //mkFillet.Add(1.0, 1.0, TopoDS::Edge(retreivedEdge.getShape()));

    //// make the fillet.
    //mkFillet.Build();
    //mkFillet.Shape();
    return 0;
}
