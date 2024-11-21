#include "asset-importer/texture-compressor/texture_compressor.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

#include <nvtt/nvtt.h>

#include <ktx.h>
#include "vkformat_enum.h"
#include "ktx_texture_keys.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static int GetRequiredNumChannels(TextureCompressionFormat format) {
  int res;
  switch (format) {
    case TextureCompressionFormat::BC1:
      res = 3;
      break;
    case TextureCompressionFormat::BC1a:
      res = 4;
      break;
    case TextureCompressionFormat::BC2:
      res = 4;
      break;
    case TextureCompressionFormat::BC3:
      res = 4;
      break;
    case TextureCompressionFormat::BC3n:
      res = 4;
      break;
    case TextureCompressionFormat::BC4:
      res = 1;
      break;
    case TextureCompressionFormat::BC4S:
      res = 1;
      break;
    case TextureCompressionFormat::BC5:
      res = 2;
      break;
    case TextureCompressionFormat::BC5S:
      res = 2;
      break;
    case TextureCompressionFormat::BC6U:
      res = 3;
      break;
    case TextureCompressionFormat::BC6S:
      res = 3;
      break;
    case TextureCompressionFormat::BC7:
      res = 4;
      break;
    case TextureCompressionFormat::Uncompressed:
      res = -1;
      break;
    default:
      res = 0;
  }
  return res;
}

static bool IsFloatType(TextureValueType type) {
  if (type == TextureValueType::FLOAT16 || type == TextureValueType::FLOAT32)
    return true;
  else
    return false;
}

static nvtt::Format ToNVTTFormat(TextureCompressionFormat format) {
  nvtt::Format res;

  switch (format) {
    case TextureCompressionFormat::BC1:
      res = nvtt::Format::Format_BC1;
      break;
    case TextureCompressionFormat::BC1a:
      res = nvtt::Format::Format_BC1a;
      break;
    case TextureCompressionFormat::BC2:
      res = nvtt::Format::Format_BC2;
      break;
    case TextureCompressionFormat::BC3:
      res = nvtt::Format::Format_BC3;
      break;
    case TextureCompressionFormat::BC3n:
      res = nvtt::Format::Format_BC3n;
      break;
    case TextureCompressionFormat::BC4:
      res = nvtt::Format::Format_BC4;
      break;
    case TextureCompressionFormat::BC4S:
      res = nvtt::Format::Format_BC4S;
      break;
    case TextureCompressionFormat::BC5:
      res = nvtt::Format::Format_BC5;
      break;
    case TextureCompressionFormat::BC5S:
      res = nvtt::Format::Format_BC5S;
      break;
    case TextureCompressionFormat::BC6U:
      res = nvtt::Format::Format_BC6U;
      break;
    case TextureCompressionFormat::BC6S:
      res = nvtt::Format::Format_BC6S;
      break;
    case TextureCompressionFormat::BC7:
      res = nvtt::Format::Format_BC7;
      break;
    case TextureCompressionFormat::Uncompressed:
      res = nvtt::Format::Format_RGBA;
      break;
  }

  return res;
}

static nvtt::ValueType ToNVTTValueType(TextureValueType type) {
  nvtt::ValueType res;

  switch (type) {
    case TextureValueType::UINT8:
      res = nvtt::ValueType::UINT8;
      break;
    case TextureValueType::SINT8:
      res = nvtt::ValueType::SINT8;
      break;
    case TextureValueType::FLOAT16:
      res = nvtt::ValueType::FLOAT16;
      break;
    case TextureValueType::FLOAT32:
      res = nvtt::ValueType::FLOAT32;
      break;
  }

  return res;
}

static nvtt::Quality ToNVTTQuality(TextureCompressionQuality quality) {
  nvtt::Quality res;

  switch (quality) {
    case TextureCompressionQuality::Fast:
      res = nvtt::Quality::Quality_Fastest;
      break;
    case TextureCompressionQuality::Normal:
      res = nvtt::Quality::Quality_Normal;
      break;
    case TextureCompressionQuality::Medium:
      res = nvtt::Quality::Quality_Production;
      break;
    case TextureCompressionQuality::Highest:
      res = nvtt::Quality::Quality_Highest;
      break;
  }

  return res;
}

