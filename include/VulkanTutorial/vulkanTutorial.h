#ifndef VULKANTUTORIAL_LIBRARY_H
#define VULKANTUTORIAL_LIBRARY_H

#include <vulkan/vulkan.hpp>

class vkt {
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;

        bool isComplete() {
            return graphicsFamily.has_value();
        }
    };

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
      };

    uint32_t flags = 0;
    vk::Instance instance;
    vk::PhysicalDevice physicalDevice;

    static bool CreateInstance(vk::Instance &_out,char const* const *_extensions, uint32_t _count, std::vector<const char*> const &_validationLayers);
    static bool CreatePhysicalDevice(vk::PhysicalDevice &_out, const vk::Instance &_instance);
    //static [[nodiscard]] bool CreateDevice(vk::Device &_out, const vk::PhysicalDevice &_physicalDevice);
    [[nodiscard]] static bool CheckValidationLayerSupport(std::vector<const char*> const &_validationLayers);
    [[nodiscard]] static bool CheckPhysicalDeviceSupport(vk::PhysicalDevice const &physicalDevice);
    static bool FindQueueFamilies(vkt::QueueFamilyIndices &_out, vk::PhysicalDevice const &physicalDevice);

public:
    void init(char const* const *_extensions, uint32_t _count);
    void update();
    void cleanup();

    vk::Instance* getInstance();
    vk::Device* getDevice();
};
#endif //VULKANTUTORIAL_LIBRARY_H