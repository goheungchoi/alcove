#include "core/engine/vk_loader.h"

#include <iostream>

#include "core/engine/vk_engine.h"
#include "core/engine/vk_structs.h"
#include "core/engine/vk_types.h"

// STB image
//#include "stb_image.h"
// GLM Quaternion
#include "glm/gtx/quaternion.hpp"
// Assimp mesh loader
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

OpMeshPtrVector LoadGLTFMeshes(VulkanEngine* engine, std::filesystem::path filepath) {
  std::cout << "Loading GLTF: " << filepath << std::endl;

  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(
    filepath.string(), 
    aiProcess_Triangulate |
    aiProcess_GenNormals
  );

  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
  {
      fmt::println("ERROR::ASSIMP::{}", importer.GetErrorString());
      return {};
  }
  std::vector<std::shared_ptr<MeshAsset>> meshes;
  std::vector<uint32_t> indices;
  std::vector<Vertex> vertices;

  // Process all meshes
  for (uint i = 0; i < scene->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[i];
    MeshAsset newmesh;
    newmesh.name = mesh->mName.C_Str();

    indices.clear();
    vertices.clear();

    GeoSurface newSurface;
    newSurface.startIndex = static_cast<uint32_t>(indices.size());
    newSurface.count = mesh->mNumFaces * 3; // Each face has 3 indices for triangles

    size_t initial_vtx = vertices.size();

    // Load indices
    for (uint j = 0; j < mesh->mNumFaces; j++) {
      aiFace face = mesh->mFaces[j];
      for (uint k = 0; k < face.mNumIndices; k++) {
        indices.push_back(face.mIndices[k] + initial_vtx);
      }
    }

    // Load vertices, normals, UVs, colors
    for (uint j = 0; j < mesh->mNumVertices; j++) {
      Vertex newvtx;

      // Vertex positions
      aiVector3D& pos = mesh->mVertices[j];
      newvtx.position = glm::vec3(pos.x, pos.y, pos.z);

      // Vertex normals
      if (mesh->HasNormals()) {
        aiVector3D& normal = mesh->mNormals[j];
        newvtx.normal = glm::vec3(normal.x, normal.y, normal.z);
      } else {
        newvtx.normal = glm::vec3(1.0f, 0.0f, 0.0f);
      }

      // Vertex UVs
      if (mesh->HasTextureCoords(0)) {
        aiVector3D& uv = mesh->mTextureCoords[0][j];
        newvtx.uv_x = uv.x;
        newvtx.uv_y = uv.y;
      } else {
        newvtx.uv_x = 0.0f;
        newvtx.uv_y = 0.0f;
      }

      // Vertex colors (if any)
      if (mesh->HasVertexColors(0)) {
        aiColor4D& color = mesh->mColors[0][j];
        newvtx.color = glm::vec4(color.r, color.g, color.b, color.a);
      } else {
        newvtx.color = glm::vec4(1.0f);
      }

      vertices.push_back(newvtx);
    }

    newmesh.surfaces.push_back(newSurface);

    // Override vertex colors with normals for visualization, if needed
    constexpr bool OverrideColors = true;
    if (OverrideColors) {
      for (Vertex& vtx : vertices) {
          vtx.color = glm::vec4(vtx.normal, 1.f);
      }
    }

    // Upload the mesh to GPU
    newmesh.meshBuffers = engine->upload_mesh(vertices, indices);

    // Store the mesh
    meshes.emplace_back(std::make_shared<MeshAsset>(std::move(newmesh)));
  }

  return meshes;
}


