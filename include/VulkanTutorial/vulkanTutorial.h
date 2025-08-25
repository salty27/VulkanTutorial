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
    struct SwapChainSupportDetails {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
      };
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    uint32_t flags = 0;
    vk::Instance instance;
    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    vk::Queue graphicsQueue;
    vk::Queue presentationQueue;
    VkSurfaceKHR surface;
    SDL_Window* window;
    //vk::SurfaceKHR surface;
    QueueFamilyIndices indices;
    SwapChainSupportDetails details;
    vk::Format swapchainFormat;
    vk::SurfaceFormatKHR surfaceFormat;
    vk::PresentModeKHR presentMode;
    vk::Extent2D swapchainExtent;
    vk::SwapchainKHR swapchain;
    std::vector<vk::Image> images;

    bool CreateInstance(char const* const *_extensions, uint32_t _count);
    bool CreatePhysicalDevice();
    bool CreateDevice();
    bool CreateSurface(SDL_Window& window);
    bool CreateSwapchain();
    [[nodiscard]] bool CheckValidationLayerSupport(std::vector<const char*> const &_validationLayers);
    [[nodiscard]] bool CheckPhysicalDeviceSupport(vk::PhysicalDevice const &physicalDevice);
    bool checkDeviceExtensionSupport();
    bool checkSwapChainSupport();
    bool FindQueueFamilies();
    bool FindSwapSurfaceFormat();
    bool FindPresentMode();
    bool FindExtent();

public:
    void init(char const* const *_extensions, uint32_t _count, SDL_Window *_window);
    void update();
    void cleanup();

    vk::Instance* getInstance();
    vk::Device* getDevice();
    VkSurfaceKHR* getSurface();
};
#endif //VULKANTUTORIAL_LIBRARY_H