#include <CompoundSolidManager.h>

#include <stdexcept>

CompoundSolidManager::CompoundSolidManager(Occ::BooleanSolid aSolid)
    : mySolid(aSolid)
{
    uint i = 0;
    for (const Occ::Face& newFace : mySolid.getFaces())
    {
        mappedFaces.emplace(i, mySolid.getConstituentFace(newFace));
        i++;
    }
    ISolidManager::mapEdges();
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
    newSolid.getFaces();
    modifiedBaseSolids[0];
}
