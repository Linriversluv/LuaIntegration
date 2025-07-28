#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>

enum class LightType : int {
    Directional = 0,
    Point = 1,
    Spot = 2
};

struct Light {
    int id;
    LightType type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    float range;
    float innerCone;
    float outerCone;
    bool enabled;
    
    Light() : id(-1), type(LightType::Point), position(0.0f), direction(0.0f, -1.0f, 0.0f),
              color(1.0f), intensity(1.0f), range(10.0f), innerCone(30.0f), 
              outerCone(45.0f), enabled(true) {}
};

class LightingSystem {
public:
    LightingSystem();
    ~LightingSystem();
    
    // Light management
    int CreateLight(LightType type = LightType::Point);
    bool RemoveLight(int lightId);
    Light* GetLight(int lightId);
    
    // Light properties
    void SetLightPosition(int lightId, const glm::vec3& position);
    void SetLightDirection(int lightId, const glm::vec3& direction);
    void SetLightColor(int lightId, const glm::vec3& color);
    void SetLightIntensity(int lightId, float intensity);
    void SetLightRange(int lightId, float range);
    void SetLightCone(int lightId, float innerCone, float outerCone);
    void SetLightEnabled(int lightId, bool enabled);
    
    // Lighting calculations
    void Update(float deltaTime);
    std::vector<Light> GetActiveLights() const;
    
    // Global lighting settings
    void SetAmbientLight(const glm::vec3& color) { m_ambientLight = color; }
    glm::vec3 GetAmbientLight() const { return m_ambientLight; }
    
    void SetSunDirection(const glm::vec3& direction) { m_sunDirection = glm::normalize(direction); }
    glm::vec3 GetSunDirection() const { return m_sunDirection; }
    
    void SetSunColor(const glm::vec3& color) { m_sunColor = color; }
    glm::vec3 GetSunColor() const { return m_sunColor; }
    
    void SetSunIntensity(float intensity) { m_sunIntensity = intensity; }
    float GetSunIntensity() const { return m_sunIntensity; }
    
private:
    std::unordered_map<int, std::unique_ptr<Light>> m_lights;
    int m_nextLightId = 1;
    
    // Global lighting
    glm::vec3 m_ambientLight = glm::vec3(0.1f, 0.1f, 0.15f);
    glm::vec3 m_sunDirection = glm::vec3(0.3f, -0.7f, 0.5f);
    glm::vec3 m_sunColor = glm::vec3(1.0f, 0.95f, 0.8f);
    float m_sunIntensity = 3.0f;
};