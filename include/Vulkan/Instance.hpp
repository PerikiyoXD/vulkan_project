#ifndef VULKAN_INSTANCE_HPP
#define VULKAN_INSTANCE_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace Vulkan
{
    class Instance
    {
      public:
        void       create ();
        VkInstance getInstance () const;

        void cleanup ();

      private:
        VkInstance instance;
    };
}  // namespace Vulkan

#endif  // VULKAN_INSTANCE_HPP
