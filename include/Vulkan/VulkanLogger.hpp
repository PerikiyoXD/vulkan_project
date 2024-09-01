#ifndef VULKAN_LOGGER_HPP
#define VULKAN_LOGGER_HPP

#include <iostream>
#include <vulkan/vulkan.h>

class VulkanLogger
{
  public:
    static void
    log (const std::string& message)
    {
        std::cout << message << std::endl;
    }

    static void
    logError (const std::string& message, VkResult result)
    {
        std::cerr << message << " Error code: " << result << std::endl;
    }

    static void
    logError (const std::string& message)
    {
        std::cerr << message << std::endl;
    }
};

#endif
