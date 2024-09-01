#include "SwapChainSupportDetails.hpp"

SwapChainSupportDetails querySwapChainSupport (VkPhysicalDevice device, VkSurfaceKHR surface)
{
    SwapChainSupportDetails details;

    // Query the capabilities of the surface
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR (device, surface, &details.capabilities);

    // Query the supported surface formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR (device, surface, &formatCount, nullptr);
    if (formatCount != 0)
    {
        details.formats.resize (formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR (device, surface, &formatCount, details.formats.data ());
    }

    // Query the supported presentation modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR (device, surface, &presentModeCount, nullptr);
    if (presentModeCount != 0)
    {
        details.presentModes.resize (presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR (device, surface, &presentModeCount, details.presentModes.data ());
    }

    return details;
}
