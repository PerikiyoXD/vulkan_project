#ifndef VULKAN_APP_HPP
#define VULKAN_APP_HPP

#include <GLFW/glfw3.h>
#include "VulkanCommandBuffers.hpp"
#include "VulkanDevice.hpp"
#include "VulkanFramebuffers.hpp"
#include "VulkanInstance.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanSurface.hpp"
#include "VulkanSwapChain.hpp"

class VulkanApp
{
  public:
    virtual void run ();  // Entry point for the application

  protected:
    virtual void initVulkan ();          // Initializes Vulkan (overridable)
    virtual void drawFrame ();           // Method to render a frame, meant to be overridden
    virtual void initVertexData () = 0;  // Pure virtual method for initializing vertex data
    void         cleanup ();             // Cleanup Vulkan resources

    // Members for Vulkan objects
    GLFWwindow*          window;
    VulkanInstance       instance;
    VulkanSurface        surface;
    VulkanDevice         device;
    VulkanSwapChain      swapChain;
    VulkanRenderPass     renderPass;
    VulkanPipeline       pipeline;
    VulkanFramebuffers   framebuffers;
    VulkanCommandBuffers commandBuffers;

    size_t    currentFrame         = 0;
    const int MAX_FRAMES_IN_FLIGHT = 2;

    // Other members for synchronization, etc.
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence>     inFlightFences;

    void createSyncObjects ();
    void cleanupSyncObjects ();

  private:
    void initWindow ();  // Initialize the GLFW window
};

#endif
