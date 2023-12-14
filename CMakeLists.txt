cmake_minimum_required(VERSION 3.5)
project(application VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_subdirectory(project)

set(SOURCES
	main.cpp
	src/thread_manager.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} project)
target_include_directories(${PROJECT_NAME} PRIVATE project)
