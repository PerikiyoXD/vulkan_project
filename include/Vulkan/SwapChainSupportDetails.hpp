#ifndef SWAPCHAIN_SUPPORT_DETAILS_HPP
#define SWAPCHAIN_SUPPORT_DETAILS_HPP

#include <vector>
#include <vulkan/vulkan.h>

namespace Vulkan
{
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR        capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   presentModes;
    };
}  // namespace Vulkan

#endif  // SWAPCHAIN_SUPPORT_DETAILS_HPP
