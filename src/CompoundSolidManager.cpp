#include <CompoundSolidManager.h>

#include <stdexcept>

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
    : ISolidManager(aManager.mySolid), mySolid(aManager.mySolid), mappedFaces(aManager.mappedFaces)
{
    ISolidManager::mapEdges();
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
        Occ::Face checkFace = this->getFaceByIndex(data.first)[0];
        if (aFace == checkFace)
        {
            return data.first;
        }
    }
    throw std::runtime_error("That face does not appear to originate from any of mySolid.getModifiedSolids()");
}

vector<Occ::Face> CompoundSolidManager::getFaceByIndex(uint i) const
{
    array<uint, 3> origMap = mappedFaces.at(i);
    const Occ::ModifiedSolid& modSolid = mySolid.getModifiedSolids()[origMap[0]];
    const Occ::Face& origFace = modSolid.getOrigSolid().getFaces()[origMap[1]];
    uint index = modSolid.getModifiedFaceIndices(origFace)[origMap[2]];
    return {modSolid.getNewSolid().getFaces()[index]};
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
            // find out what the new constiuent face(s) is/are
            vector<uint> newConstituentFaceIndices = newModSolid.getModifiedFaceIndices(origConstituentFace);
            if (newConstituentFaceIndices.size() > 1)
            {
                std::clog << "Constituent face has been split. Only taking the first value. TODO: be better.";
            }
            indices[1] = newConstituentFaceIndices[0];
            break;
        }
    }
}
