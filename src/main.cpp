#include "SpinningCircleApp.hpp"
#include "Vulkan/Logger.hpp"
#include <iostream>

int main()
{
    Vulkan::Logger::log("Starting Vulkan Spinning Circle Application...");

    try
    {
        Vulkan::Logger::log("Creating SpinningCircleApp instance...");
        SpinningCircleApp app;
        Vulkan::Logger::log("Running the application...");
        app.run();
        Vulkan::Logger::log("Application run completed successfully.");
    }
    catch (const std::exception &e)
    {
        Vulkan::Logger::logError("Exception caught: " + std::string(e.what()));
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    Vulkan::Logger::log("Application exiting normally.");
    return EXIT_SUCCESS;
}