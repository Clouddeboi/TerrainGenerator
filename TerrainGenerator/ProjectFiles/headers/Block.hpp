#pragma once

#include <cstdint>

/*
    Block type system

    Design rules:
    - Never write if(type == Grass) in rendering code
    - All per-block properties live in BLOCK_DEFS
    - Adding a new block = one new enum value + one table row
    - No heap allocation — constexpr table, indexed by BlockType
*/

enum class BlockType : uint8_t
{
    Air = 0,
    Stone = 1,
    Grass = 2,
    Dirt = 3,
    Sand = 4,
    Water = 5,
    Wood = 6,
    Leaves = 7,
    Ore = 8,
    Snow = 9,

    COUNT   //Always last, use for array sizing and validation
};

struct BlockDef
{
    bool    solid;        //Does this block block movement or cull adjacent faces?
    bool    transparent;  //Does light pass through? (also used for face culling)
    bool    emitsLight;   //Does this block produce light?
    uint8_t lightLevel;   //Light emission strength (0–15)
    uint8_t aoContrib;    //Ambient occlusion contribution (0 = none, 1 = full)
    uint8_t texID;        //Index into the texture atlas
};

//Indexed by BlockType, order must match enum values exactly
constexpr BlockDef BLOCK_DEFS[] =
{
    //solid   transparent  emitsLight  lightLevel  aoContrib  texID
    { false,  true,        false,      0,          0,         0  },  //Air
    { true,   false,       false,      0,          1,         1  },  //Stone
    { true,   false,       false,      0,          1,         2  },  //Grass
    { true,   false,       false,      0,          1,         3  },  //Dirt
    { true,   false,       false,      0,          1,         4  },  //Sand
    { false,  true,        false,      0,          0,         5  },  //Water
    { true,   false,       false,      0,          1,         6  },  //Wood
    { true,   true,        false,      0,          0,         7  },  //Leaves
    { true,   false,       true,       8,          1,         8  },  //Ore
    { true,   false,       false,      0,          1,         9  },  //Snow
};

static_assert(
    static_cast<int>(BlockType::COUNT) ==
    sizeof(BLOCK_DEFS) / sizeof(BLOCK_DEFS[0]),
    "BLOCK_DEFS size does not match BlockType::COUNT — add a row for every new block type");

//Helper queries, used everywhere, never index BLOCK_DEFS directly in game logic

inline bool IsSolid(BlockType t)
{
    return BLOCK_DEFS[static_cast<uint8_t>(t)].solid;
}

inline bool IsTransparent(BlockType t)
{
    return BLOCK_DEFS[static_cast<uint8_t>(t)].transparent;
}

inline bool IsAir(BlockType t)
{
    return t == BlockType::Air;
}

inline bool EmitsLight(BlockType t)
{
    return BLOCK_DEFS[static_cast<uint8_t>(t)].emitsLight;
}

inline uint8_t GetLightLevel(BlockType t)
{
    return BLOCK_DEFS[static_cast<uint8_t>(t)].lightLevel;
}

inline uint8_t GetAOContrib(BlockType t)
{
    return BLOCK_DEFS[static_cast<uint8_t>(t)].aoContrib;
}

inline uint8_t GetTexID(BlockType t)
{
    return BLOCK_DEFS[static_cast<uint8_t>(t)].texID;
}