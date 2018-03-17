#ifndef ISOLIDMANAGER_H
#define ISOLIDMANAGER_H

#include <OccSolid.h>
#include <OccFace.h>
#include <OccEdge.h>

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
        inline virtual ~ISolidManager(){};

        // returns an index that can  be used to consistently retrieve a topological Edge
        inline uint getEdgeIndex(const Occ::Edge& anEdge) const;
        // consistently returns the topological edge refered to by i
        inline const Occ::Edge& getEdgeByIndex(uint i) const;
        // returns an index that can be used to consistently retrieve a topological Face
        virtual uint getFaceIndex(const Occ::Face aFace) const = 0;
        // consistently returns the topological face refered to by i
        virtual vector<Occ::Face> getFaceByIndex(uint i) const = 0;

        // Updates the managed solid from a single modified solid
        virtual void updateSolid(const Occ::ModifiedSolid& aModifiedSolid) = 0;

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

uint ISolidManager::getEdgeIndex(const Occ::Edge& anEdge) const
{
    for (const auto& data : mappedEdges)
    {
        if (anEdge.isSimilar(this->getEdgeByIndex(data.first)))
        {
            return data.first;
        }
    }
    throw std::runtime_error("Was unable to find anEdge in mySolid.");
}

const Occ::Edge& ISolidManager::getEdgeByIndex(uint i) const
{
    const Occ::Face& face1 = this->getSolid().getFaces()[mappedEdges.at(i).first];
    const Occ::Face& face2 = this->getSolid().getFaces()[mappedEdges.at(i).second];
    for (const auto& edge1 : face1.getEdges())
    {
        for (const auto& edge2 : face2.getEdges())
        {
            if (edge1.isSimilar(edge2))
            {
                return edge1;
            }
        }
    }
    throw std::runtime_error("That edge does not appear to be in mySolid");
}

void ISolidManager::mapEdges()

{
    const Occ::Solid& theSolid = this->getSolid();
    uint count;
    for (const Occ::Edge& checkEdge : theSolid.getEdges())
    {
        count = 0;
        for (const Occ::Face& face1 : theSolid.getFaces())
        {
            if (not face1.containsEdge(checkEdge))
            {
                continue;
            }
            count++;
            for (const Occ::Face& face2 : theSolid.getFaces())
            {
                if (face1 == face2)
                {
                    continue;
                }
                if (face2.containsEdge(checkEdge))
                {
                    count++;
                    uint i1 = this->getFaceIndex(face1);
                    uint i2 = this->getFaceIndex(face2);
                    mappedEdges.emplace(mappedEdges.size(), std::make_pair(i1, i2));
                    break;
                }
            }
        }

        if (count == 0)
        {
            throw std::runtime_error("Did not find two faces for this Edge");
        }
        else if (count == 1)
        {
            // TODO: do something with seam edges?
            std::clog << "Warning: Only one Edge found. Doing nothing. (is this a seam edge?)" << std::endl;
        }
    }
}

#endif /* ISOLIDMANAGER_H */
