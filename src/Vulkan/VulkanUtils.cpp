#include "VulkanUtils.hpp"
#include "VulkanLogger.hpp"

#include <vector>

bool isDeviceSuitable (VkPhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices  = findQueueFamilies (device, surface);
    bool               suitable = indices.isComplete ();
    VulkanLogger::log (suitable ? "Device is suitable." : "Device is not suitable.");
    return suitable;
}

QueueFamilyIndices
findQueueFamilies (VkPhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties (device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies (queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties (device, &queueFamilyCount, queueFamilies.data ());

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR (device, i, surface, &presentSupport);

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

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
