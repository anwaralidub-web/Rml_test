cmake_minimum_required(VERSION 3.31)
project(Rml_Test)

set(CMAKE_CXX_STANDARD 20)
add_definitions(-DGLFW_STATIC)

# ---------------- Include directories ----------------
include_directories(
        include
        libs/glfw/include
        libs/glm
        libs/vulkan/include
        libs/stb
        libs/RmlUI/Include
)

# Vulkan library path
link_directories(libs/vulkan/lib/)

# ---------------- Add GLFW ----------------
add_subdirectory(libs/glfw)

# ---------------- Set Freetype root BEFORE adding RmlUI ----------------
set(Freetype_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype" CACHE PATH "Freetype root folder")

# ---------------- Add RmlUI ----------------
add_subdirectory(libs/RmlUI)

# ---------------- Collect all source files ----------------
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.hpp)

# ---------------- Add executable ----------------
add_executable(Rml_Test
        ${SOURCE_FILES}
        ${HEADER_FILES}
        include/TextureManager.hpp
        src/TextureManager.cpp
        include/RmlVk.hpp
        src/RmlVk.cpp
)

# ---------------- Link libraries ----------------
# Link GLFW, Vulkan, and RmlUI CMake targets (no need for .lib/.a)
target_link_libraries(Rml_Test
        PRIVATE glfw
        PRIVATE vulkan-1
        PRIVATE RmlUi::RmlUi   # <- Use this instead of -lRmlCore/-lRmlDebugger
)
# ---------------- Copy resources post-build ----------------
add_custom_command(TARGET Rml_Test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE_DIR:Rml_Test>/resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:Rml_Test>/resources
)
