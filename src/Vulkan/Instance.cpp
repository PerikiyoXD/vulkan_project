#include "Instance.hpp"
#include <vector>
#include "Logger.hpp"


// Define the validation layers you want to use
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

// Function to check if the required validation layers are available
bool checkValidationLayerSupport ()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties (&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers (layerCount);
    vkEnumerateInstanceLayerProperties (&layerCount, availableLayers.data ());

    for (const char* layerName : validationLayers)
    {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp (layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}

void Vulkan::Instance::create ()
{
    // Setup application info
    VkApplicationInfo appInfo{};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName   = "Hello Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION (1, 0, 0);
    appInfo.pEngineName        = "No Engine";
    appInfo.engineVersion      = VK_MAKE_VERSION (1, 0, 0);
    appInfo.apiVersion         = VK_API_VERSION_1_0;

    // Setup instance creation info
    VkInstanceCreateInfo createInfo{};
    createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Get required GLFW extensions
    uint32_t     glfwExtensionCount    = 0;
    const char** glfwExtensions        = glfwGetRequiredInstanceExtensions (&glfwExtensionCount);
    createInfo.enabledExtensionCount   = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    // Enable validation layers if in debug mode and if supported
    bool enableValidationLayers = true;  // You can toggle this based on your build config

    if (enableValidationLayers && checkValidationLayerSupport ())
    {
        createInfo.enabledLayerCount   = static_cast<uint32_t> (validationLayers.size ());
        createInfo.ppEnabledLayerNames = validationLayers.data ();
    }
    else
    {
        createInfo.enabledLayerCount   = 0;
        createInfo.ppEnabledLayerNames = nullptr;
    }

    // Create the Vulkan instance
    VkResult result = vkCreateInstance (&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS)
    {
        Logger::logError ("Failed to create Vulkan instance!", result);
        exit (EXIT_FAILURE);
    }
    Logger::log ("Vulkan instance created successfully.");
}

VkInstance Vulkan::Instance::getInstance () const
{
    return instance;
}

void Vulkan::Instance::cleanup ()
{
    vkDestroyInstance (instance, nullptr);
}
