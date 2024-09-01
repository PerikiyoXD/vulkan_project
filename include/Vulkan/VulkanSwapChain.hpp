#ifndef VULKAN_SWAPCHAIN_HPP
#define VULKAN_SWAPCHAIN_HPP

#include <vector>
#include <vulkan/vulkan.h>
#include "VulkanDevice.hpp"
#include "VulkanSurface.hpp"

class VulkanSwapChain
{
  public:
    void                      create (VulkanDevice& device, VulkanSurface& surface);
    VkSwapchainKHR            getSwapChain () const;
    VkFormat                  getImageFormat () const;
    VkExtent2D                getExtent () const;
    std::vector<VkImageView>& getImageViews ();

    void cleanup (VulkanDevice& device);

  private:
    VkSwapchainKHR           swapChain;
    VkFormat                 imageFormat;
    VkExtent2D               extent;
    std::vector<VkImageView> imageViews;

    VkSurfaceFormatKHR chooseSwapSurfaceFormat (const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR   chooseSwapPresentMode (const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D         chooseSwapExtent (const VkSurfaceCapabilitiesKHR& capabilities);
};

#endif
