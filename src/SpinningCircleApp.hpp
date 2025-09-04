#ifndef SPINNING_CIRCLE_APP_HPP
#define SPINNING_CIRCLE_APP_HPP

#include "Vulkan/App.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>


class SpinningCircleApp : public Vulkan::App
{
  public:
    void initVertexData() override;
    void drawFrame() override;
    void cleanup();

  private:
    void createVertexBuffer();
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    std::vector<glm::vec2> circleVertices;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    float currentTime = 0.0f; // Time in seconds
};

#endif
