#include <core/engine/vk_common.h>

#include <fstream>

namespace vkutil {

bool load_shader_module(
  const char* filepath,
  VkDevice device,
  VkShaderModule* outShaderModule
);

/**
 * @brief Configure the graphics pipeline layout and build a pipeline.
 * 
 */
class PipelineBuilder {
public:
  std::vector<VkPipelineShaderStageCreateInfo> _shader_stages;

  VkPipelineRenderingCreateInfo _render_info;
  VkPipelineInputAssemblyStateCreateInfo _input_assembly;
  VkPipelineRasterizationStateCreateInfo _rasterizer;
  VkPipelineColorBlendAttachmentState _color_blend_attachment;
  VkPipelineMultisampleStateCreateInfo _multisampling;
  VkPipelineDepthStencilStateCreateInfo _depth_stencil;
  VkPipelineLayout _pipeline_layout;
  VkFormat _color_attachment_format;

  PipelineBuilder() { clear(); }

  void set_shader(VkShaderStageFlagBits shaderStage, VkShaderModule shaderModule);
  
  void set_input_topology(VkPrimitiveTopology topology);

  /**
   * @brief Set the polygon mode in the rasterizer state.
   * 
   * @param mode Wireframe, solid rendering or point rendering
   */
  void set_polygon_mode(VkPolygonMode mode);

  /**
   * @brief Set the cull mode in the rasterizer state.
   * Set the front face, and opt for backface culling.
   * 
   * @param cullMode Backface culling, none, and etc.
   * @param frontFace Front face direction clockwise or counter-clockwise
   */
  void set_cull_mode(VkCullModeFlags cullMode, VkFrontFace frontFace);

  /**
   * @brief Set the multisampling to be disabled (No antialiasing).
   * 
   */
  void set_multisampling_none();

  /**
   * @brief Disable the color blend attachment.
   * 
   */
  void disable_blending();

  /**
   * @brief Set the color attachment format object.
   * 
   * @param colorAttachmentFormat 
   */
  void set_color_attachment_format(VkFormat colorAttachmentFormat);

  /**
   * @brief Set the depth format for depth testing.
   * 
   * @param depthFormat 
   */
  void set_depth_format(VkFormat depthFormat);

  /**
   * @brief Disable the depth testing.
   * 
   */
  void disable_depth_test();

  void clear();

  VkPipeline build_pipeline(VkDevice device);
};

}
