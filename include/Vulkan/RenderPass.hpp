#ifndef VULKAN_RENDER_PASS_HPP
#define VULKAN_RENDER_PASS_HPP

#include <vulkan/vulkan.h>
#include "Device.hpp"
#include "SwapChain.hpp"

namespace Vulkan
{
    class RenderPass
    {
      public:
        void         create (Device& device, SwapChain& swapChain);
        VkRenderPass getRenderPass () const;
        void         cleanup (Device& device);

      private:
        VkRenderPass renderPass;
    };
}  // namespace Vulkan

#endif  // VULKAN_RENDER_PASS_HPP
