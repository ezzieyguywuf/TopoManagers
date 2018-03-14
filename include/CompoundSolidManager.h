#ifndef CompoundSOLIDMANAGER_H
#define CompoundSOLIDMANAGER_H

#include <OccSolid.h>
#include <OccFace.h>
#include <OccEdge.h>
#include <OccBooleanSolid.h>

#include <map>
#include <utility>
#include <vector>

using std::map;
using std::pair;
using std::vector;
using uint = unsigned int;

class CompoundSolidManager
{
    // For the purposes of this algorithm, a "Compound Solid" can be defined as a solid
    // which is somehow created from two (or more) other solids. Therefore, in order to
    // create a CompoundSolidManager, some critical pieces of information are needed:
    //
    //     1) The Occ::Solid which is being managed (similar to PrimitiveSolidManager)
    //     2) A listing of all the constituent solids which are used to create the solid
    //        being managed
    //     3) For each constituent solid, a mapping of all the modified, deleted, and
    //        generated faces _from_ the constituent solid _to_ the managed solid
    //
    //  This information can be used to fully describe the managed solid. Lets take a
    //  simple example of a cube and a cylinder. Let's assume the cube is of length 10.0
    //  and the cylinder has a radius of 2.5, a height of 15. The axis of the cylinder is
    //  coincident with a corner of the cube. These two solids are "fused" into one solid.
    //
    //  Now, let's name all of our faces. This is simple to do on the cube and cylinder.
    //  Each has a "top" and "bottom" face. The cylinder has a "lateral" (circular) face.
    //  And finally, the cube has a "left", "right", "front" and "back" face.
    //
    //  (don't worry, there won't be a test)
    //
    //  After the fusion, we can map the modified faces as follows:
    //
    //          cube "top"    -> fused cubic top
    //          cube "front"  -> fused "front"
    //          cube "left"   -> fused "left"
    //          cube "right"  -> fused "right"
    //          cube "front"  -> fused "front"
    //          cube "back"   -> fused "back"
    //          cyl "top"     -> fused cylindrical top
    //          cyl "lateral" -> fused cylindcrical lateral
    //          cyl "bottom"  -> deleted
    //
    //  phew! Pretty straight forward though, right? So, how does this knowledge help us?
    //  By knowing this mapping, we can provide a consistent reference to any face on the
    //  fused Solid.
    //
    //  This is accomplished by keeping a record of which constituent shape, and which
    //  face on that constituent shape, created the face on the Fused solid in question.
    //
    //  For example, if a user wishes to obtain a reference to te "fused front" face, all
    //  we have to do is remember that "fused front was created by cube front".
    //
    //  Now, what happens when we make the cylinder a height of 5? Intuitively we know
    //  that the "top", "left", and "back" faces of the cube are modified, as i sthe "top"
    //  and "lateral" face of the cylinder. Just like in PrimitiveSolidManager, we'll need
    //  the user to invoke CompoundSolidManager::updateSolid in order to have enough
    //  information to keep track of things. The information we'll need is:
    //      
    //      1) The newSolid (which is a modification of the current solid being managed)
    //      2) An OccModifiedSolid for each constituent solid. each
    //         aModifiedSolid.getOrigSolid() _must_ correspond to one of our orignial
    //         constituent solids. Each aModifiedSolid.getNewSolid() _must_ correspond to
    //         one of the constituent solids of newSolid.
    //      3) A map of all the modified, deleted, and generated faces for each new
    //         constituent solid.
    //
    //  In our example, the data passed to updateSolid would be:
    //
    //      1) The new fused solid.
    //      2) One modified solid. The origSolid will be the cylinder with a height of 15.
    //         The newSolid will be the cylinder with a height of 5.
    //      3) The map will show
    //
    //          cube "top"    -> fused "top"
    //          cube "front"  -> fused "front"
    //          cube "left"   -> fused "left"
    //          cube "right"  -> fused "right"
    //          cube "front"  -> fused "front"
    //          cube "back"   -> fused "back"
    //          cyl "top"     -> fused cylindrical top
    //          cyl "lateral" -> fused cylindrical lateral
    //          cyl "bottom"  -> deleted
    //
    //  Look familiar? That's because it's the same as before. Despite these mappings
    //  being "topologicially" equivalent, though, they are geometrically different.
    //  That's why we need this updated info.
    //
    //  Finally, we use this updated info to update our original mapping.
    //  try again.
    //
    //  For each constituent solid, we will create a PrimitiveSolidManager. Any time the
    //  managed solid changes, the user must invoke updateSolid with an
    //  Occ::ModifiedSolids. With this, along with our original Occ::ModifiedSolids, we'll
    //  know the following information:
    //
    //       origConstituentSolid1_face1 -> origCompoundSolidFaceN
    //       ...etc...
    //
    //       newConstituentSolid1_face1 -> newCompoundSolidFaceN
    //       etc...
    //      
    //  In order to tie it all together, each Occ::ModifiedSolid::getOrigShape() _must_
    //  correspond to one of the original constituent shapes. In this manner, a mapping of
    //  the following sort can be done:
    //
    //       origContituentSolid1_face1 -> newConstituentSolid1_face1
    //       ...etc...
    //
    //  This last bit gives us what we need to glue it all together. So, if originally I
    //  knew that "face1" on my managed solid came from "faceA" on constituent solid 1, I
    //  now know that "newFace1" comes from "newFaceA" still on constituent solid 1.
    public:
        CompoundSolidManager(Occ::BooleanSolid someSolids);
        // each someSolid must contain a someSolid.getOrigSolid() that corresponds to one
        // of myBases[n].getNewSolid().
        //
        // throws std::runtime_error if someSolid.getOrigSolid() does not correspond to
        // any of myBases[n].getNewSolid()
        void updateSolid(Occ::BooleanSolid someSolids);
        uint getFaceIndex(const Occ::Face& aFace) const;
        const Occ::Face& getFaceByIndex(uint i) const;
        const Occ::Solid& getSolid() const;

    private:
        OccBooleanSolid mySolid;
        // This map<i, map<j, k>> represents:
        // i = an index which can be used to return a face in this->getSolid().getFaces().
        //     This index will always return the same topological face. This is
        //     accomplished by updated the pair <j, k> associated with i any time myBases
        //     changes.
        // j = an index representing which ModifiedSolid (i.e. aBase) in myBases contains
        //     the face in aBase.getOrigSolid().getFaces() which corresponds to
        //     the face pointed to by i
        // k = an index representing which face in myBases[j].getorigSolid().getFaces() is
        //     the bases for tho face pointed to by i
        map<uint, pair<uint, uint>> mappedFaces;
        // A map in which the key will always refer to the same edge in
        // mySolid.getEdges(). This is done by identifying each edge by the two faces that
        // make it up. Therefore, in order to identify an edge, we simply keep track of
        // each face.
        map<uint, pair<uint, uint>> mappedEdges;
};

#endif /* CompoundSOLIDMANAGER_H */
