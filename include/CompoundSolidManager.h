#ifndef CompoundSOLIDMANAGER_H
#define CompoundSOLIDMANAGER_H

#include <ISolidManager.h>
#include <OccSolid.h>
#include <OccFace.h>
#include <OccEdge.h>
#include <OccBooleanSolid.h>

#include <map>
#include <utility>
#include <vector>
#include <array>

using std::map;
using std::pair;
using std::vector;
using std::array;
using uint = unsigned int;

class CompoundSolidManager : public ISolidManager
{
    public:
        CompoundSolidManager(Occ::BooleanSolid someSolids);

        //rule of three
        CompoundSolidManager(const CompoundSolidManager& aManager);
        CompoundSolidManager operator=(const CompoundSolidManager& aManager);
        ~CompoundSolidManager() = default;


        // each someSolid must contain a someSolid.getOrigSolid() that corresponds to one
        // of myBases[n].getNewSolid().
        //
        // throws std::runtime_error if someSolid.getOrigSolid() does not correspond to
        // any of myBases[n].getNewSolid()
        void updateSolid(Occ::BooleanSolid someSolids);
        uint getFaceIndex(const Occ::Face& aFace) const override;
        // note: we can always take the zeroeth return because for CompoundSolidManager,
        // the return value of getFaceByIndex is always a single face.
        //
        // TODO: is this true????
        vector<Occ::Face> getFaceByIndex(uint i) const override;
        const Occ::Solid& getSolid() const override;
        void translate(double dx, double dy, double dz) override;

        // When we update our solid, we need two things:
        //     1) The new Occ::BooleanSolid
        //     2) A list of Occ::ModifiedSolid, one for each base solid in the original
        //        Occ::BooleanSolid 
        //
        //  Take for example a simple cube and cylinder that have been fused. Our original
        //  mySolid may consist of a cube with edge length of 10 and a cylinder of radius
        //  2.5 and height 10.
        //
        //  When updateSolid is called, the new fused shape has the same cube but a
        //  cylinder of height 5. Therefore, updateSolid will expect a single
        //  Occ::ModifiedSolid that gives the mapping of the faces from the original
        //  cylinder to the new shorter cylinder. Since tho cube hasn't changed, no
        //  Occ::ModifiedSolid is needed for it
        void updateSolid(Occ::BooleanSolid newSolid, vector<Occ::ModifiedSolid> modifiedBaseSolids);
    private:
        // helper method used by updateSolid
        void updateMappedFaces(uint i, const Occ::ModifiedSolid& newModSolid);

        Occ::BooleanSolid mySolid;
        // The unique ID for each face in mySolid will consist of a set of three values
        // (i,j,k). Please check the documentation for the getConstituentFace() method if
        // Occ::BooleanSolid for more information.
        //
        // TODO: change to map<uint, map<uint, pair<uint, uint>>> where in 
        // map<i, map<j, pair<k,l>>>, i = the constant reference, j = which modified solid
        // in mySolid, k is the origFace index and l is the newFace index.
        map<uint, array<uint, 3>> mappedFaces;
};

#endif /* CompoundSOLIDMANAGER_H */
