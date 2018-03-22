#include <CompoundSolidManager.h>

#include <stdexcept>
#include <string>

CompoundSolidManager::CompoundSolidManager(Occ::BooleanSolid aSolid)
    : ISolidManager(aSolid), mySolid(aSolid)
{
    uint i = 0;
    for (const Occ::Face& newFace : mySolid.getFaces())
    {
        mappedFaces.emplace(i, mySolid.getConstituentFace(newFace));
        i++;
    }
    ISolidManager::mapEdges();
}

CompoundSolidManager::CompoundSolidManager(const CompoundSolidManager& aManager)
    : ISolidManager(aManager.myFirstSolid, aManager.mappedEdges), 
      mySolid(aManager.mySolid), 
      mappedFaces(aManager.mappedFaces)
{
}

CompoundSolidManager CompoundSolidManager::operator=(const CompoundSolidManager& aManager)
{
    ISolidManager::myFirstSolid = aManager.myFirstSolid;
    mySolid = aManager.mySolid;
    mappedFaces = aManager.mappedFaces;
    ISolidManager::mapEdges();
    return *this;
}

uint CompoundSolidManager::getFaceIndex(const Occ::Face& aFace) const
{
    for (const auto& data : mappedFaces)
    {
        vector<Occ::Face> checkFaces = this->getFaceByIndex(data.first);
        for (const Occ::Face& checkFace : checkFaces)
        {
            if (aFace.isSimilar(checkFace))
            {
                return data.first;
            }
        }
    }
    throw std::runtime_error("That face does not appear to originate from any of mySolid.getModifiedSolids()");
}

vector<Occ::Face> CompoundSolidManager::getFaceByIndex(uint i) const
{
    // get orig face
    array<uint, 3> origMap = mappedFaces.at(i);
    const Occ::ModifiedSolid& modSolid = mySolid.getModifiedSolids()[origMap[0]];
    const Occ::Face& origFace = modSolid.getOrigSolid().getFaces()[origMap[1]];

    // create return value vector
    vector<Occ::Face> outFaces;

    // check for modified faces
    vector<uint> modFaces = modSolid.getModifiedFaceIndices(origFace);
    for (const uint& i : modFaces)
    {
        outFaces.push_back(modSolid.getNewSolid().getFaces()[i]);
    }

    if (outFaces.size() > 0)
    {
        return outFaces;
    }

    vector<uint> newFaces = modSolid.getNewFaceIndices(origFace);
    for (const uint& i : newFaces)
    {
        outFaces.push_back(modSolid.getNewSolid().getFaces()[i]);
    }

    if (outFaces.size() == 0)
    {
        throw std::runtime_error("That face did not get modfied nor did it generate a new face.");
    }

    return outFaces;
}

const Occ::Solid& CompoundSolidManager::getSolid() const
{
    return mySolid;
}

void CompoundSolidManager::updateSolid(Occ::BooleanSolid newSolid, 
                                       vector<Occ::ModifiedSolid> modifiedBaseSolids)
{
    for (const Occ::ModifiedSolid& modSolid : modifiedBaseSolids)
    {
        bool found = false;
        uint i = 0;
        for (const Occ::ModifiedSolid& myBaseSolid : mySolid.getModifiedSolids())
        {
            if (modSolid.getOrigSolid() == myBaseSolid.getOrigSolid())
            {
                found = true;
                this->updateMappedFaces(i, modSolid);
            }
            i++;
        }
        if (not found)
        {
            throw std::runtime_error("This Occ::ModifiedSolid does not correspond to any of my base Occ::ModifiedSolids.");
        }

        found = false;
        for (const Occ::ModifiedSolid newBaseSolid : newSolid.getModifiedSolids())
        {
            if (newBaseSolid.getOrigSolid() == modSolid.getNewSolid())
            {
                found = true;
            }
        }
        if (not found)
        {
            throw std::runtime_error("This Occ::ModifiedSolid does not appear to correspond to newSolid.");
        }
    }
    mySolid = newSolid;
}

//--------------------------------------------------
//------ Private Methods ---------------------------
//--------------------------------------------------

void CompoundSolidManager::updateMappedFaces(uint i, const Occ::ModifiedSolid& newModSolid)
{
    const Occ::ModifiedSolid origModSolid = mySolid.getModifiedSolids()[i];
    for(auto& data : mappedFaces)
    {
        std::array<uint, 3>& indices = data.second;
        if (indices[0] == i)
        {
            // get the original constiutent face
            const Occ::Face origConstituentFace = origModSolid.getOrigSolid().getFaces()[indices[1]];
            if (newModSolid.isDeleted(origConstituentFace))
            {
                throw std::runtime_error("That face was deleted, it cannot be the constituent face on the newSolid.");
                return;
            }

            // If not deleted, we should be able to find out what happened. First check
            // for modifications
            vector<uint> modFaces = newModSolid.getModifiedFaceIndices(origConstituentFace);
            if (modFaces.size() > 0)
            {
                indices[1] = modFaces[indices[2]];
                return;
            }

            // Next, see if it was somehow generated from something (this shouldn't happen)
            vector<uint> newFaces = newModSolid.getNewFaceIndices(origConstituentFace);
            if (newFaces.size() > 0)
            {
                // TODO: what happens if more than one face was generated? How should we
                // handle this?!
                indices[1] = newFaces[0];
                return;
            }

            // Finally, throw an error if we don't know what happened to the face.
            throw std::runtime_error("Was unable to determine what happened to that constituent face.");
        }
    }
}
