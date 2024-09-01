#include "VulkanSurface.hpp"

void VulkanSurface::create (VkInstance instance, GLFWwindow* window)
{
    VkResult result = glfwCreateWindowSurface (instance, window, nullptr, &surface);
    if (result != VK_SUCCESS)
    {
        VulkanLogger::logError ("Failed to create window surface!", result);
        exit (EXIT_FAILURE);
    }
    VulkanLogger::log ("Vulkan surface created successfully.");
}

VkSurfaceKHR VulkanSurface::getSurface () const
{
    return surface;
}

void VulkanSurface::cleanup (VkInstance instance)
{
    vkDestroySurfaceKHR (instance, surface, nullptr);
}
