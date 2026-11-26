// Copyright 2023 Borys Boiko

#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace e172vp {

struct Extension {
    static std::vector<std::string> presentInstanceExtensions();
    static std::vector<std::string> presentDeviceExtensions(vk::PhysicalDevice physicalDevice);
};

} // namespace e172vp
