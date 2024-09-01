#ifndef SPINNING_CIRCLE_APP_HPP
#define SPINNING_CIRCLE_APP_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "VulkanApp.hpp"

class SpinningCircleApp : public VulkanApp
{
  public:
    void initVertexData () override;
    void drawFrame () override;

  private:
    void createVertexBuffer ();
    void updateUniformBuffer (float rotationAngle);

    std::vector<glm::vec2> circleVertices;
    VkBuffer               vertexBuffer;
    VkDeviceMemory         vertexBufferMemory;

    float currentTime = 0.0f;  // Time in seconds
};

#endif
