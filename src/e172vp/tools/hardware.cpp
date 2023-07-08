#include "hardware.h"

#include "extensiontools.h"
#include "stringvector.h"

#include <e172/debug.h>

bool e172vp::Hardware::isDeviceSuitable(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, const std::vector<std::string> &requiredDeviceExtensions) {
    const auto missing = StringVector::subtract(requiredDeviceExtensions, Extension::presentDeviceExtensions(physicalDevice));
    bool swapChainAdequate = false;
    const auto features = physicalDevice.getFeatures();
    if (missing.size() == 0) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    return swapChainAdequate && missing.size() == 0 && features.samplerAnisotropy;
}

e172vp::Hardware::SwapChainSupportDetails e172vp::Hardware::querySwapChainSupport(const vk::PhysicalDevice &physicalDevice, vk::SurfaceKHR surface) {
    SwapChainSupportDetails details;
    if (physicalDevice.getSurfaceCapabilitiesKHR(surface, &details.capabilities)
        != vk::Result::eSuccess) {
        e172::Debug::warning("failed to get surface capabilities");
    }

    //GETTING FORMATS
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        if (physicalDevice.getSurfaceFormatsKHR(surface, &formatCount, details.formats.data())
            != vk::Result::eSuccess) {
            e172::Debug::warning("failed to get surface formats");
        }
    }
    //GETTING FORMATS END

    //GETTING PRESENT MODES
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        if (physicalDevice.getSurfacePresentModesKHR(surface,
                                                     &presentModeCount,
                                                     details.presentModes.data())
            != vk::Result::eSuccess) {
            e172::Debug::warning("failed to get surface present modes");
        }
    }
    //GETTING PRESENT MODES END

    return details;
}

vk::PhysicalDevice e172vp::Hardware::findSuitablePhysicalDevice(const vk::Instance &instance, vk::SurfaceKHR surface, const std::vector<std::string> &requiredDeviceExtensions) {
    const auto devices = instance.enumeratePhysicalDevices();
    for(const auto& d : devices) {
        if(isDeviceSuitable(d, surface, requiredDeviceExtensions))
            return d;
    }
    return nullptr;
}




e172vp::Hardware::QueueFamilies e172vp::Hardware::queryQueueFamilies(const vk::PhysicalDevice &physicalDevice, const vk::SurfaceKHR &surface) {
    QueueFamilies result;
    const auto queueFamilies = physicalDevice.getQueueFamilyProperties();

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        vk::Bool32 presentSupport = false;
        if (physicalDevice.getSurfaceSupportKHR(i, surface, &presentSupport)
            != vk::Result::eSuccess) {
            e172::Debug::warning("failed to get surface support");
        }

        if(presentSupport)
            result.m_presentFamily = i;

        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
            result.m_graphicsFamily = i;

        if(result.isValid())
            break;

        i++;
    }

    return result;
}


