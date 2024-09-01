#include "CommandBuffers.hpp"
#include "Logger.hpp"

void Vulkan::CommandBuffers::create (PhysicalDevice& physicalDevice, Device& device, SwapChain& swapChain, RenderPass& renderPass, Pipeline& pipeline, Framebuffers& framebuffers, Surface& surface)
{
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType             = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        QueueFamilyIndices indices = physicalDevice.findQueueFamilies (surface.getSurface ());
        if (vkCreateCommandPool (device.getDevice (), &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
        {
            Logger::logError ("Failed to create command pool!");
            throw std::runtime_error ("Failed to create command pool!");
        }

        commandBuffers.resize (framebuffers.getFramebuffers ().size ());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool        = commandPool;
        allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<uint32_t> (commandBuffers.size ());

        if (vkAllocateCommandBuffers (device.getDevice (), &allocInfo, commandBuffers.data ()) != VK_SUCCESS)
        {
            Logger::logError ("Failed to allocate command buffers!");
            throw std::runtime_error ("Failed to allocate command buffers!");
        }

        for (size_t i = 0; i < commandBuffers.size (); i++)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

            if (vkBeginCommandBuffer (commandBuffers[i], &beginInfo) != VK_SUCCESS)
            {
                Logger::logError ("Failed to begin recording command buffer!");
                throw std::runtime_error ("Failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass        = renderPass.getRenderPass ();
            renderPassInfo.framebuffer       = framebuffers.getFramebuffers ()[i];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swapChain.getExtent ();

            VkClearValue clearColor        = {0.0f, 0.0f, 0.0f, 1.0f};
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues    = &clearColor;

            vkCmdBeginRenderPass (commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline (commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipeline ());

            vkCmdDraw (commandBuffers[i], 3, 1, 0, 0);

            vkCmdEndRenderPass (commandBuffers[i]);

            if (vkEndCommandBuffer (commandBuffers[i]) != VK_SUCCESS)
            {
                Logger::logError ("Failed to record command buffer!");
                throw std::runtime_error ("Failed to record command buffer!");
            }
        }

        Logger::log ("Command buffers created successfully.");
    }
}

std::vector<VkCommandBuffer>& Vulkan::CommandBuffers::getCommandBuffers ()
{
    return commandBuffers;
}

void Vulkan::CommandBuffers::cleanup (Device& device)
{
    vkDestroyCommandPool (device.getDevice (), commandPool, nullptr);
}
