#ifndef VULKAN_PIPELINE_HPP
#define VULKAN_PIPELINE_HPP

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "VulkanDevice.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanSwapChain.hpp"

class VulkanPipeline
{
  public:
    void             create (VulkanDevice& device, VulkanSwapChain& swapChain, VulkanRenderPass& renderPass);
    VkPipeline       getPipeline () const;
    VkPipelineLayout getPipelineLayout () const;
    void             cleanup (VulkanDevice& device);

  private:
    VkPipeline       graphicsPipeline;
    VkPipelineLayout pipelineLayout;

    VkShaderModule    createShaderModule (const std::vector<char>& code, VulkanDevice& device);
    std::vector<char> readFile (const std::string& filename);
};

#endif
