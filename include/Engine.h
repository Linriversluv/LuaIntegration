#pragma once
#include <memory>
#include <vector>
#include <chrono>

class VulkanRenderer;
class LuaManager;
class LightingSystem;
class Scene;

class Engine {
public:
    Engine();
    ~Engine();
    
    bool Initialize();
    void Run();
    void Shutdown();
    
private:
    void Update(float deltaTime);
    void Render();
    
    std::unique_ptr<VulkanRenderer> m_renderer;
    std::unique_ptr<LuaManager> m_luaManager;
    std::unique_ptr<LightingSystem> m_lightingSystem;
    std::unique_ptr<Scene> m_scene;
    
    bool m_isRunning = false;
    std::chrono::high_resolution_clock::time_point m_lastFrameTime;
};