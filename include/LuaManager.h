#pragma once
#include <sol/sol.hpp>
#include <memory>
#include <functional>
#include <glm/glm.hpp>

class Engine;
class LightingSystem;
class Scene;

class LuaManager {
public:
    LuaManager();
    ~LuaManager();
    
    bool Initialize(Engine* engine);
    void Shutdown();
    
    // Script execution
    bool LoadScript(const std::string& filename);
    bool ExecuteString(const std::string& code);
    
    // Engine bindings
    void RegisterEngineAPI();
    void RegisterLightingAPI();
    void RegisterSceneAPI();
    
    // Callbacks
    void SetUpdateCallback(std::function<void(float)> callback);
    void CallUpdate(float deltaTime);
    
    sol::state& GetLuaState() { return m_lua; }
    
private:
    sol::state m_lua;
    Engine* m_engine = nullptr;
    
    std::function<void(float)> m_updateCallback;
    
    // Helper functions for type conversion
    void RegisterMathTypes();
    void RegisterUtilityFunctions();
};