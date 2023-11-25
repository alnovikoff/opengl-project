cmake_minimum_required(VERSION 3.5)
project(project VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

# add external libraries
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/assimp)

set(SOURCES
    src/other/glad.c
    src/other/stb.cpp

    src/project.cpp
    src/window.cpp
    src/render_util/shader.cpp
    src/render_util/texture.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_EXPORT)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        glfw
        glm
        assimp
    PRIVATE

)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        glfw
        glm
        assimp
    PRIVATE

)
