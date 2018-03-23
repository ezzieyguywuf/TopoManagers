#include <ISolidManager.h>

ISolidManager::ISolidManager(const Occ::Solid& aSolid)
    : myFirstSolid(aSolid)
{}

ISolidManager::ISolidManager(const Occ::Solid& aSolid, const map<uint, pair<uint, uint>>& edges)
    : myFirstSolid(aSolid), mappedEdges(edges)
{}

uint ISolidManager::getEdgeIndex(const Occ::Edge& anEdge) const
{
    for (const auto& data : mappedEdges)
    {
        for (Occ::Edge checkEdge : this->getEdgeByIndex(data.first))
        {
            if (anEdge.isSimilar(checkEdge))
            {
                return data.first;
            }
        }
    }
    throw std::runtime_error("Was unable to find anEdge in mySolid.");
}

vector<Occ::Edge> ISolidManager::getEdgeByIndex(uint i) const
{
    auto faces1 = this->getFaceByIndex(mappedEdges.at(i).first);
    auto faces2 = this->getFaceByIndex(mappedEdges.at(i).second);

    vector<Occ::Edge> outEdges;
    for (const Occ::Face& face1 : faces1)
    {
        for (const Occ::Face& face2 : faces2)
        {
            if (face1.sharesEdge(face2))
            {
                outEdges.push_back(face1.getCommonEdge(face2));
            }
        }
    }
    if (outEdges.size() == 0)
    {
        throw std::runtime_error("That edge does not appear to be in mySolid");
    }
    return outEdges;
}

void ISolidManager::mapEdges()

{
    mappedEdges.clear();
    const Occ::Solid& theSolid = this->getSolid();
    uint count;
    for (const Occ::Edge& checkEdge : theSolid.getEdges())
    {
        count = 0;
        for (const Occ::Face& face1 : theSolid.getFaces())
        {
            bool found = false;
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
                    found = true;
                    count++;
                    uint i1 = this->getFaceIndex(face1);
                    uint i2 = this->getFaceIndex(face2);
                    mappedEdges.emplace(mappedEdges.size(), std::make_pair(i1, i2));
                    break;
                }
            }
            if (found)
            {
                break;
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

Occ::ModifiedSolid ISolidManager::makeModifiedSolid(const ISolidManager& mgrOld,
                                                    const ISolidManager& mgrNew)
{
    map<uint, vector<uint>> newModifiedFaces;
    uints newDeletedFaces;
    map<int, vector<uint>> newNewFaces;

    if (mgrOld.myFirstSolid != mgrNew.myFirstSolid)
    {
        // TODO figure out why I need to comment this out for FeauterPartBoolean to
        // operate properly (in FreeCAD)
        //throw std::runtime_error("Both managers must share a myFirstSolid.");
    }

    for (const Occ::Face& origFace : mgrOld.getSolid().getFaces())
    {
        // Since these managers are the same, the original index should work in the new
        // manager.
        uint i = mgrOld.getFaceIndex(origFace);
        vector<Occ::Face> newFaces = mgrNew.getFaceByIndex(i);
        vector<uint> newFaceIndices;
        if (newFaces.size() > 0)
        {
            for (const Occ::Face& newFace : newFaces)
            {
                newFaceIndices.push_back(mgrNew.getSolid().getFaceIndex(newFace));
            }
            newModifiedFaces.emplace(i, newFaceIndices);
        }
        else
        {
            newDeletedFaces.push_back(i);
        }
        i++;
    }
    for (uint i = mgrOld.getSolid().getFaces().size() ; i < mgrNew.getSolid().getFaces().size() ; i++)
    {
        newNewFaces[-1].push_back(i);
    }
    // ------ ORIG IMPLEMENTATION ------
    return Occ::ModifiedSolid(mgrOld.getSolid(), 
                              mgrNew.getSolid(), 
                              newModifiedFaces,
                              newDeletedFaces, 
                              newNewFaces);
}