static nvtt::AlphaMode ToNVTTAlphaMode(TextureAlphaMode mode) {
  nvtt::AlphaMode res;

  switch (mode) {
    case TextureAlphaMode::Opaque:
      res = nvtt::AlphaMode::AlphaMode_None;
      break;
    case TextureAlphaMode::Transparent:
      res = nvtt::AlphaMode::AlphaMode_Transparency;
      break;
  }

  return res;
}

static nvtt::MipmapFilter ToNVTTMipMapFilter(TextureMipMapFilter filter) {
  nvtt::MipmapFilter res;

  switch (filter) {
    case TextureMipMapFilter::Box:
      res = nvtt::MipmapFilter::MipmapFilter_Box;
      break;
    case TextureMipMapFilter::Triangle:
      res = nvtt::MipmapFilter::MipmapFilter_Triangle;
      break;
    case TextureMipMapFilter::Kaiser:
      res = nvtt::MipmapFilter::MipmapFilter_Kaiser;
      break;
    case TextureMipMapFilter::Mitchell:
      res = nvtt::MipmapFilter::MipmapFilter_Mitchell;
      break;
    case TextureMipMapFilter::Min:
      res = nvtt::MipmapFilter::MipmapFilter_Min;
      break;
    case TextureMipMapFilter::Max:
      res = nvtt::MipmapFilter::MipmapFilter_Max;
      break;
  }

  return res;
}

static int BytesPerTile(TextureCompressionFormat format) {
  int res;

  switch (format) {
    case TextureCompressionFormat::BC1:
      res = 8;
      break;
    case TextureCompressionFormat::BC1a:
      res = 8;
      break;
    case TextureCompressionFormat::BC2:
      res = 16;
      break;
    case TextureCompressionFormat::BC3:
      res = 16;
      break;
    case TextureCompressionFormat::BC3n:
      res = 16;
      break;
    case TextureCompressionFormat::BC4:
      res = 8;
      break;
    case TextureCompressionFormat::BC4S:
      res = 8;
      break;
    case TextureCompressionFormat::BC5:
      res = 16;
      break;
    case TextureCompressionFormat::BC5S:
      res = 16;
      break;
    case TextureCompressionFormat::BC6U:
      res = 16;
      break;
    case TextureCompressionFormat::BC6S:
      res = 16;
      break;
    case TextureCompressionFormat::BC7:
      res = 16;
      break;
  }

  return res;
}

static VkFormat GetVkFormat(int channels, nvtt::ValueType type, nvtt::Format format) {
  if (format == nvtt::Format_RGBA) {  // No compression
    if (channels == 1) {
      switch (type){
        // case nvtt::UINT8:
        //   return VK_FORMAT_R8_UINT;
        // case nvtt::SINT8:
        //   return VK_FORMAT_R8_SINT;
        case nvtt::FLOAT16:
          return VK_FORMAT_R16_SFLOAT;
        case nvtt::FLOAT32:
          return VK_FORMAT_R32_SFLOAT;
      }
    } else if (channels == 2) {
      switch (type){
        // case nvtt::UINT8:
        //   return VK_FORMAT_R8G8_UINT;
        // case nvtt::SINT8:
        //   return VK_FORMAT_R8G8_SINT;
        case nvtt::FLOAT16:
          return VK_FORMAT_R16G16_SFLOAT;
        case nvtt::FLOAT32:
          return VK_FORMAT_R32G32_SFLOAT;
      }
    } else if (channels == 3) {
      // switch (type){
      //   case nvtt::UINT8:
      //     return VK_FORMAT_R8G8B8_UINT;
      //   case nvtt::SINT8:
      //     return VK_FORMAT_R8G8B8_SINT;
      //   case nvtt::FLOAT16:
      //     return VK_FORMAT_R16G16B16_SFLOAT;
      //   case nvtt::FLOAT32:
      //     return VK_FORMAT_R32G32B32_SFLOAT;
      // }
    } else {
      switch (type){
        // case nvtt::UINT8:
        //   return VK_FORMAT_R8G8B8A8_UINT;
        // case nvtt::SINT8:
        //   return VK_FORMAT_R8G8B8A8_SINT;
        case nvtt::FLOAT16:
          return VK_FORMAT_R16G16B16A16_SFLOAT;
        case nvtt::FLOAT32:
          return VK_FORMAT_R32G32B32A32_SFLOAT;
      }
    }
  } else {
    switch (format) {
      case nvtt::Format_BC1 :
        return VK_FORMAT_BC1_RGB_SRGB_BLOCK;
      case nvtt::Format_BC1a:
        return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
      case nvtt::Format_BC2:
        return VK_FORMAT_BC2_SRGB_BLOCK;
      case nvtt::Format_BC3:
        return VK_FORMAT_BC3_SRGB_BLOCK;
      case nvtt::Format_BC3n:
        return VK_FORMAT_BC3_UNORM_BLOCK;
      case nvtt::Format_BC4:
        return VK_FORMAT_BC4_UNORM_BLOCK;
      case nvtt::Format_BC4S:
        return VK_FORMAT_BC4_SNORM_BLOCK;
      case nvtt::Format_BC5:
        return VK_FORMAT_BC5_UNORM_BLOCK;
      case nvtt::Format_BC5S:
        return VK_FORMAT_BC5_SNORM_BLOCK;
      case nvtt::Format_BC6U:
        return VK_FORMAT_BC6H_UFLOAT_BLOCK;
      case nvtt::Format_BC6S:
        return VK_FORMAT_BC6H_SFLOAT_BLOCK;
      case nvtt::Format_BC7:
        return VK_FORMAT_BC7_SRGB_BLOCK;
    }
  }

  return VK_FORMAT_UNDEFINED;
}


