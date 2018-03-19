#include <PrimitiveSolidManager.h>
#include <OccModifiedSolid.h>
#include <OccFaceComparator.h>

#include <utility>
#include <vector>
#include <iostream>

using std::vector;

PrimitiveSolidManager::PrimitiveSolidManager(Occ::Solid aSolid)
    : ISolidManager(aSolid), mySolid(aSolid)
{
    for (uint i = 0; i < mySolid.getFaces().size(); i++)
    {
        mappedFaces[mappedFaces.size()] = {i};
    }
    ISolidManager::mapEdges();
}

PrimitiveSolidManager::PrimitiveSolidManager(const PrimitiveSolidManager& aManager)
    : ISolidManager(aManager.mySolid), mySolid(aManager.mySolid), mappedFaces(aManager.mappedFaces)
{
    ISolidManager::mapEdges();
}

PrimitiveSolidManager PrimitiveSolidManager::operator=(const PrimitiveSolidManager& aManager)
{
    ISolidManager::myFirstSolid = aManager.myFirstSolid;
    this->mySolid = aManager.mySolid;
    this->mappedFaces = aManager.mappedFaces;
    ISolidManager::mapEdges();
    return *this;
}

uint PrimitiveSolidManager::getFaceIndex(const Occ::Face& aFace) const
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
