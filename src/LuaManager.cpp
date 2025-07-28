#include "LuaManager.h"
#include "Engine.h"
#include "LightingSystem.h"
#include "Scene.h"
#include <iostream>
#include <fstream>

LuaManager::LuaManager() = default;
LuaManager::~LuaManager() = default;

bool LuaManager::Initialize(Engine* engine) {
    m_engine = engine;
    
    try {
        m_lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, 
                            sol::lib::table, sol::lib::io, sol::lib::package);
        
        RegisterMathTypes();
        RegisterEngineAPI();
        RegisterLightingAPI();
        RegisterSceneAPI();
        RegisterUtilityFunctions();
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Lua initialization failed: " << e.what() << std::endl;
        return false;
    }
}

void LuaManager::RegisterMathTypes() {
    // Register glm::vec3
    m_lua.new_usertype<glm::vec3>("vec3",
        sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
        "x", &glm::vec3::x,
        "y", &glm::vec3::y,
        "z", &glm::vec3::z,
        "length", [](const glm::vec3& v) { return glm::length(v); },
        "normalize", [](const glm::vec3& v) { return glm::normalize(v); },
        "dot", [](const glm::vec3& a, const glm::vec3& b) { return glm::dot(a, b); },
        "cross", [](const glm::vec3& a, const glm::vec3& b) { return glm::cross(a, b); },
        sol::meta_function::addition, [](const glm::vec3& a, const glm::vec3& b) { return a + b; },
        sol::meta_function::subtraction, [](const glm::vec3& a, const glm::vec3& b) { return a - b; },
        sol::meta_function::multiplication, sol::overload(
            [](const glm::vec3& v, float s) { return v * s; },
            [](float s, const glm::vec3& v) { return s * v; }
        )
    );
    
    // Register glm::mat4
    m_lua.new_usertype<glm::mat4>("mat4",
        sol::constructors<glm::mat4(), glm::mat4(float)>()
    );
}

void LuaManager::RegisterLightingAPI() {
    // Light types enum
    m_lua["LightType"] = m_lua.create_table_with(
        "Directional", 0,
        "Point", 1,
        "Spot", 2
    );
    
    // Light creation and management
    m_lua["Light"] = m_lua.create_table_with(
        "create", [this](sol::table config) -> int {
            // Extract light parameters from Lua table
            glm::vec3 position = config.get_or("position", glm::vec3(0.0f));
            glm::vec3 color = config.get_or("color", glm::vec3(1.0f));
            float intensity = config.get_or("intensity", 1.0f);
            int type = config.get_or("type", 1); // Default to point light
            
            // Create light through lighting system
            // Return light ID for future reference
            return 0; // Placeholder
        },
        
        "setPosition", [this](int lightId, glm::vec3 position) {
            // Update light position
        },
        
        "setColor", [this](int lightId, glm::vec3 color) {
            // Update light color
        },
        
        "setIntensity", [this](int lightId, float intensity) {
            // Update light intensity
        },
        
        "remove", [this](int lightId) {
            // Remove light
        }
    );
}

void LuaManager::RegisterEngineAPI() {
    m_lua["Engine"] = m_lua.create_table_with(
        "getTime", []() -> float {
            static auto start = std::chrono::high_resolution_clock::now();
            auto now = std::chrono::high_resolution_clock::now();
            return std::chrono::duration<float>(now - start).count();
        },
        
        "log", [](const std::string& message) {
            std::cout << "[Lua] " << message << std::endl;
        },
        
        "quit", [this]() {
            // Signal engine to quit
        }
    );
}

void LuaManager::RegisterSceneAPI() {
    m_lua["Scene"] = m_lua.create_table_with(
        "setCameraPosition", [this](glm::vec3 position) {
            // Update camera position
        },
        
        "setCameraTarget", [this](glm::vec3 target) {
            // Update camera target
        },
        
        "getCameraPosition", [this]() -> glm::vec3 {
            // Return camera position
            return glm::vec3(0.0f);
        }
    );
}

bool LuaManager::LoadScript(const std::string& filename) {
    try {
        m_lua.script_file(filename);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load Lua script '" << filename << "': " << e.what() << std::endl;
        return false;
    }
}

bool LuaManager::ExecuteString(const std::string& code) {
    try {
        m_lua.script(code);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to execute Lua code: " << e.what() << std::endl;
        return false;
    }
}

void LuaManager::SetUpdateCallback(std::function<void(float)> callback) {
    m_updateCallback = callback;
}

void LuaManager::CallUpdate(float deltaTime) {
    if (m_updateCallback) {
        try {
            m_updateCallback(deltaTime);
        }
        catch (const std::exception& e) {
            std::cerr << "Lua update callback error: " << e.what() << std::endl;
        }
    }
}