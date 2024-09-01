#ifndef VULKAN_DEVICE_HPP
#define VULKAN_DEVICE_HPP

#include <vulkan/vulkan.h>
#include "PhysicalDevice.hpp"

namespace Vulkan
{
    class Device
    {
      public:
        void     createLogicalDevice (PhysicalDevice& physicalDevice, VkSurfaceKHR surface);
        VkDevice getDevice () const;
        VkQueue  getGraphicsQueue () const;
        VkQueue  getPresentQueue () const;

        void cleanup ();

      private:
        VkDevice device;
        VkQueue  graphicsQueue;
        VkQueue  presentQueue;
    };
}  // namespace Vulkan

#endif  // VULKAN_DEVICE_HPP
