#include <core/Modules/VkRenderModule.h>
#include <core/VkWindowRenderer.h>
#include <core/CoreConfig.h>

#include <iostream>
#include <set>

namespace core {

VkRenderModule::VkRenderModule()
: IRenderModule("Render",
                "Vulkan",
                new VkWindowRenderer(this))
{

}
VkRenderModule::~VkRenderModule() {
    Cleanup();
}
void VkRenderModule::Init(Window& window) {
    AddValidationLayers();
    CheckValidationLayerSupport();
    AddDebugExtensions();
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface(window);
    PickPhysicalDevice();
    CreateLogicalDevice();
    Logger::Log(RENDER, INFO) << "Initialized VkRenderModule";
    initialized = true;
}
void VkRenderModule::Cleanup() {
    delete windowRenderer;

    vkDestroyDevice(device, nullptr);

    if ( validationLayersEnabled ) {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
}
void VkRenderModule::Frame() {

}
void VkRenderModule::PickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        Logger::Log(RENDER, ERR_HERE) << "No GPUs with Vulkan support found";
        throw std::runtime_error("No GPUs with Vulkan support found");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& d: devices) {
        if (IsDeviceSuitable(d)) {
            physicalDevice = d;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        Logger::Log(RENDER, ERR_HERE) << "No suitable GPUs found";
        throw std::runtime_error("No suitable GPUs found");
    }

}
void VkRenderModule::CreateInstance() {
    VkApplicationInfo appInfo { };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = CoreConfig::AppInfo::name.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(CoreConfig::AppInfo::majorVersion,
                                                 CoreConfig::AppInfo::minorVersion,
                                                 CoreConfig::AppInfo::patchVersion);
    appInfo.pEngineName = CoreConfig::CoreInfo::name.c_str();
    appInfo.engineVersion = VK_MAKE_VERSION(CoreConfig::CoreInfo::majorVersion,
                                            CoreConfig::CoreInfo::minorVersion,
                                            CoreConfig::CoreInfo::patchVersion);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

    Logger::Log(RENDER, INFO) << "Available Vulkan extentions(" << extensionCount << "): ";
    for (const auto& extension : availableExtensions) {
        Logger::Log(RENDER, INFO) << " | " << extension.extensionName << " v." << extension.specVersion;
    }

    VkInstanceCreateInfo createInfo { };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();
    if ( validationLayersEnabled ) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
        createInfo.ppEnabledLayerNames = requiredLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo { };
    if ( validationLayersEnabled ) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
        createInfo.ppEnabledLayerNames = requiredLayers.data();

        PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    // TODO: Check that all required extensions are supported.
    Logger::Log(RENDER, INFO) << "Creating VkInstance: ";
    Logger::Log(RENDER, INFO) << " | Application name: " << appInfo.pApplicationName;
    Logger::Log(RENDER, INFO) << " | Application version: " << appInfo.applicationVersion;
    Logger::Log(RENDER, INFO) << " | Engine version: " << appInfo.engineVersion;
    Logger::Log(RENDER, INFO) << " | API version: " << appInfo.apiVersion;
    Logger::Log(RENDER, INFO) << " | Enabled extensions (" << createInfo.enabledExtensionCount << "): ";
    for (int i = 0; i < createInfo.enabledExtensionCount; i++) {
        Logger::Log(RENDER, INFO) << " |  * " << *(createInfo.ppEnabledExtensionNames + i);
    }
    Logger::Log(RENDER, INFO) << " | Enabled layers (" << createInfo.enabledLayerCount << "): " ;
    for (int i = 0; i < createInfo.enabledLayerCount; i++) {
        Logger::Log(RENDER, INFO) << " |  * " << *(createInfo.ppEnabledLayerNames + i);
    }

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        Logger::Log(RENDER, ERR_HERE) << "VkInstance creation failed with error code: " << result;
        Logger::Log(RENDER, INFO) << "NOTE: check error code values here: ";
        Logger::Log(RENDER, INFO) << "https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkResult.html";
        throw std::runtime_error("VkInstance creation failed with error code: " + std::to_string(result));
    }
    Logger::Log(RENDER, INFO) << "Created VkInstance";
}
bool VkRenderModule::isReady() {
    return initialized;
}
void VkRenderModule::CheckValidationLayerSupport() {
    if ( CoreConfig::debugMode == true ) {
        validationLayersEnabled = true;
        Logger::Log(RENDER, INFO) << "Vulkan validation layers enabled";

        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        Logger::Log(RENDER, INFO) << "Available Vulkan validation layers (" << layerCount << "): ";
        for (const auto& layerProperties : availableLayers) {
            Logger::Log(RENDER, INFO) << " | " << layerProperties.layerName << " v." << layerProperties.implementationVersion;
        }

        for (const char* layerName : requiredLayers) {
            bool layerSupported = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerSupported = true;
                    break;
                }
            }

            if ( !layerSupported ) {
                Logger::Log(RENDER, ERR_HERE) << "Requested Vulkan validation layer \"" << string(layerName)
                                                                                        << "\" is not supported";
                throw std::runtime_error("Requested Vulkan validation layer \"" + string(layerName)
                                                                                + "\" is not supported");
            }
        }
    }
}
void VkRenderModule::AddDebugExtensions() {
    if ( validationLayersEnabled ) {
        requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
}
VKAPI_ATTR VkBool32 VKAPI_CALL VkRenderModule::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                             VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                             const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                             void* pUserData)
{
    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            Logger::Log(RENDER, DEBUG) << "Validation layer: " << pCallbackData->pMessage; break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            Logger::Log(RENDER, INFO) << "Validation layer: " << pCallbackData->pMessage; break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            Logger::Log(RENDER, WARN) << "Validation layer: " << pCallbackData->pMessage;
            LogDebugObjectsInfo(pCallbackData);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            Logger::Log(RENDER, ERR) << "Validation layer: " << pCallbackData->pMessage;
            LogDebugObjectsInfo(pCallbackData);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
            Logger::Log(RENDER, ERR) << "Validation layer: " << pCallbackData->pMessage;
            LogDebugObjectsInfo(pCallbackData);
            break;
    }

    return VK_FALSE;
}
void VkRenderModule::LogDebugObjectsInfo(const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) {
    Logger::Log(RENDER, INFO) << "Related objects: ";
    for (int i = 0; i < pCallbackData->objectCount; i++) {
        string objName = pCallbackData->pObjects[i].pObjectName ? string(pCallbackData->pObjects[i].pObjectName) : "Unnamed";
        Logger::Log(RENDER, INFO) << " * " << objName << " (" <<pCallbackData->pObjects[i].objectHandle << ')';
    }
}
void VkRenderModule::SetupDebugMessenger() {
    if ( !validationLayersEnabled ) { return; }

    VkDebugUtilsMessengerCreateInfoEXT createInfo { };
    PopulateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("Failed to set up debug messenger!");
    }

}
void VkRenderModule::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                               | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                           | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                           | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = DebugCallback;
}
VkResult VkRenderModule::CreateDebugUtilsMessengerEXT(VkInstance instance,
                                                      const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                      const VkAllocationCallbacks *pAllocator,
                                                      VkDebugUtilsMessengerEXT *pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
void VkRenderModule::DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                                   VkDebugUtilsMessengerEXT debugMessenger,
                                                   const VkAllocationCallbacks *pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}
void VkRenderModule::AddValidationLayers() {
    requiredLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
}
bool VkRenderModule::IsDeviceSuitable(VkPhysicalDevice pDevice) {
    QueueFamilyIndices indices = FindQueueFamilies(pDevice);

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(pDevice, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(pDevice, &deviceFeatures);

    bool extensionsSupported = CheckDeviceExtensionSupport(pDevice);

    const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    return indices.IsComplete() && extensionsSupported;
}
QueueFamilyIndices VkRenderModule::FindQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i;
        }
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        if ( indices.IsComplete() ) {
            break;
        }

        i++;
    }


    return indices;
}
void VkRenderModule::CreateLogicalDevice() {
    QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
    float queuePriority = 1.0f;

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo { };
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures { };

    VkDeviceCreateInfo createInfo { };
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;

    if (validationLayersEnabled) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
        createInfo.ppEnabledLayerNames = requiredLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        Logger::Log(RENDER, ERR_HERE) << "Failed to create logical device";
        throw std::runtime_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}
void VkRenderModule::CreateSurface(Window& window) {
    windowRenderer->AddRenderer(window);
}
bool VkRenderModule::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

}