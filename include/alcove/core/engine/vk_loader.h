#pragma once
#include <core/engine/vk_common.h>

#include <core/engine/vk_types.h>
#include <unordered_map>
#include <filesystem>

struct GeoSurface {
    uint32_t startIndex;
    uint32_t count;
};

struct MeshAsset {
    std::string name;

    std::vector<GeoSurface> surfaces;
    GPUMeshBuffers meshBuffers;
};

// Forward declaration
class VulkanEngine;

using OpMeshPtrVector = std::optional<std::vector<std::shared_ptr<MeshAsset>>>;
OpMeshPtrVector LoadGLTFMeshes(VulkanEngine* engine, std::filesystem::path filepath);

