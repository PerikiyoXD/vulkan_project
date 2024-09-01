#include "Surface.hpp"
#include "Logger.hpp"

void Vulkan::Surface::create (VkInstance instance, GLFWwindow* window)
{
    VkResult result = glfwCreateWindowSurface (instance, window, nullptr, &surface);
    if (result != VK_SUCCESS)
    {
        Logger::logError ("Failed to create window surface!", result);
        exit (EXIT_FAILURE);
    }
    Logger::log ("Vulkan surface created successfully.");
}

VkSurfaceKHR Vulkan::Surface::getSurface () const
{
    return surface;
}

void Vulkan::Surface::cleanup (VkInstance instance)
{
    vkDestroySurfaceKHR (instance, surface, nullptr);
}
