#include "Engine.h"
#include "VulkanRenderer.h"
#include "LuaManager.h"
#include "LightingSystem.h"
#include "Scene.h"
#include <GLFW/glfw3.h>
#include <iostream>

Engine::Engine() = default;
Engine::~Engine() = default;

bool Engine::Initialize() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Vulkan Lua Engine", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    // Initialize subsystems
    m_renderer = std::make_unique<VulkanRenderer>();
    if (!m_renderer->Initialize(window)) {
        return false;
    }
    
    m_lightingSystem = std::make_unique<LightingSystem>();
    m_scene = std::make_unique<Scene>();
    
    m_luaManager = std::make_unique<LuaManager>();
    if (!m_luaManager->Initialize(this)) {
        return false;
    }
    
    // Load initial Lua scripts
    m_luaManager->LoadScript("scripts/lighting_demo.lua");
    
    m_isRunning = true;
    m_lastFrameTime = std::chrono::high_resolution_clock::now();
    
    return true;
}

void Engine::Run() {
    GLFWwindow* window = glfwGetCurrentContext();
    
    while (m_isRunning && !glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - m_lastFrameTime).count();
        m_lastFrameTime = currentTime;
        
        Update(deltaTime);
        Render();
    }
}

void Engine::Update(float deltaTime) {
    // Update lighting system
    m_lightingSystem->Update(deltaTime);
    
    // Call Lua update callbacks
    m_luaManager->CallUpdate(deltaTime);
    
    // Update scene
    // m_scene->Update(deltaTime);
}

void Engine::Render() {
    m_renderer->BeginFrame();
    
    // Update uniforms
    UniformBufferObject ubo{};
    // Fill UBO with current camera and world data...
    m_renderer->UpdateUniforms(ubo);
    
    // Update lighting data
    auto lights = m_lightingSystem->GetActiveLights();
    std::vector<LightData> lightData;
    for (const auto& light : lights) {
        LightData data{};
        data.position = light.position;
        data.color = light.color;
        data.intensity = light.intensity;
        data.type = static_cast<int>(light.type);
        lightData.push_back(data);
    }
    m_renderer->UpdateLights(lightData);
    
    m_renderer->EndFrame();
}

void Engine::Shutdown() {
    m_luaManager.reset();
    m_renderer->Cleanup();
    m_renderer.reset();
    
    glfwTerminate();
}