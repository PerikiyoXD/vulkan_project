#ifndef VULKAN_FRAMEBUFFERS_HPP
#define VULKAN_FRAMEBUFFERS_HPP

#include <vector>
#include <vulkan/vulkan.h>
#include "Device.hpp"
#include "RenderPass.hpp"
#include "SwapChain.hpp"

namespace Vulkan
{
    class Framebuffers
    {
      public:
        void                        create (Device& device, SwapChain& swapChain, RenderPass& renderPass);
        std::vector<VkFramebuffer>& getFramebuffers ();
        void                        cleanup (Device& device);

      private:
        std::vector<VkFramebuffer> swapChainFramebuffers;
    };
}  // namespace Vulkan

#endif  // VULKAN_FRAMEBUFFERS_HPP
