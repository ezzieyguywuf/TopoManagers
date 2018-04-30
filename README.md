# TopoManagers

### How to compile

```bash
git clone --recurse-subprojects https://github.com/ezzieyguywuf/TopoManagers
cd TopoManagers
mkdir build
cd build
make -j8 all
make test # if you want to run the tests
```

### What is it?

TopoManagers is a library which provides managers for topology. These managers handle
various types of specialized solids. The interface for this management can be seen in the
`ISolidManager` abstract base clase. In short, a solid manager will:

* manage an Occ::Solid (see [https://github.com/ezzieyguywuf/OccWrapper](OccWrapper))
* provide an `updateSolid` method, which allows the user the change the managed solid to a
  new `Occ::Solid`

Please note that the `updateSolid` method is _not_ defined in tho `ISolidManager` abstract
base class because its call signature can and will vary based on the specific use-case of
the solid manager.

### Why?

This project exists in order to deal with the pernicious "Topological Naming Issue" as
defined by the FreeCAD community. More information can be seen 
[https://forum.freecadweb.org/viewtopic.php?f=10&t=27278<Paste>](here). In
short, due to the way that topological entities are currently referenced in
FreeCAD, there are situations in which a user will expect to have a constant
reference to a particular entity when in fact this is not the case.

For an example of this, perform the following steps in FreeCAD:

* Create a new document
* Swich to the `Part` workbench
* Create a `Box` using the GUI button
* Create a `Cylinder` using the GUI button
* Fuse these two items together
* Fillet any edge on the Fused solid
* Finally, change the cylinder's height to 5 and 15

Notice that when you change the height of the cylinder, the filleted edge
moves. The reason for this is discused in detail on the FreeCAD developper
forums.

## Project Status

The project is in its infancy. It is barely started. At this point, it is mostly a
proof-of-concept to show how this general idea for managing topological entities can work.

## TODO
* Add `updateSolid` to the `ISolidManager` abstract base class somehow
* Create a manager for 2d sketches, `PlanarFaceSolidManager`
* Create a manager for extruded 2d sketches, `SweptSolidManager`
* more....
