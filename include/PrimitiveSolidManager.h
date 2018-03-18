#ifndef PRIMITIVESOLIDMANAGER_H
#define PRIMITIVESOLIDMANAGER_H

#include <ISolidManager.h>

class PrimitiveSolidManager : public ISolidManager
{
    public:
        PrimitiveSolidManager() = default;
        PrimitiveSolidManager(Occ::Solid aSolid);
        uint getFaceIndex(const Occ::Face& aFace) const override;
        vector<Occ::Face> getFaceByIndex(uint i) const override;

        // rule of three
        PrimitiveSolidManager(const PrimitiveSolidManager& aManager);
        PrimitiveSolidManager operator=(const PrimitiveSolidManager& aManager);
        ~PrimitiveSolidManager() = default;

        // Note: need to extend to deal with deleted and generated faces. Right now only
        // deals with modified faces.
        void updateSolid(const Occ::ModifiedSolid& aModifiedSolid);
        // Creates a Occ::ModifiedSolid that explains the differences between mySolid and
        // aManager.mySolid
        //
        // throws std::runtime_error if aManager.myFirstSolid and myFirstSolid are not
        // equivalent.
        const Occ::ModifiedSolid makeModifiedSolid(const PrimitiveSolidManager& aManager) const;
        const Occ::Solid& getSolid() const override;

    private:
        // Used to check validity of parameter in getModifiedSolid
        Occ::Solid myFirstSolid;
        Occ::Solid mySolid;
        // a map in which the key (which is unique) can be used to consistently refer to a
        // topological face in mySolid. The value associated with each key points to a
        // vector of indices. These indices  represent the actual mySolid.getFaces() that
        // the key refers to. We store a vector of indices rather than a single index
        // because a given face could be split.
        map<uint, vector<uint>> mappedFaces;
};

#endif /* PRIMITIVESOLIDMANAGER_H */
