#ifndef VULKAN_PHYSICAL_DEVICE_HPP
#define VULKAN_PHYSICAL_DEVICE_HPP

#include "QueueFamilyIndices.hpp"
#include "SwapChainSupportDetails.hpp"
#include <vulkan/vulkan.h>


namespace Vulkan
{
class PhysicalDevice
{
  public:
    void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
    VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) const;
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) const;
    bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;

  private:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) const;
};
} // namespace Vulkan

#endif // VULKAN_PHYSICAL_DEVICE_HPP