struct CustomOutputHandler : public nvtt::OutputHandler {
  // Structure to hold info for each mipmap level
  struct MipmapData {
    int level;
    int size;
    int width;
    int height;
    int depth;
    std::vector<uint8_t> data;
  };

  std::vector<MipmapData> mipmaps;

  /// Indicate the start of a new compressed image that's part of the final texture.
  void beginImage(int size, int width, int height, int depth, int face, int miplevel) override {
    printf("Generating compression on level %d with size: %d x %d; %d byte(s)\n", miplevel, width, height, size);

    MipmapData mipmap {
      .level = miplevel,
      .size = size,
      .width = width,
      .height = height,
      .depth = depth
    };
    mipmaps.push_back(std::move(mipmap));
  };

  /// Output data. Compressed data is output as soon as it's generated to minimize memory allocations.
  bool writeData(const void* data, int size) override {
    printf("Writing data of size: %d byte(s)\n", size);
    // Copy data to the current mipmap level
    if (!mipmaps.empty()) {
      MipmapData& mipmap{ mipmaps.back() };
      mipmap.data.resize(size);

      memcpy(mipmap.data.data(), data, size);
    }
    return true;
  }

  /// Indicate the end of the compressed image. (New in NVTT 2.1)
  void endImage() override {
    // Finalize the current mipmap processing if needed
    printf("Finished compression\n");
  }
};


bool TextureCompressor::Compress(
  const char* filename, 
  const char* exportPath, 
  const ImageData* data,
  const CompressOptions* settings) {

  nvtt::Format format = ToNVTTFormat(settings->format);
  nvtt::Quality quality = ToNVTTQuality(settings->quality);
  nvtt::AlphaMode alphaMode = ToNVTTAlphaMode(data->alphaMode);
  bool isNormalMap = data->isNormalMap;
  nvtt::MipmapFilter filter = ToNVTTMipMapFilter(settings->mipMapFilter);
  bool shouldGammaCorrect = data->colorSpace == TextureColorSpace::sRGB;

  // nvtt::RefImage imgInput;
  // imgInput.data = _m->_image;
  // imgInput.width = _m->_imgWidth;
  // imgInput.height = _m->_imgHeight;
  // imgInput.num_channels = _m->_requiredChannels;
  // if (_m->_channelsInFile == 3)
  //   imgInput.channel_swizzle[3] = nvtt::One;

  // // TODO: Need to change the value type for different type of images
  // nvtt::CPUInputBuffer inputBuffer(&imgInput, ToNVTTValueType(_m->_valueType));

  // _m->_out = malloc(inputBuffer.NumTiles() * BytesPerTile(settings->format));

  // const auto encodeSettings = nvtt::EncodeSettings()
  //   .SetFormat(ToNVTTFormat(settings->format))
  //   .SetQuality(ToNVTTQuality(settings->quality))
  //   .SetUseGPU(settings->useGPU)  // Compress on the GPU if CUDA is available, instead of the CPU.
  //   .SetOutputToGPUMem(false);

  // return nvtt::nvtt_encode(inputBuffer, _m->_out, encodeSettings);

  nvtt::Surface image;
  if (!image.load(filename)) {
    return false;
  }
  image.setAlphaMode(alphaMode);
  image.setNormalMap(isNormalMap);

  nvtt::Context context(true);

  nvtt::CompressionOptions options;
  options.setFormat(format);
  options.setQuality(quality);

  nvtt::OutputOptions output;
  output.setFileName(exportPath);
  output.setContainer(nvtt::Container_DDS10);

  // MipMap generation stage
  const int numMipMaps = [&] () {
    if (settings->enableMipMap) {
      const int count = image.countMipmaps();
      int num = settings->numMipMaps;
      if (num <= 0 || count < num) {
        return count;
      } else {
        return num;
      }
    }
    return 1;
  } ();

  if (!context.outputHeader(image, numMipMaps, options, output)) {
    return false;
  }

  for (int mip = 0; mip < numMipMaps; ++mip) {
    if (!context.compress(image, 0, mip, options, output)) {
      return false;
    }

    if (mip == numMipMaps - 1) break;

    // Prepare the next mip
    if (shouldGammaCorrect) {
      image.toLinearFromSrgb();
    }
    if (alphaMode) {
      // https://github.com/nvpro-samples/nvtt_samples/blob/main/mipmap/main.cpp
      // Convert to linear premultiplied alpha. Note that toLinearFromSrgb()
      // will clamp HDR images; consider e.g. toLinear(2.2f) instead.
      image.premultiplyAlpha();
    }

    // https://github.com/nvpro-samples/nvtt_samples/blob/main/mipmap/main.cpp
    // Resize the image to the next mipmap size.
    // NVTT has several mipmapping filters; Box is the lowest-quality, but
    // also the fastest to use.
    image.buildNextMipmap(filter);

    // Convert back
    if (alphaMode) {
      image.demultiplyAlpha();
    }
    if (shouldGammaCorrect) {
      image.toSrgb();
    }
  }

  return true;
}

