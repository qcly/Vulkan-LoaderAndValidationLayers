///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2015-2016 The Khronos Group Inc.
// Copyright (c) 2015-2016 Valve Corporation
// Copyright (c) 2015-2016 LunarG, Inc.
// Copyright (c) 2015-2016 Google, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////

#define VK_PROTOTYPES
#include "vkjson.h"

#include <utility>

VkJsonDevice VkJsonGetDevice(VkPhysicalDevice physical_device) {
  VkJsonDevice device;
  vkGetPhysicalDeviceProperties(physical_device, &device.properties);
  vkGetPhysicalDeviceFeatures(physical_device, &device.features);
  vkGetPhysicalDeviceMemoryProperties(physical_device, &device.memory);

  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
                                           nullptr);
  if (queue_family_count > 0) {
    device.queues.resize(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(
        physical_device, &queue_family_count, device.queues.data());
  }

  // Only device extensions.
  // TODO(piman): do we want to show layer extensions?
  uint32_t extension_count = 0;
  vkEnumerateDeviceExtensionProperties(physical_device, nullptr,
                                       &extension_count, nullptr);
  if (extension_count > 0) {
    device.extensions.resize(extension_count);
    vkEnumerateDeviceExtensionProperties(
        physical_device, nullptr, &extension_count, device.extensions.data());
  }

  uint32_t layer_count = 0;
  vkEnumerateDeviceLayerProperties(physical_device, &layer_count, nullptr);
  if (layer_count > 0) {
    device.layers.resize(layer_count);
    vkEnumerateDeviceLayerProperties(physical_device, &layer_count,
                                     device.layers.data());
  }

  VkFormatProperties format_properties = {};
  for (VkFormat format = VK_FORMAT_R4G4_UNORM_PACK8;
       format <= VK_FORMAT_END_RANGE;
       format = static_cast<VkFormat>(format + 1)) {
    vkGetPhysicalDeviceFormatProperties(physical_device, format,
                                        &format_properties);
    if (format_properties.linearTilingFeatures ||
        format_properties.optimalTilingFeatures ||
        format_properties.bufferFeatures) {
      device.formats.insert(std::make_pair(format, format_properties));
    }
  }
  return device;
}
