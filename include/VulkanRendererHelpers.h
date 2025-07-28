#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <functional>
#include <glm/glm.hpp>

// Forward declarations
class VulkanRenderer;
struct QueueFamilyIndices;
struct SwapChainSupportDetails;
struct Vertex;

// Validation layer and extension constants
extern const std::vector<const char*> validationLayers;
extern const std::vector<const char*> deviceExtensions;

#ifdef NDEBUG
    constexpr bool enableValidationLayers = false;
#else
    constexpr bool enableValidationLayers = true;
#endif

// Maximum number of frames in flight for double/triple buffering
constexpr int MAX_FRAMES_IN_FLIGHT = 2;
constexpr int MAX_LIGHTS = 32;

// =============================================================================
// DEBUG AND VALIDATION FUNCTIONS
// =============================================================================

// Debug callback function
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);

// Validation layer support
bool CheckValidationLayerSupport();

// Debug messenger extension functions
VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator);

// =============================================================================
// FILE I/O AND RESOURCE UTILITIES
// =============================================================================

// File I/O helper for reading shader files
std::vector<char> ReadFile(const std::string& filename);

// Shader compilation utilities
std::vector<uint32_t> CompileGLSLToSPIRV(const std::string& source, const std::string& filename, bool isVertexShader);
bool CompileShaderFromFile(const std::string& inputFile, const std::string& outputFile, bool isVertexShader);

// =============================================================================
// ERROR HANDLING UTILITIES
// =============================================================================

// Error handling utilities
void ThrowIfFailed(VkResult result, const std::string& operation);

// Convert VkResult to string for better error messages
std::string VkResultToString(VkResult result);

// Resource cleanup utilities with null safety
template<typename T>
void SafeDestroy(T& handle, std::function<void(T)> destroyFunc) {
    if (handle != VK_NULL_HANDLE) {
        destroyFunc(handle);
        handle = VK_NULL_HANDLE;
    }
}

// =============================================================================
// VULKAN FEATURE AND EXTENSION CHECKING
// =============================================================================

// Instance and device extension support
bool CheckInstanceExtensionSupport(const std::vector<const char*>& requiredExtensions);
bool CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& requiredExtensions);

// Device feature checking
bool CheckDeviceFeatureSupport(VkPhysicalDevice device);
bool IsDeviceDiscrete(VkPhysicalDevice device);
uint32_t RateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface);

// =============================================================================
// QUEUE FAMILY UTILITIES
// =============================================================================

// Queue family operations
QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
bool IsQueueFamilyComplete(const QueueFamilyIndices& indices);

// =============================================================================
// SWAP CHAIN UTILITIES
// =============================================================================

// Swap chain support and configuration
SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

// =============================================================================
// MEMORY MANAGEMENT UTILITIES
// =============================================================================

// Memory utilities
uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

// Buffer creation and management
void CreateBuffer(
    VkDevice device,
    VkPhysicalDevice physicalDevice,
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkBuffer& buffer,
    VkDeviceMemory& bufferMemory);

void CopyBuffer(
    VkDevice device,
    VkCommandPool commandPool,
    VkQueue queue,
    VkBuffer srcBuffer,
    VkBuffer dstBuffer,
    VkDeviceSize size);

// =============================================================================
// IMAGE AND TEXTURE UTILITIES
// =============================================================================

// Format utilities
VkFormat FindSupportedFormat(
    VkPhysicalDevice physicalDevice,
    const std::vector<VkFormat>& candidates,
    VkImageTiling tiling,
    VkFormatFeatureFlags features);

VkFormat FindDepthFormat(VkPhysicalDevice physicalDevice);
bool HasStencilComponent(VkFormat format);

// Image creation and management
void CreateImage(
    VkDevice device,
    VkPhysicalDevice physicalDevice,
    uint32_t width,
    uint32_t height,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkImage& image,
    VkDeviceMemory& imageMemory);

VkImageView CreateImageView(
    VkDevice device,
    VkImage image,
    VkFormat format,
    VkImageAspectFlags aspectFlags);

void TransitionImageLayout(
    VkDevice device,
    VkCommandPool commandPool,
    VkQueue queue,
    VkImage image,
    VkFormat format,
    VkImageLayout oldLayout,
    VkImageLayout newLayout);

// =============================================================================
// COMMAND BUFFER UTILITIES
// =============================================================================

// Command buffer management
VkCommandBuffer BeginSingleTimeCommands(VkDevice device, VkCommandPool commandPool);
void EndSingleTimeCommands(
    VkDevice device,
    VkCommandPool commandPool,
    VkQueue queue,
    VkCommandBuffer commandBuffer);

