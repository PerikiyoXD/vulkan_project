#ifndef SPINNING_CIRCLE_APP_HPP
#define SPINNING_CIRCLE_APP_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "App.hpp"

class SpinningCircleApp : public App
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
