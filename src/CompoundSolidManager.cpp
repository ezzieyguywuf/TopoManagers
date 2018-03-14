#include <CompoundSolidManager.h>

#include <stdexcept>

CompoundSolidManager::CompoundSolidManager(OccBooleanSolid aSolid)
    : mySolid(aSolid)
{
    uint i = 0;
    for (const Occ::Face& newface : mySolid.getFaces())
    {
        pair<uint, uint> data = mySolid.getConstituentFace(newFace);
        mappedFaces.emplace(i, data);
        i++;
    }
}

uint CompoundSolidManager::getFaceIndex(const Occ::Face& aFace) const
{
    for (const auto& data : mappedFaces)
    {
        pair<uint, uint> origMap = data.second;
        Occ::ModifiedSolid aModifiedSolid = mySolid.getModifiedSolids()[origMap.first];
        const Occ::Face& origFace = aModifiedSolid.getOrigSolid().getFaces()[origMap.second];
        if (aModifiedSolid.isModified(origFace))
        {
            const Occ::Face& newFace = aModifiedSolid.getModifiedFace(origFace);
            if (aFace == newFace)
            {
                return data.first;
            }
        }
    }
    throw std::runtime_error("That face does not appear to originate from any of mySolid.getModifiedSolids()");
}

const Occ::Face& CompoundSolidManager::getFaceByIndex(uint i) const
{
    const auto& data = mappedFaces.at(i);
    const vector<Occ::ModifiedSolid>& modSolids = mySolid.getModifiedSolids()
    const Occ::Face& origFace = modSolids.at(data.first).getOrigSolid().getFaces()[data.second];
    return modSolids.at(data.first).getModifiedFace(origFace);
}

const Occ::Solid& CompoundSolidManager::getSolid() const
{
    return mySolid;
}
