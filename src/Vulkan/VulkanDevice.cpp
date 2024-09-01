#include "VulkanDevice.hpp"

void VulkanDevice::pickPhysicalDevice (VkInstance instance, VulkanSurface& surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices (instance, &deviceCount, nullptr);
    if (deviceCount == 0)
    {
        VulkanLogger::logError ("Failed to find GPUs with Vulkan support!");
        exit (EXIT_FAILURE);
    }

    std::vector<VkPhysicalDevice> devices (deviceCount);
    vkEnumeratePhysicalDevices (instance, &deviceCount, devices.data ());

    for (const auto& device : devices)
    {
        if (isDeviceSuitable (device, surface))
        {
            physicalDevice = device;
            VulkanLogger::log ("Selected suitable GPU for Vulkan.");
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
    {
        VulkanLogger::logError ("Failed to find a suitable GPU!");
        exit (EXIT_FAILURE);
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

    float                   queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value ();
    queueCreateInfo.queueCount       = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos    = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    VkResult result = vkCreateDevice (physicalDevice, &createInfo, nullptr, &device);
    if (result != VK_SUCCESS)
    {
        VulkanLogger::logError ("Failed to create logical device!", result);
        exit (EXIT_FAILURE);
    }

    vkGetDeviceQueue (device, indices.graphicsFamily.value (), 0, &graphicsQueue);
    VulkanLogger::log ("Logical device and graphics queue created successfully.");
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

void VulkanDevice::cleanup ()
{
    vkDestroyDevice (device, nullptr);
}
