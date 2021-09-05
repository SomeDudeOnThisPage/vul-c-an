cmake_minimum_required(VERSION 3.10)
project(game C)

set(CMAKE_C_STANDARD 11)

# base project paths
set(SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR}/src)
set(LIBRARY_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

# path to GLFW include and dependencies directories
set(GLFW_DIRECTORY ${LIBRARY_DIRECTORY}/glfw-3.3.4)
set(GLFW_LIBRARY_INCLUDE ${GLFW_DIRECTORY}/include)
set(GLFW_DEPENDENCIES ${GLFW_DIRECTORY}/deps)

# path to vulkan sdk include folder
set(VULKAN_SDK ${LIBRARY_DIRECTORY}/vk-1.2.182.0)
set(VULKAN_SDK_INCLUDE ${VULKAN_SDK}/Include)
set(VULKAN_SDK_LINK_LIBRARY ${VULKAN_SDK}/Lib/vulkan-1.lib)

# project source files
set(SOURCE_FILES
        ${SOURCE_DIRECTORY}/main.c
        ${SOURCE_DIRECTORY}/platform/window.c
        ${SOURCE_DIRECTORY}/platform/vulkan.c
        src/game.c)

# project source header files
set(HEADER_FILES
        ${SOURCE_DIRECTORY}/platform/window.h
        ${SOURCE_DIRECTORY}/game.h
        ${SOURCE_DIRECTORY}/platform/vulkan.h
        src/debug.h)

# include stuff needed for our program, namely GLFW, its' dependencies and the vulkan SDK headers
add_subdirectory(${GLFW_DIRECTORY})
include_directories(
        ${GLFW_LIBRARY_INCLUDE}
        ${GLFW_DEPENDENCIES}
        ${VULKAN_SDK_INCLUDE}
)

# make the thing happen
add_executable(game ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(game glfw ${GLFW_LIBRARIES} ${VULKAN_SDK_LINK_LIBRARY})
target_compile_definitions(game PUBLIC "$<$<CONFIG:VK_DEBUG>:VK_DEBUG>")
target_compile_definitions(game PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
