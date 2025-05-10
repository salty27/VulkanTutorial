#include "VulkanTutorial/vulkanTutorial.h"

#include <iostream>
#include <ostream>
#include <vulkan/vulkan.hpp>
#define VKT_INIT_CREATE_INSTANCE 0x1

#ifdef NDEBUG
#define LOG(message) std::cout << message << std::endl
#define DEBUGFUNCTION(function) function
#else
#define LOG(message)
#define DEBUGFUNCTION(function)
#endif


//TODO: do we need flags for init phase anymore?
void vkt::init(char const* const *_extensions, uint32_t _count) {
    if (!CreateInstance(instance, _extensions, _count, validationLayers)) {
        //Errors are already reported by other layer, just exit
        return;
    }
    if (!CreatePhysicalDevice(physicalDevice, instance)) {
        return;
    }
    /*if (!CreateDevice(device, physicalDevice)) {
        return;
    }*/
}

void vkt::update() {
}

void vkt::cleanup() {
}
vk::Instance* vkt::getInstance() {
    return flags & VKT_INIT_CREATE_INSTANCE ? &instance : nullptr;
}
vk::Device* vkt::getDevice() {
    return nullptr;
}

bool vkt::CreateInstance(vk::Instance &_out,char const* const *_extensions, uint32_t _count, std::vector<const char*> const &_validationLayers) {
    LOG("Creating vulkan instance");
    vk::ApplicationInfo appInfo;
    appInfo.sType = vk::StructureType::eApplicationInfo;
    appInfo.pApplicationName = "VulkanTutorial";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "VulkanTutorial";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_4;
    vk::InstanceCreateInfo instanceCreateInfo;
    instanceCreateInfo.sType = vk::StructureType::eInstanceCreateInfo;
    instanceCreateInfo.pApplicationInfo = &appInfo;

#ifdef NDEBUG
    uint32_t count;
    if (vk::enumerateInstanceExtensionProperties(nullptr, &count, nullptr) != vk::Result::eSuccess) {
        std::cerr << "Unable to count instance extension properties\n";
        return false;
    }
    std::vector<vk::ExtensionProperties> extensions(count);
    if (vk::enumerateInstanceExtensionProperties(nullptr, &count, extensions.data()) != vk::Result::eSuccess) {
        std::cerr << "Unable to enumerate instance extension properties\n";
        return false;
    }

    std::cout << "Instance extensions:" << std::endl;
    for (auto & extension : extensions) {
        std::cout << extension.extensionName << std::endl;
    }
    if (!CheckValidationLayerSupport(_validationLayers)) {
        std::cerr << "Validation layers requested, but not available\n";
        return false;
    }
    else {
        std::cout << "Validation layers available" << std::endl;
    }
    instanceCreateInfo.enabledLayerCount = _validationLayers.size();
    instanceCreateInfo.ppEnabledLayerNames = _validationLayers.data();
#else
    instanceCreateInfo.enabledLayerCount = 0;
    instanceCreateInfo.ppEnabledLayerNames = nullptr;
#endif
    instanceCreateInfo.enabledExtensionCount = _count;
    instanceCreateInfo.ppEnabledExtensionNames = _extensions;
    _out = vk::createInstance(instanceCreateInfo);
    LOG("Vulkan instance created");
    return true;
}
bool vkt::CreatePhysicalDevice(vk::PhysicalDevice &_out, const vk::Instance &_instance) {
    LOG("Creating vulkan physical device");
    uint32_t physicalDeviceCount;
    if (_instance.enumeratePhysicalDevices(&physicalDeviceCount, nullptr) != vk::Result::eSuccess) {
        std::cerr << "Unable to enumerate physical devices\n";
        return false;
    }
    if (physicalDeviceCount == 0) {
        std::cerr << "No physical device able to use Vulkan found\n";
        return false;
    }
    std::vector<vk::PhysicalDevice> physicalDevices(physicalDeviceCount);
    if (_instance.enumeratePhysicalDevices(&physicalDeviceCount, physicalDevices.data()) != vk::Result::eSuccess) {
        std::cerr << "Unable to enumerate physical devices\n";
        return false;
    }
    bool found = false;
    auto it = physicalDevices.begin();
    //In order to pick, you can also rank them by a score function
    for (; !found && it != physicalDevices.end(); it++) {
        found = CheckPhysicalDeviceSupport(*it);
    }
    if (!found) {
        std::cerr << "No suitable physical device found\n";
        return false;
    }
    _out = *it;
    LOG("Vulkan physical device created");
    return true;
}
//[[nodiscard]] bool vkt::CreateDevice(vk::Device &_out, const vk::PhysicalDevice &_physicalDevice) {}
[[nodiscard]] bool vkt::CheckValidationLayerSupport(std::vector<const char*> const &_validationLayers) {
    uint32_t layerCount;
    if (vk::enumerateInstanceLayerProperties(&layerCount, nullptr) != vk::Result::eSuccess) {
        return false;
    }
    std::vector<vk::LayerProperties> layers(layerCount);
    if (vk::enumerateInstanceLayerProperties(&layerCount, layers.data()) != vk::Result::eSuccess) {
        return false;
    }

    bool layerFound = true;
    for (auto validationLayer = _validationLayers.begin(); layerFound && validationLayer != _validationLayers.end(); validationLayer++) {
        layerFound = false;
        for (auto i = layers.begin(); !layerFound && i != layers.end(); i++) {
            layerFound = strcmp(*validationLayer, i->layerName) == 0;
        }
    }

    return layerFound;
}
[[nodiscard]] bool vkt::CheckPhysicalDeviceSupport(vk::PhysicalDevice const &physicalDevice) {
    QueueFamilyIndices indices;
    FindQueueFamilies(indices, physicalDevice);
    return indices.graphicsFamily.has_value();
}
bool vkt::FindQueueFamilies(QueueFamilyIndices &_out, vk::PhysicalDevice const &physicalDevice) {
    QueueFamilyIndices indices;

    auto queueFamilies = physicalDevice.getQueueFamilyProperties();
    bool found = false;
    for (uint32_t i = 0; !found && i < queueFamilies.size(); i++) {
        if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphicsFamily = i;
        }
        found = indices.isComplete();
    }

    _out = indices;
    return true;
}