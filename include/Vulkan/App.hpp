#ifndef VULKAN_APP_HPP
#define VULKAN_APP_HPP

#include <GLFW/glfw3.h>
#include "CommandBuffers.hpp"
#include "Device.hpp"
#include "Framebuffers.hpp"
#include "Instance.hpp"
#include "PhysicalDevice.hpp"
#include "Pipeline.hpp"
#include "RenderPass.hpp"
#include "Surface.hpp"
#include "SwapChain.hpp"

namespace Vulkan
{
    class App
    {
      public:
        virtual void run ();  // Entry point for the application

      protected:
        virtual void initVulkan ();          // Initializes Vulkan (overridable)
        virtual void drawFrame ();           // Method to render a frame, meant to be overridden
        virtual void initVertexData () = 0;  // Pure virtual method for initializing vertex data
        void         cleanup ();             // Cleanup Vulkan resources

        // Members for Vulkan objects
        GLFWwindow*    window;
        Instance       instance;
        Surface        surface;
        Device         device;
        PhysicalDevice physicalDevice;
        SwapChain      swapChain;
        RenderPass     renderPass;
        Pipeline       pipeline;
        Framebuffers   framebuffers;
        CommandBuffers commandBuffers;

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
}  // namespace Vulkan

#endif  // VULKAN_APP_HPP
