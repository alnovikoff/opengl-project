cmake_minimum_required(VERSION 3.5)
project(project VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
#find_package(SndFile REQUIRED)

# add external libraries
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/assimp)
add_subdirectory(thirdparty/openal-soft)
add_subdirectory(thirdparty/libsndfile)


set(SOURCES
    src/other/glad.c
    src/other/stb.cpp

    src/project.cpp
    src/window.cpp

    src/render_util/shader.cpp
    src/render_util/texture.cpp
    src/render_util/camera.cpp

    src/render/mesh.cpp
    src/render/model.cpp
    src/render/skybox.cpp

    src/io/keyboard.cpp
    src/io/mouse.cpp

    src/sound/sound_device.cpp
    src/sound/sound_buffer.cpp
    src/sound/sound_source.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_EXPORT)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        glfw
        glm
        assimp
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/openal-soft/include>
    PRIVATE
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        glfw
        glm
        assimp
        OpenAL
        SndFile::sndfile
    PRIVATE
)
