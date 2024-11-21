#include "resource-manager/resource_pool.h"

#include <vulkan/vulkan.h>
#include <ktxvulkan.h>
#include "vkformat_enum.h"
#include "ktx_texture_keys.h"

#include <string>

static TextureFormat GetTextureFormat(VkFormat format) {
  switch (format) {
    case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
      return TextureFormat::BC1;
    case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
      return TextureFormat::BC1A;
    case VK_FORMAT_BC3_SRGB_BLOCK:
      return TextureFormat::BC3;
    case VK_FORMAT_BC3_UNORM_BLOCK:
      return TextureFormat::BC3n;
    case VK_FORMAT_BC4_UNORM_BLOCK:
      return TextureFormat::BC4;
    case VK_FORMAT_BC4_SNORM_BLOCK:
      return TextureFormat::BC4S;
    case VK_FORMAT_BC5_UNORM_BLOCK:
      return TextureFormat::BC5;
    case VK_FORMAT_BC5_SNORM_BLOCK:
      return TextureFormat::BC5S;
    case VK_FORMAT_BC6H_UFLOAT_BLOCK:
      return TextureFormat::BC6U;
    case VK_FORMAT_BC6H_SFLOAT_BLOCK:
      return TextureFormat::BC6S;
    case VK_FORMAT_BC7_SRGB_BLOCK:
      return TextureFormat::BC7;
    case VK_FORMAT_R16_SFLOAT:
      return TextureFormat::R16_FLOAT;
    case VK_FORMAT_R32_SFLOAT:
      return TextureFormat::R32_FLOAT;
    case VK_FORMAT_R16G16_SFLOAT:
      return TextureFormat::R16G16_FLOAT;
    case VK_FORMAT_R32G32_SFLOAT:
      return TextureFormat::R32G32_FLOAT;
    case VK_FORMAT_R16G16B16A16_SFLOAT:
      return TextureFormat::R16G16B16A16_FLOAT;
    case VK_FORMAT_R32G32B32A32_SFLOAT:
      return TextureFormat::R32G32B32A32_FLOAT;
    default:
      return TextureFormat::UNDEFINED;
  }
}

template<>
bool ResourcePool<Texture>::LoadImpl(Handle& handle, UUID uuid, void* pUser) {
  // pUser should be the texture type
  TextureType* textureType = (TextureType*) pUser;

  // Get the file path
  char cuuid[32];
  UUIDToString(uuid, cuuid);
  std::string path(cuuid, 32);
  path = xstr(RESOURCE_DIR) + path;

  // Texture resource
  Texture newTexture;

  // KTX texture loading
  ktxTexture* texture;
  KTX_error_code result;
  result = ktxTexture_CreateFromNamedFile(
    path.c_str(),
    KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
    &texture
  );
  if (result != KTX_SUCCESS) {
    printf(ktxErrorString(result));
    return false;
  }

  // Retrieve information about the texture from fields in the ktxTexture
  // ktx_bool_t isArray = texture->isArray; // Don't support array textures

  // sRGB
  unsigned int len;
  if (bool* value; ktxHashList_FindValue(
    &texture->kvDataHead, KTX_KEY_IS_SRGB, 
    &len, (void**) &value) == KTX_SUCCESS) {
    if (len == sizeof(bool)) {
      newTexture.sRGB = *value;
    }
  }

  // TODO: Support cubemaps
  // isCubeMap
  if (bool* value; ktxHashList_FindValue(
    &texture->kvDataHead, KTX_KEY_IS_CUBE_MAP, 
    &len, (void**) &value) == KTX_SUCCESS) {
    if (len == sizeof(bool)) {
      newTexture.isCubeMap = *value;
    }
  }

  // Alpha mode
  if (int* value; ktxHashList_FindValue(
    &texture->kvDataHead, KTX_KEY_ALPHA_MODE, 
    &len, (void**) &value) == KTX_SUCCESS) {
    if (len == sizeof(int)) {
      newTexture.alphaMode = (TextureAlphaMode) *value;
    }
  }

  // TODO: VkFormat to texture format
  VkFormat vkFormat = ktxTexture_GetVkFormat(texture);
  newTexture.format = GetTextureFormat(vkFormat);

  newTexture.width = (u32) texture->baseWidth;
  newTexture.height = (u32) texture->baseHeight;
  
  // Value type
  if (int* value; ktxHashList_FindValue(
    &texture->kvDataHead, KTX_KEY_VALUE_TYPE, 
    &len, (void**) &value) == KTX_SUCCESS) {
    if (len == sizeof(int)) {
      newTexture.valueType = (TextureValueType) *value;
    }
  }

  newTexture.mipLevels = (u32) texture->numLevels;
  newTexture.arrayLayers = (u32) texture->numFaces;

  // Iterate over and get mipmap data. 
  for (int i = 0; i < newTexture.mipLevels; ++i) {
    MipmapData mipmap{};

    // Retrieve a pointer to the image for a specific mip level, array layer
    // & face or depth slice.
    ktx_size_t offset;
    result = ktxTexture_GetImageOffset(texture, i, 0, 0, &offset);
    ktx_uint8_t* image = ktxTexture_GetData(texture) + offset;
    ktx_size_t size = ktxTexture_GetImageSize(texture, i);

    mipmap.level = i;
    mipmap.size = size;
    mipmap.data.resize(size);
    memcpy(mipmap.data.data(), image, size);

    newTexture.mipmaps.emplace_back(std::move(mipmap));
  }

  // TODO: Set handle texture type
  SetHandleType(handle, AL_HANDLE_TEXTURE, (u8)*textureType);

  ktxTexture_Destroy(texture);

  // Set the table entry
  return SetTableEntry(handle, std::move(newTexture));
}

template <>
bool ResourcePool<Texture>::UnloadImpl(Handle& handle) {
  // There is nothing to do, but resetting the optional
  _table[handle.index].second.reset();
  return true;
}







template class ResourcePool<Texture>;

