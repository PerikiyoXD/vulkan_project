#ifndef VULKAN_SWAPCHAIN_HPP
#define VULKAN_SWAPCHAIN_HPP

#include <vector>
#include <vulkan/vulkan.h>
#include "Device.hpp"
#include "PhysicalDevice.hpp"
#include "Surface.hpp"

namespace Vulkan
{

    class SwapChain
    {
      public:
        void                      create (PhysicalDevice& physicalDevice, Device& device, Surface& surface);
        VkSwapchainKHR            getSwapChain () const { return swapChain; }
        VkFormat                  getImageFormat () const { return imageFormat; }
        VkExtent2D                getExtent () const { return extent; }
        std::vector<VkImageView>& getImageViews () { return imageViews; }

        void cleanup (Device& device);

      private:
        VkSwapchainKHR           swapChain;
        VkFormat                 imageFormat;
        VkExtent2D               extent;
        std::vector<VkImageView> imageViews;

        VkSurfaceFormatKHR chooseSwapSurfaceFormat (const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR   chooseSwapPresentMode (const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D         chooseSwapExtent (const VkSurfaceCapabilitiesKHR& capabilities);
    };

}  // namespace Vulkan

#endif  // VULKAN_SWAPCHAIN_HPP
