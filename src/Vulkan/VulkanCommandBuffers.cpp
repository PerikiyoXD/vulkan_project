#include "VulkanCommandBuffers.hpp"
#include "VulkanLogger.hpp"
#include "VulkanUtils.hpp"

void VulkanCommandBuffers::create (VulkanDevice& device, VulkanSwapChain& swapChain, VulkanRenderPass& renderPass, VulkanPipeline& pipeline, VulkanFramebuffers& framebuffers, VulkanSurface& surface)
{
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType             = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        QueueFamilyIndices indices = findQueueFamilies (device.getPhysicalDevice (), surface.getSurface ());
        if (vkCreateCommandPool (device.getDevice (), &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
        {
            VulkanLogger::logError ("Failed to create command pool!");
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
            VulkanLogger::logError ("Failed to allocate command buffers!");
            throw std::runtime_error ("Failed to allocate command buffers!");
        }

        for (size_t i = 0; i < commandBuffers.size (); i++)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

            if (vkBeginCommandBuffer (commandBuffers[i], &beginInfo) != VK_SUCCESS)
            {
                VulkanLogger::logError ("Failed to begin recording command buffer!");
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
                VulkanLogger::logError ("Failed to record command buffer!");
                throw std::runtime_error ("Failed to record command buffer!");
            }
        }

        VulkanLogger::log ("Command buffers created successfully.");
    }
}

std::vector<VkCommandBuffer>& VulkanCommandBuffers::getCommandBuffers ()
{
    return commandBuffers;
}

void VulkanCommandBuffers::cleanup (VulkanDevice& device)
{
    vkDestroyCommandPool (device.getDevice (), commandPool, nullptr);
}
