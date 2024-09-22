#include <core/engine/vk_pipelines.h>

bool vkutil::load_shader_module(
  const char* filepath,
  VkDevice device,
  VkShaderModule* outShaderModule
)  {
  // Open the file with the cursor at the end
  std::ifstream file(filepath, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    return false;
  }

  // The location of the cursor tells the size of 
  // the file in bytes
  std::size_t filesize = static_cast<std::size_t>(file.tellg());

  // spirv expects the buffer to be on uint32_t,
  // so make sure to reserve a int vector big enough
  // for the entire file
  std::vector<uint32_t> buffer(filesize / sizeof(uint32_t));

  // Place the fie cursor at the beginning
  file.seekg(0);

  // Load the entire file into the buffer
  file.read((char*)buffer.data(), filesize);

  // Now that the file is loaded into the buffer, we can close it
  file.close();

  // Create a new shader module, using the buffer we loaded
  VkShaderModuleCreateInfo shaderCreateInfo {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext = nullptr,

    // Code size has to be in bytes,
    // so multiply the ints in the buffer by the size of int
    // to know the real size of the buffer
    .codeSize = buffer.size() * sizeof(uint32_t),
    .pCode = buffer.data()
  };

  // Check that the creation goes well
  VkShaderModule shaderModule;
  if (vkCreateShaderModule(device, &shaderCreateInfo, nullptr, &shaderModule) != VK_SUCCESS) {
    return false;
  }

  // Return the shader module handle
  *outShaderModule = shaderModule;
  return true;
}

void vkutil::PipelineBuilder::clear() {
  // clear all of the structurs we need back to 0 with their correct struct types

  _input_assembly = { .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };

  _rasterizer = { .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };

  _color_blend_attachment = {};

  _multisampling = { .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };

  _pipeline_layout = {};

  _depth_stencil = { .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };

  _render_info = { .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO };

  _shader_stages.clear();
}

void vkutil::PipelineBuilder::set_shader(VkShaderStageFlagBits shaderStage, VkShaderModule shaderModule) {
  // Set a shader stage
  VkPipelineShaderStageCreateInfo stageInfo {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .pNext = nullptr,
    .stage = shaderStage,
    .module = shaderModule,
    .pName = "main"
  };
  
  _shader_stages.push_back(stageInfo);
}

void vkutil::PipelineBuilder::set_input_topology(VkPrimitiveTopology topology) {
  _input_assembly.topology = topology;

  // NOTE: Disable the primitive restart just for now.
  // This only applies to indexed draws, triangle strips and line strips.
  // A special vertex index value is treated 
  // as restarting the seembly of primitives.
  _input_assembly.primitiveRestartEnable = VK_FALSE;
}

void vkutil::PipelineBuilder::set_polygon_mode(VkPolygonMode mode) {
  _rasterizer.polygonMode = mode;
  _rasterizer.lineWidth = 1.f;
}

void vkutil::PipelineBuilder::set_cull_mode(VkCullModeFlags cullMode, VkFrontFace frontFace) {
  _rasterizer.cullMode = cullMode;
  _rasterizer.frontFace = frontFace;
}

void vkutil::PipelineBuilder::set_multisampling_none() {
  _multisampling.sampleShadingEnable = VK_FALSE;

  // Multisampling defaulted to no multisampling (1 sample per pixel)
  _multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  _multisampling.minSampleShading = 1.f;
  _multisampling.pSampleMask = nullptr;

  // No alpha to coverage either
  _multisampling.alphaToCoverageEnable = VK_FALSE;
  _multisampling.alphaToOneEnable = VK_FALSE; 
}

void vkutil::PipelineBuilder::disable_blending() {
  // Default write mask to write pixel output to the attachement
  _color_blend_attachment.colorWriteMask = 
    VK_COLOR_COMPONENT_R_BIT |
    VK_COLOR_COMPONENT_G_BIT |
    VK_COLOR_COMPONENT_B_BIT |
    VK_COLOR_COMPONENT_A_BIT;

  // No blending
  _color_blend_attachment.blendEnable = VK_FALSE;
}

void vkutil::PipelineBuilder::enable_blending_additive() {
  _color_blend_attachment.colorWriteMask = 
    VK_COLOR_COMPONENT_R_BIT |
    VK_COLOR_COMPONENT_G_BIT |
    VK_COLOR_COMPONENT_B_BIT |
    VK_COLOR_COMPONENT_A_BIT;
  
  // Enable blending
  _color_blend_attachment.blendEnable = VK_TRUE;

  // Color blend formula
  _color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
  _color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
  _color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;

  // Alpha blend formula
  _color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  _color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  _color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;
}

