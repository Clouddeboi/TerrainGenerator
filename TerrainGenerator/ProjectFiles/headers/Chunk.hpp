#pragma once

#include <cstdint>
#include <glm/glm.hpp>

#include "../headers/Block.hpp"

/*
    Chunk data structure

    Responsibilities:
    - Own a 16x256x16 block array (64 KB flat, Y-major layout)
    - Provide bounds-checked get/set in debug, unchecked in release
    - Track world chunk position (chunk coords, not block coords)
    - Store pointers to 4 horizontal neighbours for edge face culling
    - Expose a dirty flag so the mesher knows when to rebuild

    Memory:
    - 16 * 256 * 16 * 1 byte = 65,536 bytes = 64 KB per chunk
    - 25x25 render distance = 625 chunks = ~40 MB total

    Indexing (Y-major / column-major):
    - index = x * CZ * CY + z * CY + y
    - Y-major keeps vertical columns contiguous — cache-friendly for
      top-down passes like heightmap sampling and light propagation
*/

class Chunk
{
public:
    //Chunk dimensions
    static constexpr int CX = 16;
    static constexpr int CY = 256;
    static constexpr int CZ = 16;

    static constexpr int BLOCK_COUNT = CX * CY * CZ;

public:
    Chunk();
    ~Chunk() = default;

    //Prevent copying, 64 KB per chunk, copies should be explicit
    Chunk(const Chunk&) = delete;
    Chunk& operator=(const Chunk&) = delete;

    //Allow moving, needed for storage in containers
    Chunk(Chunk&&) = default;
    Chunk& operator=(Chunk&&) = default;

    //Block accessors
    //In debug: bounds-checked with assert
    //In release: direct array access, no overhead
    inline BlockType Get(int x, int y, int z) const;
    inline void Set(int x, int y, int z, BlockType type);

    //Fill entire chunk with a single block type
    void Fill(BlockType type);

    //World chunk position (multiply by CX/CZ to get block-space origin)
    glm::ivec2 GetChunkPos() const { return m_ChunkPos; }
    void SetChunkPos(const glm::ivec2& pos) { m_ChunkPos = pos; }

    //Dirty flag — set when blocks change, cleared after mesh rebuild
    bool IsDirty() const { return m_Dirty; }
    void SetDirty(bool dirty) { m_Dirty = dirty; }

    //Neighbour pointers (null = no neighbour loaded)
    //Used by the mesher to cull faces at chunk boundaries
    Chunk* NeighbourNorth = nullptr;  //+Z
    Chunk* NeighbourSouth = nullptr;  //-Z
    Chunk* NeighbourEast = nullptr;  //+X
    Chunk* NeighbourWest = nullptr;  //-X

    //Query a block, crossing into a neighbour chunk if needed
    //Returns Air if the neighbour is not loaded
    BlockType GetWithNeighbours(int x, int y, int z) const;

private:
    //Y-major flat array: index = x*CZ*CY + z*CY + y
    uint8_t m_Blocks[BLOCK_COUNT];

    //Chunk coordinate in chunk-space (not block-space)
    glm::ivec2 m_ChunkPos = { 0, 0 };

    bool m_Dirty = true;

private:
    //Convert 3D coords to flat array index (Y-major)
    static inline int Index(int x, int y, int z)
    {
        return x * CZ * CY + z * CY + y;
    }

    //Bounds check used in debug builds
    static inline bool InBounds(int x, int y, int z)
    {
        return x >= 0 && x < CX &&
            y >= 0 && y < CY &&
            z >= 0 && z < CZ;
    }
};

//Inline definitions, kept in header to allow compiler inlining in release

inline BlockType Chunk::Get(int x, int y, int z) const
{
#ifdef _DEBUG
    if (!InBounds(x, y, z))
        return BlockType::Air;
#endif
    return static_cast<BlockType>(m_Blocks[Index(x, y, z)]);
}

inline void Chunk::Set(int x, int y, int z, BlockType type)
{
#ifdef _DEBUG
    if (!InBounds(x, y, z))
        return;
#endif
    m_Blocks[Index(x, y, z)] = static_cast<uint8_t>(type);
    m_Dirty = true;
}