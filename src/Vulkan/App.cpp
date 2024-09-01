#include "App.hpp"
#include <stdexcept>
#include <vector>
#include "Logger.hpp"
#include "PhysicalDevice.hpp"

void Vulkan::App::run ()
{
    initWindow ();
    initVulkan ();
    createSyncObjects ();

    // Main loop
    while (!glfwWindowShouldClose (window))
    {
        glfwPollEvents ();
        drawFrame ();
    }

    vkDeviceWaitIdle (device.getDevice ());

    cleanup ();
}

void Vulkan::App::initWindow ()
{
    if (!glfwInit ())
    {
        Logger::logError ("Failed to initialize GLFW!");
        throw std::runtime_error ("Failed to initialize GLFW!");
    }

    glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow (800, 600, "Vulkan Window", nullptr, nullptr);
    if (!window)
    {
        Logger::logError ("Failed to create GLFW window!");
        glfwTerminate ();
        throw std::runtime_error ("Failed to create GLFW window!");
    }

    Logger::log ("GLFW window created successfully.");
}

void Vulkan::App::initVulkan ()
{
    instance.create ();
    surface.create (instance.getInstance (), window);
    physicalDevice.pickPhysicalDevice (instance.getInstance (), surface.getSurface ());
    device.createLogicalDevice (physicalDevice, surface.getSurface ());
    swapChain.create (physicalDevice, device, surface);
    renderPass.create (device, swapChain);
    pipeline.create (device, swapChain, renderPass);
    framebuffers.create (device, swapChain, renderPass);
    commandBuffers.create (physicalDevice, device, swapChain, renderPass, pipeline, framebuffers, surface);
}

void Vulkan::App::drawFrame ()
{
    vkWaitForFences (device.getDevice (), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    vkResetFences (device.getDevice (), 1, &inFlightFences[currentFrame]);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR (device.getDevice (), swapChain.getSwapChain (), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        Logger::log ("Swap chain out of date, rebuilding...");
        // Handle swap chain recreation here (not implemented in this base class)
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error ("Failed to acquire swap chain image!");
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore          waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[]     = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount         = 1;
    submitInfo.pWaitSemaphores            = waitSemaphores;
    submitInfo.pWaitDstStageMask          = waitStages;

    VkCommandBuffer commandBuffers[] = {this->commandBuffers.getCommandBuffers ()[imageIndex]};
    submitInfo.commandBufferCount    = 1;
    submitInfo.pCommandBuffers       = commandBuffers;

    VkSemaphore signalSemaphores[]  = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;

    if (vkQueueSubmit (device.getGraphicsQueue (), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error ("Failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain.getSwapChain ()};
    presentInfo.swapchainCount  = 1;
    presentInfo.pSwapchains     = swapChains;
    presentInfo.pImageIndices   = &imageIndex;

    result = vkQueuePresentKHR (device.getPresentQueue (), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || glfwWindowShouldClose (window))
    {
        Logger::log ("Swap chain out of date or suboptimal, rebuilding...");
        // Handle swap chain recreation here (not implemented in this base class)
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error ("Failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Vulkan::App::createSyncObjects ()
{
    imageAvailableSemaphores.resize (MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize (MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize (MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (vkCreateSemaphore (device.getDevice (), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS || vkCreateSemaphore (device.getDevice (), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS || vkCreateFence (device.getDevice (), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
        {
            throw std::runtime_error ("Failed to create synchronization objects for a frame!");
        }
    }
}

void Vulkan::App::cleanupSyncObjects ()
{
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore (device.getDevice (), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore (device.getDevice (), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence (device.getDevice (), inFlightFences[i], nullptr);
    }
}

void Vulkan::App::cleanup ()
{
    cleanupSyncObjects ();

    commandBuffers.cleanup (device);
    framebuffers.cleanup (device);
    pipeline.cleanup (device);
    renderPass.cleanup (device);
    swapChain.cleanup (device);
    device.cleanup ();
    surface.cleanup (instance.getInstance ());
    instance.cleanup ();

    if (window != nullptr)
    {
        glfwDestroyWindow (window);
        glfwTerminate ();
    }
}
