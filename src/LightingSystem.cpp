#include "LightingSystem.h"
#include <algorithm>

LightingSystem::LightingSystem() = default;
LightingSystem::~LightingSystem() = default;

int LightingSystem::CreateLight(LightType type) {
    int id = m_nextLightId++;
    auto light = std::make_unique<Light>();
    light->id = id;
    light->type = type;
    
    m_lights[id] = std::move(light);
    return id;
}

bool LightingSystem::RemoveLight(int lightId) {
    auto it = m_lights.find(lightId);
    if (it != m_lights.end()) {
        m_lights.erase(it);
        return true;
    }
    return false;
}

Light* LightingSystem::GetLight(int lightId) {
    auto it = m_lights.find(lightId);
    return (it != m_lights.end()) ? it->second.get() : nullptr;
}

void LightingSystem::SetLightPosition(int lightId, const glm::vec3& position) {
    if (auto light = GetLight(lightId)) {
        light->position = position;
    }
}

void LightingSystem::SetLightDirection(int lightId, const glm::vec3& direction) {
    if (auto light = GetLight(lightId)) {
        light->direction = glm::normalize(direction);
    }
}

void LightingSystem::SetLightColor(int lightId, const glm::vec3& color) {
    if (auto light = GetLight(lightId)) {
        light->color = color;
    }
}

void LightingSystem::SetLightIntensity(int lightId, float intensity) {
    if (auto light = GetLight(lightId)) {
        light->intensity = std::max(0.0f, intensity);
    }
}

void LightingSystem::SetLightRange(int lightId, float range) {
    if (auto light = GetLight(lightId)) {
        light->range = std::max(0.1f, range);
    }
}

void LightingSystem::SetLightCone(int lightId, float innerCone, float outerCone) {
    if (auto light = GetLight(lightId)) {
        light->innerCone = glm::clamp(innerCone, 0.0f, 89.0f);
        light->outerCone = glm::clamp(outerCone, light->innerCone, 90.0f);
    }
}

void LightingSystem::SetLightEnabled(int lightId, bool enabled) {
    if (auto light = GetLight(lightId)) {
        light->enabled = enabled;
    }
}

void LightingSystem::Update(float deltaTime) {
    // Update dynamic lighting effects here
    // This could include light animations, flickering, etc.
}

std::vector<Light> LightingSystem::GetActiveLights() const {
    std::vector<Light> activeLights;
    
    for (const auto& [id, light] : m_lights) {
        if (light->enabled) {
            activeLights.push_back(*light);
        }
    }
    
    return activeLights;
}