// =============================================================================
// SHADER UTILITIES
// =============================================================================

// Shader module creation
VkShaderModule CreateShaderModule(VkDevice device, const std::vector<char>& code);
VkShaderModule CreateShaderModuleFromFile(VkDevice device, const std::string& filename);

// =============================================================================
// VERTEX DATA UTILITIES
// =============================================================================

// Vertex attribute helpers
VkVertexInputBindingDescription GetVertexBindingDescription();
std::vector<VkVertexInputAttributeDescription> GetVertexAttributeDescriptions();

// =============================================================================
// LOGGING AND DEBUG UTILITIES
// =============================================================================

// Logging utilities
void LogVulkanInfo(VkInstance instance, VkPhysicalDevice physicalDevice);
void LogAvailableExtensions();
void LogAvailableLayers();
void LogDeviceProperties(VkPhysicalDevice device);
void LogMemoryProperties(VkPhysicalDevice device);
void LogQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

// Performance monitoring
struct FrameTimeStats {
    float averageFrameTime = 0.0f;
    float minFrameTime = std::numeric_limits<float>::max();
    float maxFrameTime = 0.0f;
    uint32_t frameCount = 0;
};

void UpdateFrameStats(FrameTimeStats& stats, float frameTime);
void LogFrameStats(const FrameTimeStats& stats);

// =============================================================================
// DESCRIPTOR SET UTILITIES
// =============================================================================

// Descriptor set helpers
VkDescriptorSetLayout CreateDescriptorSetLayout(
    VkDevice device,
    const std::vector<VkDescriptorSetLayoutBinding>& bindings);

VkDescriptorPool CreateDescriptorPool(
    VkDevice device,
    const std::vector<VkDescriptorPoolSize>& poolSizes,
    uint32_t maxSets);

void AllocateDescriptorSets(
    VkDevice device,
    VkDescriptorPool descriptorPool,
    const std::vector<VkDescriptorSetLayout>& layouts,
    std::vector<VkDescriptorSet>& descriptorSets);

void UpdateDescriptorSets(
    VkDevice device,
    const std::vector<VkWriteDescriptorSet>& descriptorWrites);

// =============================================================================
// PIPELINE UTILITIES
// =============================================================================

// Pipeline creation helpers
VkPipelineLayout CreatePipelineLayout(
    VkDevice device,
    const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,
    const std::vector<VkPushConstantRange>& pushConstantRanges = {});

VkRenderPass CreateRenderPass(
    VkDevice device,
    VkFormat colorFormat,
    VkFormat depthFormat);

// =============================================================================
// SYNCHRONIZATION UTILITIES
// =============================================================================

// Synchronization object creation
VkSemaphore CreateSemaphore(VkDevice device);
VkFence CreateFence(VkDevice device, VkFenceCreateFlags flags = 0);

// =============================================================================
// UTILITY STRUCTS AND ENUMS
// =============================================================================

// Utility structures for common operations
struct BufferCreateInfo {
    VkDeviceSize size;
    VkBufferUsageFlags usage;
    VkMemoryPropertyFlags properties;
};

struct ImageCreateInfo {
    uint32_t width;
    uint32_t height;
    VkFormat format;
    VkImageTiling tiling;
    VkImageUsageFlags usage;
    VkMemoryPropertyFlags properties;
};

// Render target information
struct RenderTargetInfo {
    VkFormat colorFormat;
    VkFormat depthFormat;
    uint32_t width;
    uint32_t height;
    uint32_t samples = 1;
};

// =============================================================================
// VULKAN CONSTANTS AND LIMITS
// =============================================================================

namespace VulkanLimits {
    constexpr uint32_t MAX_DESCRIPTOR_SETS = 1000;
    constexpr uint32_t MAX_UNIFORM_BUFFERS = 100;
    constexpr uint32_t MAX_STORAGE_BUFFERS = 100;
    constexpr uint32_t MAX_SAMPLERS = 100;
    constexpr uint32_t MAX_COMBINED_IMAGE_SAMPLERS = 100;
    constexpr uint32_t MAX_VERTEX_ATTRIBUTES = 16;
    constexpr uint32_t MAX_VERTEX_BINDINGS = 16;
    constexpr VkDeviceSize MIN_UNIFORM_BUFFER_OFFSET_ALIGNMENT = 256;
    constexpr VkDeviceSize MIN_STORAGE_BUFFER_OFFSET_ALIGNMENT = 256;
}

// =============================================================================
// MATH AND UTILITY HELPERS
// =============================================================================

