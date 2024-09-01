#include "VulkanApp.hpp"
#include "VulkanLogger.hpp"

int main() {
  VulkanApp app;

  try {
    app.run();
  } catch (const std::exception &e) {
    VulkanLogger::logError("Error occurred: " + std::string(e.what()));
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
