#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "App.hpp"
#include "Logger.hpp"

class SpinningCircleApp : public Vulkan::App
{
  public:
    void
    initVertexData ()
    {
        int   numLights = 16;
        float radius    = 0.5f;

        for (int i = 0; i < numLights; ++i)
        {
            float angle = i * 2.0f * glm::pi<float> () / numLights;
            circleVertices.push_back (glm::vec2 (radius * cos (angle), radius * sin (angle)));
        }

        // Create vertex buffer
        createVertexBuffer ();
    }

    void
    createVertexBuffer ()
    {
        VkDeviceSize bufferSize = sizeof (circleVertices[0]) * circleVertices.size ();

        // Create and fill the buffer here using Vulkan commands
        // For simplicity, this part of the code is omitted
        // Assume a VkBuffer vertexBuffer and VkDeviceMemory vertexBufferMemory are created and
        // filled
    }

    void
    updateUniformBuffer (float rotationAngle)
    {
        // Update the push constant with the current rotation angle
        // This function will update the Vulkan command to set the rotation angle for the vertex
        // shader
        VkCommandBuffer commandBuffer = commandBuffers.getCommandBuffers ()[currentFrame];

        vkCmdPushConstants (commandBuffer,
                            pipeline.getPipelineLayout (),
                            VK_SHADER_STAGE_VERTEX_BIT,
                            0,
                            sizeof (rotationAngle),
                            &rotationAngle);
    }

    void
    drawFrame ()
    {
        // Calculate rotation angle
        float rotationAngle = currentTime * glm::radians (90.0f);  // 90 degrees per second

        // Update the uniform buffer with the new rotation angle
        updateUniformBuffer (rotationAngle);

        // Call the base App drawFrame to handle rendering and presentation
        App::drawFrame ();
    }

  protected:
    std::vector<glm::vec2> circleVertices;
    size_t                 currentFrame = 0;
    float                  currentTime  = 0.0f;  // Time in seconds

    // Implement other necessary functions and overrides...
};

int main ()
{
    SpinningCircleApp app;

    try
    {
        app.run ();
    }
    catch (const std::exception& e)
    {
        Vulkan::Logger::logError ("Application error: " + std::string (e.what ()));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
