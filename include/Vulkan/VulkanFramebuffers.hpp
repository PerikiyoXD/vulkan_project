#ifndef VULKAN_FRAMEBUFFERS_HPP
#define VULKAN_FRAMEBUFFERS_HPP

#include <vector>
#include <vulkan/vulkan.h>
#include "VulkanDevice.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanSwapChain.hpp"

class VulkanFramebuffers
{
  public:
    void                        create (VulkanDevice& device, VulkanSwapChain& swapChain, VulkanRenderPass& renderPass);
    std::vector<VkFramebuffer>& getFramebuffers ();
    void                        cleanup (VulkanDevice& device);

  private:
    std::vector<VkFramebuffer> swapChainFramebuffers;
};

#endif
