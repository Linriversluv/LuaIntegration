cmake_minimum_required(VERSION 3.20)
project(VulkanLuaEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Lua 5.4
find_package(PkgConfig REQUIRED)
pkg_check_modules(LUA REQUIRED lua5.4)

# Sol2 for Lua binding
include(FetchContent)
FetchContent_Declare(
    sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2.git
    GIT_TAG v3.3.0
)
FetchContent_MakeAvailable(sol2)

# Create executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Engine.cpp
    src/VulkanRenderer.cpp
    src/LuaManager.cpp
    src/LightingSystem.cpp
    src/Scene.cpp
)

target_link_libraries(${PROJECT_NAME}
    Vulkan::Vulkan
    glfw
    glm::glm
    sol2::sol2
    ${LUA_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${LUA_INCLUDE_DIRS}
    include/
)