#include "VulkanInstance.hpp"
#include "VulkanLogger.hpp"

void VulkanInstance::create ()
{
    VkApplicationInfo appInfo{};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName   = "Hello Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION (1, 0, 0);
    appInfo.pEngineName        = "No Engine";
    appInfo.engineVersion      = VK_MAKE_VERSION (1, 0, 0);
    appInfo.apiVersion         = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t     glfwExtensionCount    = 0;
    const char** glfwExtensions        = glfwGetRequiredInstanceExtensions (&glfwExtensionCount);
    createInfo.enabledExtensionCount   = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    VkResult result = vkCreateInstance (&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS)
    {
        VulkanLogger::logError ("Failed to create Vulkan instance!", result);
        exit (EXIT_FAILURE);
    }
    VulkanLogger::log ("Vulkan instance created successfully.");
}

VkInstance VulkanInstance::getInstance () const
{
    return instance;
}

void VulkanInstance::cleanup ()
{
    vkDestroyInstance (instance, nullptr);
}
