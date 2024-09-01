#ifndef VULKAN_INSTANCE_HPP
#define VULKAN_INSTANCE_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

class VulkanInstance
{
  public:
    void       create ();
    VkInstance getInstance () const;

    void cleanup ();

  private:
    VkInstance instance;
};

#endif
