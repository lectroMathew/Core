#ifndef CORE_VKRENDERMODULE_H
#define CORE_VKRENDERMODULE_H

#include <core/Essential.h>
#include <core/Window.h>

#include <core/Modules/IRenderModule.h>
#include <core/Modules/VkMesh.h>

#include <core/Components/VkRenderer.h>

#include <vulkan/vulkan.h>
#include <optional>

namespace core {

#ifdef CORE_SIMPLIFY_SYNTAX
    typedef class VkRenderModule RenderModule;
#endif

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool IsComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VkRenderModule : public IRenderModule {
    friend class VkRenderer;
    friend class VkWindowRenderer;
    friend class VkBufferFactory;
    friend class VkMesh;
public:
    VkRenderModule(InputModule* inputModule = nullptr);
    ~VkRenderModule();

    void Frame() override;

    void Stop() override;

    bool isReady();

private:
    void Init(Window& window);
    void Cleanup();
    void CleanupSwapChain();

    void CreateInstance();
    void CreateSurface(Window& window);
    void CreateSwapChain(Window& window);
    void RecreateSwapChain(Window& window);
    void CreateLogicalDevice();
    void CreateImageViews();
    void CreateRenderPass();
    void CreateFrameBuffers();
    void CreateDescriptorSetLayout();
    void CreateGraphicsPipeline();
    void CreateCommandPool();
    void CreateCommandBuffers();
    void CreateSemaphores();
    void CreateFences();
    void CreateImagesSyncObject();

    void CheckValidationLayerSupport();
    bool CheckDeviceExtensionSupport(VkPhysicalDevice pDevice);
    void PickPhysicalDevice();
    void AddDebugExtensions();
    void AddValidationLayers();
    void SetupDebugMessenger();

    void OnWindowResize();

    bool IsDeviceSuitable(VkPhysicalDevice pDevice);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice pDevice);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window& window);
    VkShaderModule CreateShaderModule(const std::vector<char>& code);

    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    static VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                             VkDebugUtilsMessageTypeFlagsEXT messageType,
                                             const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                             void* pUserData);
    static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                                 const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                 const VkAllocationCallbacks* pAllocator,
                                                 VkDebugUtilsMessengerEXT* pDebugMessenger);
    static void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                              VkDebugUtilsMessengerEXT debugMessenger,
                                              const VkAllocationCallbacks* pAllocator);
    static void LogDebugObjectsInfo(const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);

    static uint32_t          imageIndex;
    static VkInstance        instance;
    static VkPhysicalDevice  physicalDevice;
    static VkDevice          device;
    static VkCommandPool     commandPool;
    static VkQueue           graphicsQueue;
    static VkQueue           presentQueue;

    VkSurfaceKHR             surface;
    VkSwapchainKHR           swapChain;
    VkFormat                 swapChainImageFormat;
    VkExtent2D               swapChainExtent;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkRenderPass             renderPass;
    VkDescriptorSetLayout    descriptorSetLayout;
    VkPipelineLayout         pipelineLayout;
    VkPipeline               graphicsPipeline;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence>     inFlightFences;
    std::vector<VkFence>     imagesInFlight;

    VkSemaphore              imageAvailableSemaphore;
    VkSemaphore              renderFinishedSemaphore;

    bool initialized             { false };
    bool validationLayersEnabled { false };

    std::vector<const char*>     requiredExtensions;
    std::vector<const char*>     deviceExtensions;
    std::vector<const char*>     requiredLayers;
    static std::vector<VkImage>  swapChainImages;
    std::vector<VkImageView>     swapChainImageViews;
    std::vector<VkFramebuffer>   swapChainFramebuffers;
    std::vector<VkCommandBuffer> commandBuffers;
    static std::vector<VkMesh*>  meshes;

    const uint32_t maxFramesInFlight { 2 };
    size_t         currentFrame      { 0 };

    Window* currentWindow { nullptr };

};

}

#endif //CORE_VKRENDERMODULE_H