void vkutil::PipelineBuilder::enable_blending_alphablend() {
  _color_blend_attachment.colorWriteMask = 
    VK_COLOR_COMPONENT_R_BIT |
    VK_COLOR_COMPONENT_G_BIT |
    VK_COLOR_COMPONENT_B_BIT |
    VK_COLOR_COMPONENT_A_BIT;

  // Enable blending
  _color_blend_attachment.blendEnable = VK_TRUE;

  // Color blend formula
  _color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
  _color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  _color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;

  // Alpha blend formula
  _color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  _color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  _color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;
}

void vkutil::PipelineBuilder::set_color_attachment_format(VkFormat colorAttachmentFormat) {
  _color_attachment_format = colorAttachmentFormat;

  // Connect the color attachment format to the render info
  // For deferred rendering, it needs an array of color attachments
  // to draw to multiple images at once.
  _render_info.colorAttachmentCount = 1;
  _render_info.pColorAttachmentFormats = &_color_attachment_format;
}

void vkutil::PipelineBuilder::set_depth_format(VkFormat depthFormat) {
  _render_info.depthAttachmentFormat = depthFormat;
}

void vkutil::PipelineBuilder::disable_depth_test() {
  _depth_stencil.depthTestEnable = VK_FALSE;
  _depth_stencil.depthWriteEnable = VK_FALSE;
  _depth_stencil.depthCompareOp = VK_COMPARE_OP_NEVER;
  _depth_stencil.depthBoundsTestEnable = VK_FALSE;
  _depth_stencil.stencilTestEnable = VK_FALSE;
  _depth_stencil.front = {};
  _depth_stencil.back = {};
  _depth_stencil.minDepthBounds = 0.f;
  _depth_stencil.maxDepthBounds = 1.f;
}

void vkutil::PipelineBuilder::enable_depth_test(bool depthWriteEnable, VkCompareOp op) {
  _depth_stencil.depthTestEnable = VK_TRUE;
  _depth_stencil.depthWriteEnable = depthWriteEnable;
  _depth_stencil.depthCompareOp = op;
  _depth_stencil.depthBoundsTestEnable = VK_FALSE;
  _depth_stencil.stencilTestEnable = VK_FALSE;
  _depth_stencil.front = {};
  _depth_stencil.back = {};
  _depth_stencil.minDepthBounds = 0.f;
  _depth_stencil.maxDepthBounds = 1.f;
}

VkPipeline vkutil::PipelineBuilder::build_pipeline(VkDevice device) {
  // Make viewport stage from our stored viewport and scissor
  // As we are using dynamic viewport state, 
  // no need to fill the viewport, or stencil options
  VkPipelineViewportStateCreateInfo viewportStateInfo {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    .pNext = nullptr,

    .viewportCount = 1,
    .scissorCount = 1
  };

  // Setup dynamic states of the viewport and scissor
  VkDynamicState dynamicStates[] = { 
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
  };
  VkPipelineDynamicStateCreateInfo dynamicInfo{
    .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
    .pNext = nullptr,
    
    .dynamicStateCount = std::size(dynamicStates),
    .pDynamicStates = dynamicStates,
  };

  // NOTE: Dummy color blending with no transparent objects
  // The blending is just "no blend", but we do write to the color attachment
  VkPipelineColorBlendStateCreateInfo colorBlending {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    .pNext = nullptr,

    .logicOpEnable = VK_FALSE,
    .logicOp = VK_LOGIC_OP_COPY,
    .attachmentCount = 1,
    .pAttachments = &_color_blend_attachment
  };

  // We don't use the vertex input state
  VkPipelineVertexInputStateCreateInfo vertexInputInfo {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO
  };

  // Build the graphics pipeline
  VkGraphicsPipelineCreateInfo graphicsPipelineInfo {
    .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    .pNext = &_render_info,

    .stageCount = static_cast<uint32_t>(_shader_stages.size()),
    .pStages = _shader_stages.data(),
    
    .pVertexInputState = &vertexInputInfo,
    .pInputAssemblyState = &_input_assembly,
    .pViewportState = &viewportStateInfo,
    .pRasterizationState = &_rasterizer,
    .pMultisampleState = &_multisampling,
    .pDepthStencilState = &_depth_stencil,
    .pColorBlendState = &colorBlending,
    .pDynamicState = &dynamicInfo,
    .layout = _pipeline_layout,
  };

  // It's easy to error out on create graphics pipeline, 
  // so we handle it a bit better than the common VK_CHECK case
  VkPipeline pipeline;
  if (
    vkCreateGraphicsPipelines(
      device, 
      VK_NULL_HANDLE,
      1,
      &graphicsPipelineInfo,
      nullptr,
      &pipeline
    ) != VK_SUCCESS
  ) {
    fmt::println("failed to create pipeline!");
    return VK_NULL_HANDLE;
  } else {
    return pipeline;
  }
}