bool TextureCompressor::CompressKTX2(
  const char *filename, 
  const char *exportPath, 
  const ImageData* data,
  const CompressOptions *settings) {
  const void* image;
  int width, height, channels, ok;
  // Check if the file is a valid texture
  ok = stbi_info(filename, &width, &height, &channels);
  if (!ok) {
    printf("invalid image file\n");
    return false;
  } 

  // Get the required number of channels for the compression format
  int requiredChannels = GetRequiredNumChannels(settings->format);

  // nvtt settings
  nvtt::Format format;
  // nvtt::ValueType valueType = ToNVTTValueType(data->type);
  // nvtt::AlphaMode alphaMode = ToNVTTAlphaMode(data->alphaMode);
  // bool isNormalMap = settings->isNormalMap;
  // nvtt::MipmapFilter filter = ToNVTTMipMapFilter(settings->mipMapFilter);
  // bool shouldGammaCorrect = data->colorSpace == ColorSpace::sRGB;
  const bool isFloat{ IsFloatType(data->type) };
  // Check if the texture compression is in default mode
  if (requiredChannels) {
    if (requiredChannels < 0) {
      format = ToNVTTFormat(settings->format);
      requiredChannels = channels;
    } else  {
      format = ToNVTTFormat(settings->format);
    }
  } else {
    // Find the proper format for the texture
    switch (channels) {
      case 1: {
        if (data->type == TextureValueType::UINT8)
          format = nvtt::Format_BC4;
        else if (data->type == TextureValueType::SINT8)
          format = nvtt::Format_BC4S;
        else  // HDR with 1 channel should not be compressed
          format = nvtt::Format_RGBA;
        requiredChannels = 1;
      }
      break;
      case 2: {
        if (data->type == TextureValueType::UINT8)
          format = nvtt::Format_BC5;
        else if (data->type == TextureValueType::SINT8)
          format = nvtt::Format_BC5S;
        else  // HDR with 2 channels should not be compressed
          format = nvtt::Format_RGBA;
        requiredChannels = 2;
      }
      break;
      case 3:
        if (isFloat) 
          format = nvtt::Format_BC6S;
        else
          format = nvtt::Format_BC7;
        requiredChannels = 3;
      break;
      case 4:
        if (isFloat) {  // HDR with 4 channels can't be compressed
          format = nvtt::Format_RGBA;
        } else {
          format = nvtt::Format_BC7;
        }
        requiredChannels = 4;
      break;
      default:
        printf("unknown error");
        return false;
    }
  }

  // Load the texture
  nvtt::InputFormat inputFormat;
  if (isFloat) {
    // WARNING
    if (!stbi_is_hdr(filename)) { 
      printf(
        "warning: texture component is in UINT8 or SINT8 type, "
        "but loaded in FLOAT16 or FLOAT32 type\n"
      );
    }

    // HDR loading
    if (data->type == TextureValueType::FLOAT16) {
      inputFormat = nvtt::InputFormat_RGBA_16F;
      image = stbi_load_16(filename, &width, &height, &channels, 4);
    } else {
      if (channels == 1) {
        inputFormat = nvtt::InputFormat_R_32F;
        image = stbi_loadf(filename, &width, &height, &channels, 1);
      } else {
        inputFormat = nvtt::InputFormat_RGBA_32F;
        image = stbi_loadf(filename, &width, &height, &channels, 4);
      }
    }
  } else {
    // LDR loading
    if (data->type == TextureValueType::UINT8)
      inputFormat = nvtt::InputFormat_BGRA_8UB;
    else
      inputFormat = nvtt::InputFormat_BGRA_8SB;

    image = stbi_load(filename, &width, &height, &channels, 4);
  }

  // Image was not loaded, raise an error message
  if (!image) {
    printf("image loading failed\n");
    return false;
  }

  nvtt::ValueType type = ToNVTTValueType(data->type);
  
  nvtt::AlphaMode alphaMode = ToNVTTAlphaMode(data->alphaMode);
  bool isNormalMap = data->isNormalMap;
  
  nvtt::Surface surface;
  if (!surface.setImage(inputFormat, width, height, 1, image)) {
    printf("surface creation failed\n");
    return false;
  }
  surface.setAlphaMode(alphaMode);
  surface.setNormalMap(isNormalMap);
  // Swizzle channels
  if (isFloat) {
    if (channels < requiredChannels && channels <= 1)
      surface.swizzle(nvtt::Red, nvtt::Zero, nvtt::Zero, nvtt::One);
    else if (channels < requiredChannels && channels <= 2)
      surface.swizzle(nvtt::Red, nvtt::Green, nvtt::Zero, nvtt::One);
    else if (channels < requiredChannels && channels <= 3)
      surface.swizzle(nvtt::Red, nvtt::Green, nvtt::Blue, nvtt::One);
    else
      surface.swizzle(nvtt::Red, nvtt::Green, nvtt::Blue, nvtt::Alpha);
  } else {
    if (channels < requiredChannels && channels <= 1)
      surface.swizzle(nvtt::Zero, nvtt::Zero, nvtt::Red, nvtt::One);
    else if (channels < requiredChannels && channels <= 2)
      surface.swizzle(nvtt::Zero, nvtt::Green, nvtt::Red, nvtt::One);
    else if (channels < requiredChannels && channels <= 3)
      surface.swizzle(nvtt::Blue, nvtt::Green, nvtt::Red, nvtt::One);
    else
      surface.swizzle(nvtt::Blue, nvtt::Green, nvtt::Red, nvtt::Alpha);
  }

  nvtt::Context context(true);
  
  // Handle quality exceptions
  /// Quality    | BC1 | BC1a | BC2 | BC3 | BC3n | RGBM | BC4 | BC5 | BC6 | BC7       | ASTC |
	/// -----------|-----|------|-----|-----|------|------|-----|-----|-----|-----------|------|
	/// Fastest    | Yes | No   | No  | No  | No   | No   | Yes | Yes | Yes | Yes       | Yes  |
	/// Normal     | Yes | Yes  | Yes | Yes | Yes  | No   | Yes | Yes | Yes | Yes       | Yes  |
	/// Production | Yes | Yes  | Yes | Yes | Yes  | No   | No  | No  | No  | No (slow) | Yes  |
	/// Highest    | Yes | Yes  | Yes | Yes | Yes  | No   | No  | No  | No  | No (slow) | Yes  |
	/// 
  nvtt::Quality quality{ ToNVTTQuality(settings->quality) };
  switch (format) {
    case nvtt::Format_BC1: break;
    case nvtt::Format_BC1a:
    case nvtt::Format_BC2:
    case nvtt::Format_BC3:
    case nvtt::Format_BC3n:{
      if (quality < nvtt::Quality_Normal)
        quality = nvtt::Quality_Normal;
    } break;
    case nvtt::Format_BC4:
    case nvtt::Format_BC4S:
    case nvtt::Format_BC5:
    case nvtt::Format_BC5S:
    case nvtt::Format_BC6U:
    case nvtt::Format_BC6S:
    case nvtt::Format_BC7:{
      if (nvtt::Quality_Normal < quality)
        quality = nvtt::Quality_Normal;
    } break;
    default: break;
  }

  nvtt::CompressionOptions options;
  options.setFormat(format);
  options.setQuality(quality);

  CustomOutputHandler outputHandler;
  nvtt::OutputOptions output;
  output.setOutputHandler(&outputHandler);

  
  // MipMap generation stage
  const int numMipMaps = [&] () {
    if (settings->enableMipMap) {
      const int count = surface.countMipmaps();
      int num = settings->numMipMaps;
      if (num <= 0 || count < num) {
        return count;
      } else {
        return num;
      }
    }
    return 1;
  } ();

  nvtt::MipmapFilter filter{ ToNVTTMipMapFilter(settings->mipMapFilter) };
  bool isSRGB{ data->colorSpace == TextureColorSpace::sRGB };
  for (int mip = 0; mip < numMipMaps; ++mip) {
    if (!context.compress(surface, 0, mip, options, output)) {
      printf("texture compression failed\n");
      return false;
    }

    if (mip == numMipMaps - 1) break;

    // Prepare the next mip
    if (isSRGB) {
      // https://github.com/nvpro-samples/nvtt_samples/blob/main/mipmap/main.cpp
      // Convert to linear premultiplied alpha. Note that toLinearFromSrgb()
      // will clamp HDR images; consider e.g. toLinear(2.2f) instead.
      if (isFloat)
        surface.toLinear(2.2);
      else
        surface.toLinearFromSrgb();
    }
    if (alphaMode) {
      surface.premultiplyAlpha();
    }

    // https://github.com/nvpro-samples/nvtt_samples/blob/main/mipmap/main.cpp
    // Resize the image to the next mipmap size.
    // NVTT has several mipmapping filters; Box is the lowest-quality, but
    // also the fastest to use.
    surface.buildNextMipmap(filter);

    // Convert back
    if (alphaMode) {
      surface.demultiplyAlpha();
    }
    if (isSRGB) {
      surface.toSrgb();
    }
  }

  // Store the texture 
  ktxTexture2* texture;

  ktxTextureCreateInfo createInfo{};
  createInfo.vkFormat = GetVkFormat(channels, type, format);
  if (createInfo.vkFormat == VK_FORMAT_UNDEFINED) {
    printf("undefined vkformat");
    return false;
  }
  createInfo.baseWidth = width;
  createInfo.baseHeight = height;
  createInfo.baseDepth = 1;
  createInfo.numDimensions = 2;
  createInfo.numLevels = numMipMaps;
  createInfo.numLayers = 1;
  createInfo.numFaces = 1;
  createInfo.isArray = KTX_FALSE;
  createInfo.generateMipmaps = KTX_FALSE;

  KTX_error_code result;
  result = ktxTexture2_Create(&createInfo, KTX_TEXTURE_CREATE_ALLOC_STORAGE, &texture);
  if (result != KTX_SUCCESS) {
    printf(ktxErrorString(result));
    return false;
  }

  for(auto mipmap : outputHandler.mipmaps) {
    result = ktxTexture_SetImageFromMemory(
      ktxTexture(texture),
      mipmap.level,
      0,
      0,
      mipmap.data.data(),
      mipmap.size
    );
    if (result != KTX_SUCCESS) {
      printf(ktxErrorString(result));
      return false;
    }
  }

  ktxHashList_AddKVPair(&texture->kvDataHead, KTX_KEY_IS_SRGB, sizeof(bool), &isSRGB);
  ktxHashList_AddKVPair(&texture->kvDataHead, KTX_KEY_IS_CUBE_MAP, sizeof(bool), &data->isCubeMap);

  ktxHashList_AddKVPair(&texture->kvDataHead, KTX_KEY_ALPHA_MODE, sizeof(int), &alphaMode);

  ktxHashList_AddKVPair(&texture->kvDataHead, KTX_KEY_VALUE_TYPE, sizeof(int), &type);

  result = ktxTexture_WriteToNamedFile(ktxTexture(texture), exportPath);
  if (result != KTX_SUCCESS) {
    printf(ktxErrorString(result));
    return false;
  }
  ktxTexture_Destroy(ktxTexture(texture));
  
  return true;
}


// bool TextureCompressor::Export(const char *path) {
//   if (!_m->_out) return false;
//   // TODO: KTX2 image exportion?

//   return false;
// }
