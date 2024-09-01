#ifndef VULKAN_UTILS_HPP
#define VULKAN_UTILS_HPP

#include <optional>
#include <vulkan/vulkan.h>

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool
    isComplete ()
    {
        return graphicsFamily.has_value () && presentFamily.has_value ();
    }
};

bool
isDeviceSuitable (VkPhysicalDevice device, VkSurfaceKHR surface);
QueueFamilyIndices
findQueueFamilies (VkPhysicalDevice device, VkSurfaceKHR surface);

#endif
