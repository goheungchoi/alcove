#include "asset-importer/compressor/texture_compressor.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include <nvtt/nvtt.h>

#include "ktx.h"

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

static nvtt::Format ToNVTTFormat(CompressionFormat format) {
  nvtt::Format res;

  switch (format) {
    case CompressionFormat::BC1:
      res = nvtt::Format::Format_BC1;
      break;
    case CompressionFormat::BC1A:
      res = nvtt::Format::Format_BC1a;
      break;
    case CompressionFormat::BC2:
      res = nvtt::Format::Format_BC2;
      break;
    case CompressionFormat::BC3:
      res = nvtt::Format::Format_BC3;
      break;
    case CompressionFormat::BC3N:
      res = nvtt::Format::Format_BC3n;
      break;
    case CompressionFormat::BC4:
      res = nvtt::Format::Format_BC4;
      break;
    case CompressionFormat::BC4S:
      res = nvtt::Format::Format_BC4S;
      break;
    case CompressionFormat::BC5:
      res = nvtt::Format::Format_BC5;
      break;
    case CompressionFormat::BC5S:
      res = nvtt::Format::Format_BC5S;
      break;
    case CompressionFormat::BC6U:
      res = nvtt::Format::Format_BC6U;
      break;
    case CompressionFormat::BC6S:
      res = nvtt::Format::Format_BC6S;
      break;
    case CompressionFormat::BC7:
      res = nvtt::Format::Format_BC7;
      break;
  }

  return res;
}

static nvtt::ValueType ToNVTTValueType(ValueType type) {
  nvtt::ValueType res;

  switch (type) {
    case ValueType::UINT8:
      res = nvtt::ValueType::UINT8;
      break;
    case ValueType::SINT8:
      res = nvtt::ValueType::SINT8;
      break;
    case ValueType::FLOAT16:
      res = nvtt::ValueType::FLOAT16;
      break;
    case ValueType::FLOAT32:
      res = nvtt::ValueType::FLOAT32;
      break;
  }

  return res;
}

static nvtt::Quality ToNVTTQuality(CompressionQuality quality) {
  nvtt::Quality res;

  switch (quality) {
    case CompressionQuality::Fast:
      res = nvtt::Quality::Quality_Fastest;
      break;
    case CompressionQuality::Normal:
      res = nvtt::Quality::Quality_Normal;
      break;
    case CompressionQuality::Medium:
      res = nvtt::Quality::Quality_Production;
      break;
    case CompressionQuality::Highest:
      res = nvtt::Quality::Quality_Highest;
      break;
  }

  return res;
}

static nvtt::AlphaMode ToNVTTAlphaMode(AlphaMode mode) {
  nvtt::AlphaMode res;

  switch (mode) {
    case AlphaMode::Opaque:
      res = nvtt::AlphaMode::AlphaMode_None;
      break;
    case AlphaMode::Transparent:
      res = nvtt::AlphaMode::AlphaMode_Transparency;
      break;
  }

  return res;
}

static nvtt::MipmapFilter ToNVTTMipMapFilter(MipMapFilter filter) {
  nvtt::MipmapFilter res;

  switch (filter) {
    case MipMapFilter::Box:
      res = nvtt::MipmapFilter::MipmapFilter_Box;
      break;
    case MipMapFilter::Triangle:
      res = nvtt::MipmapFilter::MipmapFilter_Triangle;
      break;
    case MipMapFilter::Kaiser:
      res = nvtt::MipmapFilter::MipmapFilter_Kaiser;
      break;
    case MipMapFilter::Mitchell:
      res = nvtt::MipmapFilter::MipmapFilter_Mitchell;
      break;
    case MipMapFilter::Min:
      res = nvtt::MipmapFilter::MipmapFilter_Min;
      break;
    case MipMapFilter::Max:
      res = nvtt::MipmapFilter::MipmapFilter_Max;
      break;
  }

  return res;
}

static int BytesPerTile(CompressionFormat format) {
  int res;

  switch (format) {
    case CompressionFormat::BC1:
      res = 8;
      break;
    case CompressionFormat::BC1A:
      res = 8;
      break;
    case CompressionFormat::BC2:
      res = 16;
      break;
    case CompressionFormat::BC3:
      res = 16;
      break;
    case CompressionFormat::BC3N:
      res = 16;
      break;
    case CompressionFormat::BC4:
      res = 8;
      break;
    case CompressionFormat::BC4S:
      res = 8;
      break;
    case CompressionFormat::BC5:
      res = 16;
      break;
    case CompressionFormat::BC5S:
      res = 16;
      break;
    case CompressionFormat::BC6U:
      res = 16;
      break;
    case CompressionFormat::BC6S:
      res = 16;
      break;
    case CompressionFormat::BC7:
      res = 16;
      break;
  }

  return res;
}

struct TextureCompressor::Private {
  void* _image;
  int _imgWidth, _imgHeight;
  unsigned long long _imgSize;

// An output image with N components has the following components interleaved
// in this order in each pixel:
//
//     N=#comp     components
//       1           grey
//       2           grey, alpha
//       3           red, green, blue
//       4           red, green, blue, alpha
  int _channelsInFile;
  int _requiredChannels;
  ValueType _valueType;

  void* _out;
};

// TextureCompressor::TextureCompressor() {
//   _m = new Private{};
// }

// TextureCompressor::~TextureCompressor() {
//   free(_m->_image);
//   free(_m->_out);
//   delete _m;
// }

// bool TextureCompressor::Load(const char* filename, ValueType type, int channels) {
//   _m->_requiredChannels = channels;
//   _m->_valueType = type;
//   _m->_image = stbi_load(
//     filename, 
//     &(_m->_imgWidth), 
//     &(_m->_imgHeight), 
//     &(_m->_channelsInFile), 
//     _m->_requiredChannels
//   );

//   // TODO: Test if the image size is correct!
//   unsigned long long numChannels = _m->_requiredChannels == 0 ? _m->_channelsInFile : _m->_requiredChannels;
//   _m->_imgSize = _m->_imgWidth * _m->_imgHeight * numChannels;

//   if (!_m->_image) return false;
  
//   return true;
// }

bool TextureCompressor::Compress(const char* filename, const char* exportPath, const CompressOptions* settings) {

  nvtt::Format format = ToNVTTFormat(settings->format);
  nvtt::Quality quality = ToNVTTQuality(settings->quality);
  nvtt::AlphaMode alphaMode = ToNVTTAlphaMode(settings->alphaMode);
  bool isNormalMap = settings->isNormalMap;
  nvtt::MipmapFilter filter = ToNVTTMipMapFilter(settings->mipMapFilter);
  bool shouldGammaCorrect = settings->colorSpace == ColorSpace::sRGB;

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

// bool TextureCompressor::Export(const char *path) {
//   if (!_m->_out) return false;
//   // TODO: KTX2 image exportion?

//   return false;
// }
