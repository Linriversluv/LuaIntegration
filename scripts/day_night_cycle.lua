-- Day/Night Cycle Script
print("Loading day/night cycle...")

local timeOfDay = 12.0 -- Start at noon (24-hour format)
local daySpeed = 1.0 -- How fast time passes (1.0 = real time)

local sunLight = nil
local moonLight = nil

function init()
    Engine.log("Setting up day/night cycle")
    
    -- Create sun (directional light)
    sunLight = Light.create({
        type = LightType.Directional,
        color = vec3(1, 0.95, 0.8),
        intensity = 3.0
    })
    
    -- Create moon (directional light)
    moonLight = Light.create({
        type = LightType.Directional,
        color = vec3(0.3, 0.3, 0.6),
        intensity = 0.5
    })
end

function updateSunPosition(hour)
    -- Convert hour to angle (0-24 hours -> 0-360 degrees)
    local sunAngle = (hour / 24.0) * 2 * math.pi - math.pi/2
    
    -- Calculate sun direction
    local sunHeight = math.sin(sunAngle)
    local sunForward = math.cos(sunAngle)
    
    local sunDirection = vec3(sunForward, -sunHeight, 0.2)
    Light.setPosition(sunLight, sunDirection) -- For directional lights, position stores direction
    
    -- Moon is opposite to sun
    local moonDirection = vec3(-sunForward, sunHeight, -0.2)
    Light.setPosition(moonLight, moonDirection)
    
    -- Adjust sun intensity based on height
    local sunIntensity = math.max(0, sunHeight * 3.0)
    Light.setIntensity(sunLight, sunIntensity)
    
    -- Moon is visible when sun is down
    local moonIntensity = math.max(0, -sunHeight * 0.8)
    Light.setIntensity(moonLight, moonIntensity)
    
    -- Adjust sun color based on height (warmer when low)
    local warmth = math.max(0, 1.0 - sunHeight)
    local sunColor = vec3(
        1.0,
        0.95 - warmth * 0.3,
        0.8 - warmth * 0.5
    )
    Light.setColor(sunLight, sunColor)
end

function update(deltaTime)
    -- Advance time
    timeOfDay = timeOfDay + deltaTime * daySpeed / 3600.0 -- Convert seconds to hours
    if timeOfDay >= 24.0 then
        timeOfDay = timeOfDay - 24.0
    end
    
    updateSunPosition(timeOfDay)
    
    -- Log time occasionally
    if math.floor(Engine.getTime()) % 5 == 0 then
        local hours = math.floor(timeOfDay)
        local minutes = math.floor((timeOfDay - hours) * 60)
        Engine.log(string.format("Time: %02d:%02d", hours, minutes))
    end
end

init()