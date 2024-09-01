#include "VulkanApp.hpp"
#include "VulkanLogger.hpp"
#include "VulkanUtils.hpp"

#include <vector>

void
VulkanApp::run ()
{
    initWindow ();
    initVulkan ();
    mainLoop ();
    cleanup ();
}

void
VulkanApp::initWindow ()
{
    if (!glfwInit ())
    {
        VulkanLogger::logError ("Failed to initialize GLFW!");
        exit (EXIT_FAILURE);
    }
    glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow (800, 600, "Vulkan Window", nullptr, nullptr);
    if (!window)
    {
        VulkanLogger::logError ("Failed to create GLFW window!");
        glfwTerminate ();
        exit (EXIT_FAILURE);
    }
    VulkanLogger::log ("GLFW window created successfully.");
}

void
VulkanApp::initVulkan ()
{
    createInstance ();
    createSurface ();
    pickPhysicalDevice ();
    createLogicalDevice ();
    createSwapChain ();
}

void
VulkanApp::mainLoop ()
{
    while (!glfwWindowShouldClose (window))
    {
        glfwPollEvents ();
    }
}

void
VulkanApp::cleanup ()
{
    vkDestroySwapchainKHR (device, swapChain, nullptr);
    vkDestroyDevice (device, nullptr);
    vkDestroySurfaceKHR (instance, surface, nullptr);
    vkDestroyInstance (instance, nullptr);
    glfwDestroyWindow (window);
    glfwTerminate ();
    VulkanLogger::log ("Cleaned up Vulkan resources and exited.");
}

void
VulkanApp::createInstance ()
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

    // Get required extensions from GLFW
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

void
VulkanApp::createSurface ()
{
    VkResult result = glfwCreateWindowSurface (instance, window, nullptr, &surface);
    if (result != VK_SUCCESS)
    {
        VulkanLogger::logError ("Failed to create window surface!", result);
        exit (EXIT_FAILURE);
    }
    VulkanLogger::log ("Vulkan surface created successfully.");
}

void
VulkanApp::pickPhysicalDevice ()
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

void
VulkanApp::createLogicalDevice ()
{
    QueueFamilyIndices indices = findQueueFamilies (physicalDevice, surface);

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

void
VulkanApp::createSwapChain ()
{
    // Placeholder for swap chain creation code
    VulkanLogger::log ("Swap chain creation placeholder.");
}