// Math utilities for Vulkan coordinate systems
glm::mat4 CreateProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);
glm::mat4 CreateViewMatrix(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
glm::mat4 CreateModelMatrix(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);

// Utility functions for common calculations
float CalculateAspectRatio(uint32_t width, uint32_t height);
VkExtent2D ClampExtent(const VkExtent2D& extent, const VkExtent2D& minExtent, const VkExtent2D& maxExtent);

// =============================================================================
// PLATFORM-SPECIFIC UTILITIES
// =============================================================================

// Platform-specific surface creation helpers
#ifdef VK_USE_PLATFORM_WIN32_KHR
VkSurfaceKHR CreateWin32Surface(VkInstance instance, HWND hwnd);
#endif

#ifdef VK_USE_PLATFORM_XLIB_KHR
VkSurfaceKHR CreateXlibSurface(VkInstance instance, Display* display, Window window);
#endif

// GLFW integration helpers
std::vector<const char*> GetRequiredGLFWExtensions();
VkSurfaceKHR CreateGLFWSurface(VkInstance instance, GLFWwindow* window);

// =============================================================================
// VALIDATION AND DEBUG MACROS
// =============================================================================

#ifdef _DEBUG
    #define VK_CHECK(call) \
        do { \
            VkResult result = call; \
            if (result != VK_SUCCESS) { \
                std::cerr << "Vulkan error in " << __FILE__ << ":" << __LINE__ \
                          << " - " << VkResultToString(result) << std::endl; \
                throw std::runtime_error("Vulkan call failed"); \
            } \
        } while(0)
    
    #define VK_LOG(message) \
        std::cout << "[Vulkan]: " << message << std::endl
#else
    #define VK_CHECK(call) call
    #define VK_LOG(message)
#endif

// =============================================================================
// RESOURCE LIFETIME MANAGEMENT
// =============================================================================

// RAII wrapper for Vulkan resources
template<typename VkHandle, typename Deleter>
class VulkanResource {
public:
    VulkanResource() = default;
    VulkanResource(VkHandle handle, Deleter deleter) : m_handle(handle), m_deleter(deleter) {}
    
    ~VulkanResource() {
        if (m_handle != VK_NULL_HANDLE) {
            m_deleter(m_handle);
        }
    }
    
    // Move semantics
    VulkanResource(VulkanResource&& other) noexcept 
        : m_handle(other.m_handle), m_deleter(std::move(other.m_deleter)) {
        other.m_handle = VK_NULL_HANDLE;
    }
    
    VulkanResource& operator=(VulkanResource&& other) noexcept {
        if (this != &other) {
            if (m_handle != VK_NULL_HANDLE) {
                m_deleter(m_handle);
            }
            m_handle = other.m_handle;
            m_deleter = std::move(other.m_deleter);
            other.m_handle = VK_NULL_HANDLE;
        }
        return *this;
    }
    
    // Disable copy semantics
    VulkanResource(const VulkanResource&) = delete;
    VulkanResource& operator=(const VulkanResource&) = delete;
    
    VkHandle get() const { return m_handle; }
    VkHandle* getPtr() { return &m_handle; }
    
    void reset() {
        if (m_handle != VK_NULL_HANDLE) {
            m_deleter(m_handle);
            m_handle = VK_NULL_HANDLE;
        }
    }
    
private:
    VkHandle m_handle = VK_NULL_HANDLE;
    Deleter m_deleter;
};

// Type aliases for common Vulkan resources
using VulkanBuffer = VulkanResource<VkBuffer, std::function<void(VkBuffer)>>;
using VulkanImage = VulkanResource<VkImage, std::function<void(VkImage)>>;
using VulkanImageView = VulkanResource<VkImageView, std::function<void(VkImageView)>>;
using VulkanSemaphore = VulkanResource<VkSemaphore, std::function<void(VkSemaphore)>>;
using VulkanFence = VulkanResource<VkFence, std::function<void(VkFence)>>;

// =============================================================================
// HELPER FACTORY FUNCTIONS
// =============================================================================

// Factory functions for creating RAII-wrapped Vulkan objects
VulkanBuffer CreateManagedBuffer(VkDevice device, const BufferCreateInfo& createInfo);
VulkanImage CreateManagedImage(VkDevice device, VkPhysicalDevice physicalDevice, const ImageCreateInfo& createInfo);
VulkanImageView CreateManagedImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
VulkanSemaphore CreateManagedSemaphore(VkDevice device);
VulkanFence CreateManagedFence(VkDevice device, VkFenceCreateFlags flags = 0);