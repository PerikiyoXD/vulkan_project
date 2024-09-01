#include "VulkanSwapChain.hpp"
#include "SwapChainSupportDetails.hpp"

void VulkanSwapChain::create (VulkanDevice& device, VulkanSurface& surface)
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport (device.getPhysicalDevice (), surface.getSurface ());

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

    QueueFamilyIndices indices              = findQueueFamilies (device.getPhysicalDevice (), surface.getSurface ());
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
        VulkanLogger::logError ("Failed to create swap chain!", result);
        exit (EXIT_FAILURE);
    }

    vkGetSwapchainImagesKHR (device.getDevice (), swapChain, &imageCount, nullptr);
    std::vector<VkImage> swapChainImages (imageCount);
    vkGetSwapchainImagesKHR (device.getDevice (), swapChain, &imageCount, swapChainImages.data ());

    imageFormat = surfaceFormat.format;
    extent      = extent;

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
            VulkanLogger::logError ("Failed to create image views!");
            exit (EXIT_FAILURE);
        }
    }

    VulkanLogger::log ("Swap chain created successfully.");
}

VkSwapchainKHR VulkanSwapChain::getSwapChain () const
{
    return swapChain;
}

VkFormat VulkanSwapChain::getImageFormat () const
{
    return imageFormat;
}

VkExtent2D VulkanSwapChain::getExtent () const
{
    return extent;
}

std::vector<VkImageView>& VulkanSwapChain::getImageViews ()
{
    return imageViews;
}

void VulkanSwapChain::cleanup (VulkanDevice& device)
{
    for (auto imageView : imageViews)
    {
        vkDestroyImageView (device.getDevice (), imageView, nullptr);
    }
    vkDestroySwapchainKHR (device.getDevice (), swapChain, nullptr);
}
