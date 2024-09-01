#ifndef VULKAN_RENDER_PASS_HPP
#define VULKAN_RENDER_PASS_HPP

#include <vulkan/vulkan.h>
#include "VulkanDevice.hpp"
#include "VulkanSwapChain.hpp"

class VulkanRenderPass
{
  public:
    void         create (VulkanDevice& device, VulkanSwapChain& swapChain);
    VkRenderPass getRenderPass () const;
    void         cleanup (VulkanDevice& device);

  private:
    VkRenderPass renderPass;
};

#endif
