-- Lighting Demo Script
print("Loading lighting demo...")

-- Global variables
local time = 0
local lights = {}
local cameraAngle = 0

-- Initialize the scene
function init()
    Engine.log("Initializing lighting demo")
    
    -- Create a rotating point light
    lights.rotating = Light.create({
        type = LightType.Point,
        position = vec3(0, 2, 0),
        color = vec3(1, 0.5, 0.2),
        intensity = 5.0
    })
    
    -- Create a pulsing light
    lights.pulsing = Light.create({
        type = LightType.Point,
        position = vec3(3, 1, 3),
        color = vec3(0.2, 0.5, 1),
        intensity = 2.0
    })
    
    -- Create a spotlight
    lights.spot = Light.create({
        type = LightType.Spot,
        position = vec3(-2, 4, -2),
        color = vec3(1, 1, 0.8),
        intensity = 8.0
    })
end

-- Update function called every frame
function update(deltaTime)
    time = time + deltaTime
    
    -- Rotate the orange light around the center
    local radius = 3.0
    local rotatingPos = vec3(
        math.cos(time) * radius,
        2.0 + math.sin(time * 2) * 0.5,
        math.sin(time) * radius
    )
    Light.setPosition(lights.rotating, rotatingPos)
    
    -- Pulse the blue light
    local pulseIntensity = 2.0 + math.sin(time * 4) * 1.5
    Light.setIntensity(lights.pulsing, math.max(0.1, pulseIntensity))
    
    -- Move camera in a circle
    cameraAngle = cameraAngle + deltaTime * 0.3
    local cameraPos = vec3(
        math.cos(cameraAngle) * 8,
        3,
        math.sin(cameraAngle) * 8
    )
    Scene.setCameraPosition(cameraPos)
    Scene.setCameraTarget(vec3(0, 0, 0))
    
    -- Color cycling for the spotlight
    local r = (math.sin(time * 0.5) + 1) * 0.5
    local g = (math.sin(time * 0.7) + 1) * 0.5
    local b = (math.sin(time * 0.3) + 1) * 0.5
    Light.setColor(lights.spot, vec3(r, g, b))
end

-- Initialize when script loads
init()