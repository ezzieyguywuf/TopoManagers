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

    private:
        const Occ::Face& getSingleFaceByIndex(uint i) const;

        Occ::BooleanSolid mySolid;
        // The unique ID for each face in mySolid will consist of a set of three values
        // (i,j,k). Please check the documentation for the getConstituentFace() method if
        // Occ::BooleanSolid for more information.
        map<uint, array<uint, 3>> mappedFaces;
};

#endif /* CompoundSOLIDMANAGER_H */
