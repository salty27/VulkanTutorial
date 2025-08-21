#include "VulkanTutorial/vulkanTutorial.h"

#include <iostream>
#include <ostream>
#include <set>
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
void vkt::init(char const* const *_extensions, uint32_t _count, SDL_Window *_window) {
    if (!CreateInstance(_extensions, _count)) {
        //Errors are already reported by other layer, just exit
        return;
    }
    if (!CreateSurface(*_window)) {
        return;
    }
    if (!CreatePhysicalDevice()) {
        return;
    }
    if (!CreateDevice()) {
        return;
    }
}

void vkt::update() {
}

void vkt::cleanup() {
    device.destroy();
}
vk::Instance* vkt::getInstance() {
    return flags & VKT_INIT_CREATE_INSTANCE ? &instance : nullptr;
}
vk::Device* vkt::getDevice() {
    return &device;
}

VkSurfaceKHR * vkt::getSurface() {
    return &surface;
}

bool vkt::CreateInstance(char const* const *_extensions, uint32_t _count) {
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
    if (!CheckValidationLayerSupport(validationLayers)) {
        std::cerr << "Validation layers requested, but not available\n";
        return false;
    }
    else {
        std::cout << "Validation layers available" << std::endl;
    }
    instanceCreateInfo.enabledLayerCount = validationLayers.size();
    instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
#else
    instanceCreateInfo.enabledLayerCount = 0;
    instanceCreateInfo.ppEnabledLayerNames = nullptr;
#endif
    instanceCreateInfo.enabledExtensionCount = _count;
    instanceCreateInfo.ppEnabledExtensionNames = _extensions;
    instance = vk::createInstance(instanceCreateInfo);
    LOG("Vulkan instance created");
    return true;
}
bool vkt::CreatePhysicalDevice() {
    LOG("Creating vulkan physical device");
    uint32_t physicalDeviceCount;
    if (instance.enumeratePhysicalDevices(&physicalDeviceCount, nullptr) != vk::Result::eSuccess) {
        std::cerr << "Unable to enumerate physical devices\n";
        return false;
    }
    if (physicalDeviceCount == 0) {
        std::cerr << "No physical device able to use Vulkan found\n";
        return false;
    }
    std::vector<vk::PhysicalDevice> physicalDevices(physicalDeviceCount);
    if (instance.enumeratePhysicalDevices(&physicalDeviceCount, physicalDevices.data()) != vk::Result::eSuccess) {
        std::cerr << "Unable to enumerate physical devices\n";
        return false;
    }
    bool found = false;
    auto it = physicalDevices.begin();
    //In order to pick, you can also rank them by a score function
    for (; !found && it != physicalDevices.end(); it++) {
        physicalDevice = *it;
        found = CheckPhysicalDeviceSupport(*it);
    }
    if (!found) {
        std::cerr << "No suitable physical device found\n";
        return false;
    }
    LOG("Vulkan physical device created");
    return true;
}
[[nodiscard]] bool vkt::CreateDevice() {
    LOG("Creating logical device");
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
    for (auto i : uniqueQueueFamilies) {
        vk::DeviceQueueCreateInfo deviceQueueCreateInfo;
        deviceQueueCreateInfo.sType = vk::StructureType::eDeviceQueueCreateInfo;
        deviceQueueCreateInfo.queueCount = 1;
        deviceQueueCreateInfo.queueFamilyIndex = i;
        float queuePriority = 1.0f;
        deviceQueueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(deviceQueueCreateInfo);
    }
    vk::PhysicalDeviceFeatures deviceFeatures {};
    vk::DeviceCreateInfo deviceCreateInfo;
    deviceCreateInfo.sType = vk::StructureType::eDeviceCreateInfo;
    deviceCreateInfo.enabledExtensionCount = 0;
#ifdef NDEBUG
    deviceCreateInfo.enabledLayerCount = validationLayers.size();
    deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
#else
    deviceCreateInfo.enabledLayerCount = 0;
#endif
    deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    device = physicalDevice.createDevice(deviceCreateInfo); //TODO: FIGURE OUT HOW TO DO ERROR CHECKING WITH CONSTRUCTORS
    graphicsQueue = device.getQueue(indices.graphicsFamily.value(), 0);
    presentationQueue = device.getQueue(indices.presentFamily.value(), 0);
    LOG("Logical device created");
    return true;
}

bool vkt::CreateSurface(SDL_Window& window) {
    LOG("Creating surface");
    return SDL_Vulkan_CreateSurface(&window, static_cast<VkInstance>(instance), nullptr, &surface);
    LOG("Surface created");
}

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
    FindQueueFamilies();
    return indices.graphicsFamily.has_value();
}
bool vkt::FindQueueFamilies() {
    auto queueFamilies = physicalDevice.getQueueFamilyProperties();
    bool found = false;
    vk::Bool32 presentSupport = false;
    for (uint32_t i = 0; !found && i < queueFamilies.size(); i++) {
        if (physicalDevice.getSurfaceSupportKHR(i, surface, &presentSupport) != vk::Result::eSuccess) {
            throw;
        }
        if (presentSupport) {
            indices.presentFamily = i;
        }
        if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphicsFamily = i;
        }
        found = indices.isComplete();
    }
    return indices.isComplete();
}