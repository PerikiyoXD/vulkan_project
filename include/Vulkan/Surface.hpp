#ifndef VULKAN_SURFACE_HPP
#define VULKAN_SURFACE_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

namespace Vulkan
{
    class Surface
    {
      public:
        void         create (VkInstance instance, GLFWwindow* window);
        VkSurfaceKHR getSurface () const;
        void         cleanup (VkInstance instance);

      private:
        VkSurfaceKHR surface;
    };
}  // namespace Vulkan

#endif  // VULKAN_SURFACE_HPP
