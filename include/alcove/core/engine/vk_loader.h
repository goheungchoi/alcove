#include <vk_types.h>
#include <unordered_map>
#include <filesystem>

struct SubMesh {
    uint32_t startIndex;
    uint32_t count;
};

struct MeshAsset {
    std::string name;

    std::vector<SubMesh> surfaces;
    GPUMeshBuffers meshBuffers;
};

// Forward declaration
class VulkanEngine;
