#ifndef ISOLIDMANAGER_H
#define ISOLIDMANAGER_H

#include <OccSolid.h>
#include <OccFace.h>
#include <OccEdge.h>
#include <OccModifiedSolid.h>

#include <OccTypes.h>

#include <map>
#include <utility>
#include <vector>
#include <stdexcept>

using std::map;
using std::pair;
using std::vector;
using Occ::uint;
using Occ::uints;

class ISolidManager
{
    public:
        ISolidManager() = default;
        virtual ~ISolidManager(){};

        // returns an index that can  be used to consistently retrieve a topological Edge
        uint getEdgeIndex(const Occ::Edge& anEdge) const;
        // consistently returns the topological edge refered to by i
        vector<Occ::Edge> getEdgeByIndex(uint i) const;
        // returns an index that can be used to consistently retrieve a topological Face
        virtual uint getFaceIndex(const Occ::Face& aFace) const = 0;
        // consistently returns the topological face refered to by i
        virtual vector<Occ::Face> getFaceByIndex(uint i) const = 0;

        virtual const Occ::Solid& getSolid() const = 0;
    protected:
        // Note: this must be called by the inheriting class after initializing whatever
        // face mapping they need to do. mapEdges makes use of getFaceIndex
        void mapEdges();
    private:
        // A map in which the key will always refer to the same edge in
        // getSolid().getEdges(). This is done by identifying each edge by the two faces that
        // make it up. Therefore, in order to identify an edge, we simply keep track of
        // each face.
        map<uint, pair<uint, uint>> mappedEdges;
};

#endif /* ISOLIDMANAGER_H */
