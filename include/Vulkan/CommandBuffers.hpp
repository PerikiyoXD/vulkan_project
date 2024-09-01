#ifndef VULKAN_COMMAND_BUFFERS_HPP
#define VULKAN_COMMAND_BUFFERS_HPP

#include <vector>
#include <vulkan/vulkan.h>
#include "Device.hpp"
#include "Framebuffers.hpp"
#include "Pipeline.hpp"
#include "RenderPass.hpp"
#include "SwapChain.hpp"

namespace Vulkan
{
    class CommandBuffers
    {
      public:
        void                          create (PhysicalDevice& physicalDevice, Device& device, SwapChain& swapChain, RenderPass& renderPass, Pipeline& pipeline, Framebuffers& framebuffers, Surface& surface);
        std::vector<VkCommandBuffer>& getCommandBuffers ();
        void                          cleanup (Device& device);

      private:
        VkCommandPool                commandPool;
        std::vector<VkCommandBuffer> commandBuffers;
    };
}  // namespace Vulkan

#endif  // VULKAN_COMMAND_BUFFERS_HPP
