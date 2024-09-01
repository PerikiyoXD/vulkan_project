#ifndef VULKAN_PIPELINE_HPP
#define VULKAN_PIPELINE_HPP

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "Device.hpp"
#include "RenderPass.hpp"
#include "SwapChain.hpp"

namespace Vulkan
{
    class Pipeline
    {
      public:
        void             create (Device& device, SwapChain& swapChain, RenderPass& renderPass);
        VkPipeline       getPipeline () const;
        VkPipelineLayout getPipelineLayout () const;
        void             cleanup (Device& device);

      private:
        VkPipeline       graphicsPipeline;
        VkPipelineLayout pipelineLayout;

        VkShaderModule    createShaderModule (const std::vector<char>& code, Device& device);
        std::vector<char> readFile (const std::string& filename);
    };
}  // namespace Vulkan

#endif  // VULKAN_PIPELINE_HPP
