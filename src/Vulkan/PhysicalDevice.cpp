#include "Vulkan/PhysicalDevice.hpp"
#include "Vulkan/Logger.hpp"
#include <stdexcept>
#include <vector>
#include <string>
#include <set>

void Vulkan::PhysicalDevice::pickPhysicalDevice(VkInstance instance,
                                                VkSurfaceKHR surface)
{
    Vulkan::Logger::log("Picking physical device...");

    uint32_t deviceCount = 0;
    VkResult result = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (result != VK_SUCCESS) {
        Vulkan::Logger::logError("Failed to enumerate physical devices (count)", result);
        throw std::runtime_error("Failed to enumerate physical devices!");
    }

    Vulkan::Logger::log("Found " + std::to_string(deviceCount) + " physical devices.");

    if (deviceCount == 0)
    {
        Vulkan::Logger::logError("No GPUs with Vulkan support found!");
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    result = vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    if (result != VK_SUCCESS) {
        Vulkan::Logger::logError("Failed to enumerate physical devices (list)", result);
        throw std::runtime_error("Failed to enumerate physical devices!");
    }

    for (const auto &device : devices)
    {
        Vulkan::Logger::log("Checking device suitability...");
        if (isDeviceSuitable(device, surface))
        {
            physicalDevice = device;
            Vulkan::Logger::log("Suitable physical device found.");
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
    {
        Vulkan::Logger::logError("No suitable GPU found!");
        throw std::runtime_error("Failed to find a suitable GPU!");
    }

    Vulkan::Logger::log("Physical device selected successfully.");
}

bool Vulkan::PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device,
                                              VkSurfaceKHR surface) const
{
    QueueFamilyIndices indices = findQueueFamilies(device, surface);
    bool extensionsSupported = checkDeviceExtensionSupport(device);

    return indices.isComplete() && extensionsSupported;
}

bool Vulkan::PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) const
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

Vulkan::QueueFamilyIndices
Vulkan::PhysicalDevice::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) const
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                             nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                             queueFamilies.data());

    int i = 0;
    for (const auto &queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface,
                                             &presentSupport);

        if (presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete())
        {
            break;
        }

        i++;
    }

    return indices;
}

Vulkan::SwapChainSupportDetails
Vulkan::PhysicalDevice::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) const
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                              &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                         nullptr);

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface,
                                              &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface,
                                                  &presentModeCount,
                                                  details.presentModes.data());
    }

    return details;
}