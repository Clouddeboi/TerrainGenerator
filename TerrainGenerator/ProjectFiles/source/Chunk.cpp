#include "../headers/Chunk.hpp"

#include <cstring>

Chunk::Chunk()
{
    //Default fill with Air
    std::memset(m_Blocks, 0, sizeof(m_Blocks));
}

void Chunk::Fill(BlockType type)
{
    std::memset(m_Blocks, static_cast<uint8_t>(type), sizeof(m_Blocks));
    m_Dirty = true;
}

BlockType Chunk::GetWithNeighbours(int x, int y, int z) const
{
    //Y is always local, only X and Z can cross chunk borders
    if (y < 0 || y >= CY)
        return BlockType::Air;

    //Local block, no crossing needed
    if (x >= 0 && x < CX && z >= 0 && z < CZ)
        return Get(x, y, z);

    //Crossed into east neighbour (+X)
    if (x >= CX)
    {
        if (NeighbourEast)
            return NeighbourEast->Get(x - CX, y, z);
        return BlockType::Air;
    }

    //Crossed into west neighbour (-X)
    if (x < 0)
    {
        if (NeighbourWest)
            return NeighbourWest->Get(x + CX, y, z);
        return BlockType::Air;
    }

    //Crossed into north neighbour (+Z)
    if (z >= CZ)
    {
        if (NeighbourNorth)
            return NeighbourNorth->Get(x, y, z - CZ);
        return BlockType::Air;
    }

    //Crossed into south neighbour (-Z)
    if (z < 0)
    {
        if (NeighbourSouth)
            return NeighbourSouth->Get(x, y, z + CZ);
        return BlockType::Air;
    }

    return BlockType::Air;
}