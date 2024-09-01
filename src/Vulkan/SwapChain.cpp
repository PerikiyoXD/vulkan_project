#include "SwapChain.hpp"
#include "Logger.hpp"
#include "SwapChainSupportDetails.hpp"

void Vulkan::SwapChain::create (PhysicalDevice& physicalDevice, Device& device, Surface& surface)
{
    SwapChainSupportDetails swapChainSupport = physicalDevice.querySwapChainSupport (surface.getSurface ());

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat (swapChainSupport.formats);
    VkPresentModeKHR   presentMode   = chooseSwapPresentMode (swapChainSupport.presentModes);
    VkExtent2D         extent        = chooseSwapExtent (swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface          = surface.getSurface ();
    createInfo.minImageCount    = imageCount;
    createInfo.imageFormat      = surfaceFormat.format;
    createInfo.imageColorSpace  = surfaceFormat.colorSpace;
    createInfo.imageExtent      = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices              = physicalDevice.findQueueFamilies (surface.getSurface ());
    uint32_t           queueFamilyIndices[] = {indices.graphicsFamily.value (), indices.presentFamily.value ()};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices   = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices   = nullptr;
    }

    createInfo.preTransform   = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode    = presentMode;
    createInfo.clipped        = VK_TRUE;
    createInfo.oldSwapchain   = VK_NULL_HANDLE;

    VkResult result = vkCreateSwapchainKHR (device.getDevice (), &createInfo, nullptr, &swapChain);
    if (result != VK_SUCCESS)
    {
        Logger::logError ("Failed to create swap chain!", result);
        exit (EXIT_FAILURE);
    }

    vkGetSwapchainImagesKHR (device.getDevice (), swapChain, &imageCount, nullptr);
    std::vector<VkImage> swapChainImages (imageCount);
    vkGetSwapchainImagesKHR (device.getDevice (), swapChain, &imageCount, swapChainImages.data ());

    imageFormat  = surfaceFormat.format;
    this->extent = extent;

    imageViews.resize (swapChainImages.size ());

    for (size_t i = 0; i < swapChainImages.size (); i++)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image                           = swapChainImages[i];
        createInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format                          = imageFormat;
        createInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel   = 0;
        createInfo.subresourceRange.levelCount     = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount     = 1;

        if (vkCreateImageView (device.getDevice (), &createInfo, nullptr, &imageViews[i]) != VK_SUCCESS)
        {
            Logger::logError ("Failed to create image views!");
            exit (EXIT_FAILURE);
        }
    }

    Logger::log ("Swap chain created successfully.");
}

VkSurfaceFormatKHR Vulkan::SwapChain::chooseSwapSurfaceFormat (const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }
    return availableFormats[0];  // Fallback to the first format if the preferred one isn't found
}

VkPresentModeKHR Vulkan::SwapChain::chooseSwapPresentMode (const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;  // FIFO is guaranteed to be available
}

VkExtent2D Vulkan::SwapChain::chooseSwapExtent (const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
        return capabilities.currentExtent;
    }
    else
    {
        VkExtent2D actualExtent = {800, 600};  // Choose your preferred default size here
        actualExtent.width      = std::max (capabilities.minImageExtent.width, std::min (capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height     = std::max (capabilities.minImageExtent.height, std::min (capabilities.maxImageExtent.height, actualExtent.height));
        return actualExtent;
    }
}

void Vulkan::SwapChain::cleanup (Device& device)
{
    for (auto imageView : imageViews)
    {
        vkDestroyImageView (device.getDevice (), imageView, nullptr);
    }

    vkDestroySwapchainKHR (device.getDevice (), swapChain, nullptr);
}