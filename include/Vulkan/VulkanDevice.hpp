#ifndef VULKAN_DEVICE_HPP
#define VULKAN_DEVICE_HPP

#include <vulkan/vulkan.h>
#include "VulkanSurface.hpp"

class VulkanDevice
{
  public:
    void             pickPhysicalDevice (VkInstance instance, VulkanSurface& surface);
    void             createLogicalDevice (VulkanSurface& surface);
    VkDevice         getDevice () const;
    VkPhysicalDevice getPhysicalDevice () const;
    VkQueue          getGraphicsQueue () const;

    void cleanup ();

  private:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice         device;
    VkQueue          graphicsQueue;

    bool isDeviceSuitable (VkPhysicalDevice device, VulkanSurface& surface);
};

#endif
