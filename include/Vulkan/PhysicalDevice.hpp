#ifndef VULKAN_PHYSICAL_DEVICE_HPP
#define VULKAN_PHYSICAL_DEVICE_HPP

#include <vulkan/vulkan.h>
#include "QueueFamilyIndices.hpp"
#include "SwapChainSupportDetails.hpp"

namespace Vulkan
{
    class PhysicalDevice
    {
      public:
        void             pickPhysicalDevice (VkInstance instance, VkSurfaceKHR surface);
        VkPhysicalDevice getPhysicalDevice () const { return physicalDevice; }

        QueueFamilyIndices      findQueueFamilies (VkSurfaceKHR surface) const;
        SwapChainSupportDetails querySwapChainSupport (VkSurfaceKHR surface) const;

      private:
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

        bool isDeviceSuitable (VkPhysicalDevice device, VkSurfaceKHR surface) const;
    };
}  // namespace Vulkan

#endif  // VULKAN_PHYSICAL_DEVICE_HPP
