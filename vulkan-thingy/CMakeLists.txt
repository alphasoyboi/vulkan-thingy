cmake_minimum_required(VERSION 3.20)

project(vulkan-thingy LANGUAGES CXX)

add_executable(${PROJECT_NAME} src/main.cc src/vulkan_application.cc src/vulkan_application.h)
target_include_directories(${PROJECT_NAME} PRIVATE src)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

#--------------------
# LINK DEPENDENCIES
#--------------------

# spdlog
add_subdirectory(src/vendor/spdlog)
target_link_libraries(${PROJECT_NAME} spdlog)

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "don't build the GLFW documentation" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "don't generate installation target" FORCE)
add_subdirectory(src/vendor/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

# vulkan
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})