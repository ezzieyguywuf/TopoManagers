#include <PrimitiveSolidManager.h>
#include <OccModifiedSolid.h>
#include <OccFaceComparator.h>

#include <utility>
#include <vector>
#include <iostream>

using std::vector;

PrimitiveSolidManager::PrimitiveSolidManager(Occ::Solid aSolid)
    : mySolid(aSolid)
{
    for (uint i = 0; i < mySolid.getFaces().size(); i++)
    {
        mappedFaces[mappedFaces.size()] = {i};
    }

    vector<uint> foundFaces;
    for (uint i = 0; i < mySolid.getEdges().size(); i++)
    {
        foundFaces.clear();
        const Occ::Edge& myEdge = mySolid.getEdges()[i];
        for (const auto& data : mappedFaces)
        {
            const Occ::Face& checkFace = mySolid.getFaces()[data.first];
            for (uint j = 0; j < checkFace.getEdges().size() ; j++)
            {
                const Occ::Edge& checkEdge = checkFace.getEdges()[j];
                if (myEdge.isSimilar(checkEdge))
                {
                    foundFaces.push_back(data.first);
                }
            }
        }

        if (foundFaces.size() == 0)
        {
            throw std::runtime_error("Did not find two faces for this Edge");
        }
        else if (foundFaces.size() == 1)
        {
            // TODO: do something with seam edges?
            std::clog << "Warning: Only one Edge found. Doing nothing. (is this a seam edge?)";
        }
        else if (foundFaces.size() > 2)
        {
            throw std::runtime_error("Found more than two faces that share this Edge.");
        }
        mappedEdges.emplace(mappedEdges.size() - 1, std::make_pair(foundFaces[0], foundFaces[1]));
    }
}

uint PrimitiveSolidManager::getEdgeIndex(const Occ::Edge anEdge) const
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

uint PrimitiveSolidManager::getFaceIndex(const Occ::Face aFace) const
{
    for (const auto& data : mappedFaces)
    {
        const vector<uint>& indices = data.second;
        if (indices.size() > 1)
        {
            continue;
        }
        if (aFace == mySolid.getFaces()[indices.at(0)])
        {
            return data.first;
        }
    }
    throw std::runtime_error("That face is not not in mySolid");
}

Occ::Edge PrimitiveSolidManager::getEdgeByIndex(uint i) const
{
    const Occ::Face& face1 = mySolid.getFaces()[mappedEdges.at(i).first];
    const Occ::Face& face2 = mySolid.getFaces()[mappedEdges.at(i).second];
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

vector<Occ::Face> PrimitiveSolidManager::getFaceByIndex(uint i) const
{
    vector<Occ::Face> outFaces;
    for (uint index : mappedFaces.at(i))
    {
        outFaces.push_back(mySolid.getFaces()[index]);
    }
    return outFaces;
}

//bool PrimitiveSolidManager::hasEdge(uint i) const
//{
    //return mappedEdges.count(i);
//}

//bool PrimitiveSolidManager::hasFace(uint i) const
//{
    //return mappedFaces.count(i);
//}

void PrimitiveSolidManager::updateSolid(const Occ::ModifiedSolid& aModifiedSolid)
{
    if (mySolid != aModifiedSolid.getOrigSolid())
    {
        throw std::runtime_error("This ModifiedSolid does not appear to modify mySolid!");
    }

    // we'll add these to the end of our map
    vector<uint> toAdd;
    for (auto& pair : mappedFaces)
    {
        const Occ::Solid& origSolid = aModifiedSolid.getOrigSolid();
        const Occ::Face& origFace = origSolid.getFaces()[pair.first];
        if (aModifiedSolid.isDeleted(origFace))
        {
            // when deleted, we point to an empty vector.
            pair.second = {};
        }
        else
        {
            // first, update our map with the new vector
            pair.second = aModifiedSolid.getModifiedFaceIndices(origFace);

            // next, store values to add to our map
            if (pair.second.size() > 1)
            {
                // if the face was split, create a new entry in our mappedFace map for
                // each split face. Why? Imagine a User has index 0 representing the Top
                // face of a box. now, that face is split into Top1 and Top2. index 0 will
                // point to {Top1, Top2}. Great. But what if the user now wishes to obtain
                // a reference to only Top1? That's why we make new entries.
                toAdd.reserve(pair.second.size()); // preallocate memory
                toAdd.insert(toAdd.end(), pair.second.begin(), pair.second.end());
            }
        }
    }

    for (uint i : toAdd)
    {
        mappedFaces.emplace(mappedFaces.size(), i);
    }

    for (const uint& i : aModifiedSolid.getNewFaceIndices())
    {
        mappedFaces.emplace(mappedFaces.size(), i);
    }

    mySolid = aModifiedSolid.getNewSolid();
}

const Occ::Solid& PrimitiveSolidManager::getSolid() const
{
    return mySolid;
}
