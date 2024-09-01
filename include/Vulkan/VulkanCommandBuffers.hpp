#ifndef VULKAN_COMMAND_BUFFERS_HPP
#define VULKAN_COMMAND_BUFFERS_HPP

#include <vector>
#include <vulkan/vulkan.h>
#include "VulkanDevice.hpp"
#include "VulkanFramebuffers.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanSwapChain.hpp"

class VulkanCommandBuffers
{
  public:
    void                          create (VulkanDevice& device, VulkanSwapChain& swapChain, VulkanRenderPass& renderPass, VulkanPipeline& pipeline, VulkanFramebuffers& framebuffers, VulkanSurface& surface);
    std::vector<VkCommandBuffer>& getCommandBuffers ();
    void                          cleanup (VulkanDevice& device);

  private:
    VkCommandPool                commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
};

#endif
