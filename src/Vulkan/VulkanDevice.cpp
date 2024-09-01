#include "VulkanDevice.hpp"
#include <set>
#include <vector>
#include "VulkanLogger.hpp"
#include "VulkanUtils.hpp"

void VulkanDevice::pickPhysicalDevice (VkInstance instance, VulkanSurface& surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices (instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error ("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices (deviceCount);
    vkEnumeratePhysicalDevices (instance, &deviceCount, devices.data ());

    for (const auto& device : devices)
    {
        if (isDeviceSuitable (device, surface) && checkDeviceExtensionSupport (device))
        {
            physicalDevice = device;
            VulkanLogger::log ("Selected suitable GPU for Vulkan.");
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error ("Failed to find a suitable GPU!");
    }
}

bool VulkanDevice::isDeviceSuitable (VkPhysicalDevice device, VulkanSurface& surface)
{
    QueueFamilyIndices indices = findQueueFamilies (device, surface.getSurface ());
    return indices.isComplete ();
}

void VulkanDevice::createLogicalDevice (VulkanSurface& surface)
{
    QueueFamilyIndices indices = findQueueFamilies (physicalDevice, surface.getSurface ());

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t>                   uniqueQueueFamilies = {indices.graphicsFamily.value (), indices.presentFamily.value ()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount       = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back (queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    // Enable the VK_KHR_swapchain extension
    std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount    = static_cast<uint32_t> (queueCreateInfos.size ());
    createInfo.pQueueCreateInfos       = queueCreateInfos.data ();
    createInfo.pEnabledFeatures        = &deviceFeatures;
    createInfo.enabledExtensionCount   = static_cast<uint32_t> (deviceExtensions.size ());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data ();

    // Optional: Check for validation layers (not needed in release)
    // if (enableValidationLayers)
    // {
    //     createInfo.enabledLayerCount   = static_cast<uint32_t> (validationLayers.size ());
    //     createInfo.ppEnabledLayerNames = validationLayers.data ();
    // }
    // else
    // {
    createInfo.enabledLayerCount = 0;
    // }

    if (vkCreateDevice (physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
    {
        throw std::runtime_error ("Failed to create logical device!");
    }

    vkGetDeviceQueue (device, indices.graphicsFamily.value (), 0, &graphicsQueue);
    vkGetDeviceQueue (device, indices.presentFamily.value (), 0, &presentQueue);

    VulkanLogger::log ("Logical device and queues created successfully.");
}

bool VulkanDevice::checkDeviceExtensionSupport (VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties (device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions (extensionCount);
    vkEnumerateDeviceExtensionProperties (device, nullptr, &extensionCount, availableExtensions.data ());

    std::set<std::string> requiredExtensions (deviceExtensions.begin (), deviceExtensions.end ());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase (extension.extensionName);
    }

    return requiredExtensions.empty ();
}

VkDevice VulkanDevice::getDevice () const
{
    return device;
}

VkPhysicalDevice VulkanDevice::getPhysicalDevice () const
{
    return physicalDevice;
}

VkQueue VulkanDevice::getGraphicsQueue () const
{
    return graphicsQueue;
}

VkQueue VulkanDevice::getPresentQueue () const
{
    return presentQueue;
}

void VulkanDevice::cleanup ()
{
    vkDestroyDevice (device, nullptr);
}
