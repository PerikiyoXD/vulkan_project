#include "Framebuffers.hpp"
#include "Logger.hpp"

void Vulkan::Framebuffers::create (Device& device, SwapChain& swapChain, RenderPass& renderPass)
{
    swapChainFramebuffers.resize (swapChain.getImageViews ().size ());

    for (size_t i = 0; i < swapChain.getImageViews ().size (); i++)
    {
        VkImageView attachments[] = {
            swapChain.getImageViews ()[i]};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass      = renderPass.getRenderPass ();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments    = attachments;
        framebufferInfo.width           = swapChain.getExtent ().width;
        framebufferInfo.height          = swapChain.getExtent ().height;
        framebufferInfo.layers          = 1;

        if (vkCreateFramebuffer (device.getDevice (), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
        {
            Logger::logError ("Failed to create framebuffer!");
            exit (EXIT_FAILURE);
        }
    }

    Logger::log ("Framebuffers created successfully.");
}

std::vector<VkFramebuffer>& Vulkan::Framebuffers::getFramebuffers ()
{
    return swapChainFramebuffers;
}

void Vulkan::Framebuffers::cleanup (Device& device)
{
    for (auto framebuffer : swapChainFramebuffers)
    {
        vkDestroyFramebuffer (device.getDevice (), framebuffer, nullptr);
    }
}
