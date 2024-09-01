#ifndef VULKAN_QUEUE_FAMILY_INDICES_HPP
#define VULKAN_QUEUE_FAMILY_INDICES_HPP

#include <optional>
#include <vulkan/vulkan.h>

namespace Vulkan
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete () const
        {
            return graphicsFamily.has_value () && presentFamily.has_value ();
        }
    };
}  // namespace Vulkan

#endif  // VULKAN_QUEUE_FAMILY_INDICES_HPP