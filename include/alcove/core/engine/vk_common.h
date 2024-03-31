/**
 * @file vk.h
 * @author Goheung Choi (you@domain.com)
 * @brief Make sure to compile Vulkan library once. 
 *        Sets custom arguments and functions for the Vulkan engine.
 * @version 0.1
 * @date 2023-12-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <span>
#include <array>
#include <set>
#include <functional>
#include <deque>
#include <type_traits>
#include <algorithm>

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vk_mem_alloc.h>

#include <fmt/core.h>
#include <fmt/format.h>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

constexpr void VK_CHECK(VkResult err) {
  do {
    if (err) {
      fmt::print("Detected Vulkan error: {}", string_VkResult(err));
      abort();
    }
  } while (0);
}
