#include "SpinningCircleApp.hpp"
#include "Vulkan/Logger.hpp"
#include "Vulkan/QueueFamilyIndices.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

void SpinningCircleApp::initVertexData()
{
    // Generate circle vertices
    const int numSegments = 64;
    const float radius = 0.5f;
    circleVertices.clear();
    for (int i = 0; i < numSegments; ++i)
    {
        float angle = 2.0f * 3.14159f * i / numSegments;
        circleVertices.push_back(glm::vec2(radius * cos(angle), radius * sin(angle)));
    }

    Vulkan::Logger::log("Circle vertices generated: " + std::to_string(circleVertices.size()));
    createVertexBuffer();
    Vulkan::Logger::log("Vertex buffer created successfully");
}

void SpinningCircleApp::drawFrame()
{
    static int frameCount = 0;
    Vulkan::Logger::log("Drawing frame: " + std::to_string(++frameCount));

    // For now, just call base drawFrame
    Vulkan::App::drawFrame();

    Vulkan::Logger::log("Frame " + std::to_string(frameCount) + " completed.");
}

void SpinningCircleApp::createVertexBuffer()
{
    Vulkan::Logger::log("Creating vertex buffer...");
    VkDeviceSize bufferSize = sizeof(circleVertices[0]) * circleVertices.size();

    createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                 vertexBuffer, vertexBufferMemory);

    void* data;
    VkResult result = vkMapMemory(device.getDevice(), vertexBufferMemory, 0, bufferSize, 0, &data);
    if (result != VK_SUCCESS) {
        Vulkan::Logger::logError("Failed to map vertex buffer memory", result);
        throw std::runtime_error("Failed to map vertex buffer memory!");
    }
    memcpy(data, circleVertices.data(), (size_t)bufferSize);
    vkUnmapMemory(device.getDevice(), vertexBufferMemory);

    Vulkan::Logger::log("Vertex buffer created and data copied successfully.");
}

void SpinningCircleApp::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    Vulkan::Logger::log("Creating buffer of size: " + std::to_string(size));

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult result = vkCreateBuffer(device.getDevice(), &bufferInfo, nullptr, &buffer);
    if (result != VK_SUCCESS) {
        Vulkan::Logger::logError("Failed to create buffer", result);
        throw std::runtime_error("Failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device.getDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    result = vkAllocateMemory(device.getDevice(), &allocInfo, nullptr, &bufferMemory);
    if (result != VK_SUCCESS) {
        Vulkan::Logger::logError("Failed to allocate buffer memory", result);
        throw std::runtime_error("Failed to allocate buffer memory!");
    }

    result = vkBindBufferMemory(device.getDevice(), buffer, bufferMemory, 0);
    if (result != VK_SUCCESS) {
        Vulkan::Logger::logError("Failed to bind buffer memory", result);
        throw std::runtime_error("Failed to bind buffer memory!");
    }

    Vulkan::Logger::log("Buffer created successfully.");
}

uint32_t SpinningCircleApp::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice.getPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type!");
}

void SpinningCircleApp::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    Vulkan::QueueFamilyIndices queueFamilyIndices = physicalDevice.findQueueFamilies(physicalDevice.getPhysicalDevice(), surface.getSurface());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    VkCommandPool tempCommandPool;
    if (vkCreateCommandPool(device.getDevice(), &poolInfo, nullptr, &tempCommandPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create command pool!");
    }

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = tempCommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device.getDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(device.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device.getGraphicsQueue());

    vkFreeCommandBuffers(device.getDevice(), tempCommandPool, 1, &commandBuffer);
    vkDestroyCommandPool(device.getDevice(), tempCommandPool, nullptr);
}

void SpinningCircleApp::cleanup()
{
    vkDestroyBuffer(device.getDevice(), vertexBuffer, nullptr);
    vkFreeMemory(device.getDevice(), vertexBufferMemory, nullptr);
    Vulkan::App::cleanup();
}
