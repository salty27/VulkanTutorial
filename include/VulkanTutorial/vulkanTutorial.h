#ifndef VULKANTUTORIAL_LIBRARY_H
#define VULKANTUTORIAL_LIBRARY_H

#include <vulkan/vulkan.hpp>
#include <SDL3/SDL_vulkan.h>

class vkt {
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
      };

    uint32_t flags = 0;
    vk::Instance instance;
    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    vk::Queue graphicsQueue;
    vk::Queue presentationQueue;
    VkSurfaceKHR surface;
    //vk::SurfaceKHR surface;
    QueueFamilyIndices indices;

    bool CreateInstance(char const* const *_extensions, uint32_t _count);
    bool CreatePhysicalDevice();
    bool CreateDevice();
    bool CreateSurface(SDL_Window& window);
    [[nodiscard]] bool CheckValidationLayerSupport(std::vector<const char*> const &_validationLayers);
    [[nodiscard]] bool CheckPhysicalDeviceSupport(vk::PhysicalDevice const &physicalDevice);
    bool FindQueueFamilies();

public:
    void init(char const* const *_extensions, uint32_t _count, SDL_Window *_window);
    void update();
    void cleanup();

    vk::Instance* getInstance();
    vk::Device* getDevice();
    VkSurfaceKHR* getSurface();
};
#endif //VULKANTUTORIAL_LIBRARY_H