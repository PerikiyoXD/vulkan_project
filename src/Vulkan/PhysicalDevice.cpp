#include "PhysicalDevice.hpp"
#include <stdexcept>
#include <vector>

void Vulkan::PhysicalDevice::pickPhysicalDevice (VkInstance instance, VkSurfaceKHR surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices (instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error ("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices (deviceCount);
    vkEnumeratePhysicalDevices (instance, &deviceCount, devices.data ());

    for (const auto& device : devices)
    {
        if (isDeviceSuitable (device, surface))
        {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error ("Failed to find a suitable GPU!");
    }
}

bool Vulkan::PhysicalDevice::isDeviceSuitable (VkPhysicalDevice device, VkSurfaceKHR surface) const
{
    QueueFamilyIndices indices             = findQueueFamilies (surface);
    bool               extensionsSupported = true;  // Add extension support checks here if needed

    return indices.isComplete () && extensionsSupported;
}

Vulkan::QueueFamilyIndices Vulkan::PhysicalDevice::findQueueFamilies (VkSurfaceKHR surface) const
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties (physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies (queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties (physicalDevice, &queueFamilyCount, queueFamilies.data ());

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR (physicalDevice, i, surface, &presentSupport);

        if (presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete ())
        {
            break;
        }

        i++;
    }

    return indices;
}

Vulkan::SwapChainSupportDetails Vulkan::PhysicalDevice::querySwapChainSupport (VkSurfaceKHR surface) const
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR (physicalDevice, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR (physicalDevice, surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.formats.resize (formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR (physicalDevice, surface, &formatCount, details.formats.data ());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR (physicalDevice, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize (presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR (physicalDevice, surface, &presentModeCount, details.presentModes.data ());
    }

    return details;
}