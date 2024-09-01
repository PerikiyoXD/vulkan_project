#include "Device.hpp"
#include <set>
#include <stdexcept>
#include <vector>
#include "Logger.hpp"

void Vulkan::Device::createLogicalDevice (PhysicalDevice& physicalDevice, VkSurfaceKHR surface)
{
    QueueFamilyIndices                   indices = physicalDevice.findQueueFamilies (surface);
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
    VkDeviceCreateInfo       createInfo{};
    createInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t> (queueCreateInfos.size ());
    createInfo.pQueueCreateInfos    = queueCreateInfos.data ();
    createInfo.pEnabledFeatures     = &deviceFeatures;

    VkResult result = vkCreateDevice (physicalDevice.getPhysicalDevice (), &createInfo, nullptr, &device);
    if (result != VK_SUCCESS)
    {
        Logger::logError ("Failed to create logical device!", result);
        throw std::runtime_error ("Failed to create logical device!");
    }

    vkGetDeviceQueue (device, indices.graphicsFamily.value (), 0, &graphicsQueue);
    vkGetDeviceQueue (device, indices.presentFamily.value (), 0, &presentQueue);
}

VkDevice Vulkan::Device::getDevice () const
{
    return device;
}

VkQueue Vulkan::Device::getGraphicsQueue () const
{
    return graphicsQueue;
}

VkQueue Vulkan::Device::getPresentQueue () const
{
    return presentQueue;
}

void Vulkan::Device::cleanup ()
{
    vkDestroyDevice (device, nullptr);
}
