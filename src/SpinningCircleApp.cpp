#include "SpinningCircleApp.hpp"
#include "Vulkan/Logger.hpp"
#include "Vulkan/QueueFamilyIndices.hpp"
#include <chrono>
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
    recordCommandBuffers();
    Vulkan::Logger::log("Vertex buffer created and command buffers recorded successfully");
}

void SpinningCircleApp::drawFrame()
{
    // Update time for rotation
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTimePoint = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTimePoint - startTime).count();

    Vulkan::Logger::log("Drawing frame with rotation angle: " + std::to_string(time));

    // Update vertex positions for rotation
    updateVertexBuffer(time);

    // Call base drawFrame for rendering
    Vulkan::App::drawFrame();

    Vulkan::Logger::log("Frame rendered successfully.");
}

void SpinningCircleApp::updateVertexBuffer(float time)
{
    // Generate rotated circle vertices
    const int numSegments = 64;
    const float radius = 0.5f;
    std::vector<glm::vec2> rotatedVertices;
    
    for (int i = 0; i < numSegments; ++i)
    {
        float angle = 2.0f * 3.14159f * i / numSegments + time; // Add rotation
        rotatedVertices.push_back(glm::vec2(radius * cos(angle), radius * sin(angle)));
    }

    // Update the vertex buffer
    VkDeviceSize bufferSize = sizeof(rotatedVertices[0]) * rotatedVertices.size();
    void* data;
    VkResult result = vkMapMemory(device.getDevice(), vertexBufferMemory, 0, bufferSize, 0, &data);
    if (result != VK_SUCCESS) {
        Vulkan::Logger::logError("Failed to map vertex buffer memory for update", result);
        throw std::runtime_error("Failed to map vertex buffer memory for update!");
    }

    memcpy(data, rotatedVertices.data(), (size_t)bufferSize);
    vkUnmapMemory(device.getDevice(), vertexBufferMemory);
}

void SpinningCircleApp::recordCommandBuffers()
{
    for (size_t i = 0; i < commandBuffers.getCommandBuffers().size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

        if (vkBeginCommandBuffer(commandBuffers.getCommandBuffers()[i], &beginInfo) != VK_SUCCESS) {
            Vulkan::Logger::logError("Failed to begin recording command buffer!");
            throw std::runtime_error("Failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass.getRenderPass();
        renderPassInfo.framebuffer = framebuffers.getFramebuffers()[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain.getExtent();

        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffers.getCommandBuffers()[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffers.getCommandBuffers()[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipeline());

        VkBuffer vertexBuffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffers.getCommandBuffers()[i], 0, 1, vertexBuffers, offsets);

        vkCmdDraw(commandBuffers.getCommandBuffers()[i], static_cast<uint32_t>(circleVertices.size()), 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers.getCommandBuffers()[i]);

        if (vkEndCommandBuffer(commandBuffers.getCommandBuffers()[i]) != VK_SUCCESS) {
            Vulkan::Logger::logError("Failed to record command buffer!");
            throw std::runtime_error("Failed to record command buffer!");
        }
    }
    Vulkan::Logger::log("Command buffers recorded for spinning circle.");
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
