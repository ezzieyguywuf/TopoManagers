#ifndef PRIMITIVESOLIDMANAGER_H
#define PRIMITIVESOLIDMANAGER_H

#include <OccSolid.h>
#include <OccFace.h>
#include <OccEdge.h>
#include <OccTypes.h>

#include <map>
#include <utility>
#include <vector>

using std::map;
using std::pair;
using std::vector;
using Occ::uint;
using Occ::uints;

class PrimitiveSolidManager
{
    public:
        PrimitiveSolidManager() = default;
        PrimitiveSolidManager(Occ::Solid aSolid);
        uint getEdgeIndex(const Occ::Edge anEdge) const;
        uint getFaceIndex(const Occ::Face aFace) const;
        Occ::Edge getEdgeByIndex(uint i) const;
        vector<Occ::Face> getFaceByIndex(uint i) const;
        //bool hasEdge(uint i) const;
        //bool hasFace(uint i) const;

        // Note: need to extend to deal with deleted and generated faces. Right now only
        // deals with modified faces.
        void updateSolid(const Occ::ModifiedSolid& aModifiedSolid);
        const Occ::Solid& getSolid() const;

    private:
        Occ::Solid mySolid;
        // a map in which the key (which is unique) can be used to consistently refer to a
        // topological face in mySolid. The value associated with each key points to a
        // vector of indices. These indices  represent the actual mySolid.getFaces() that
        // the key refers to. We store a vector of indices rather than a single index
        // because a given face could be split.
        map<uint, vector<uint>> mappedFaces;
        // A map in which the key will always refer to the same edge in
        // mySolid.getEdges(). This is done by identifying each edge by the two faces that
        // make it up. Therefore, in order to identify an edge, we simply keep track of
        // each face.
        map<uint, pair<uint, uint>> mappedEdges;
};

#endif /* PRIMITIVESOLIDMANAGER_H */
