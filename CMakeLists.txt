cmake_minimum_required(VERSION 3.10)

project(icEngine)

include_directories(IceFairyEngine/GraphicsModule/src IceFairyEngine/IceFairyEngine/src)

add_subdirectory(IceFairyEngine/GraphicsModule)
add_subdirectory(IceFairyEngine/IceFairyEngine)

set(VulkanGame_SRC_DIR IceFairyEngine/VulkanGame/src)

find_package(glfw3 3.3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(X11 REQUIRED)

link_libraries(${X11_LIBRARIES})
add_definitions(-DVK_USE_PLATFORM_XLIB_KHR)

add_executable(${PROJECT_NAME} ${VulkanGame_SRC_DIR}/main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 17)

target_link_libraries(${PROJECT_NAME} gmodule IceFairyEngine glfw Vulkan::Vulkan)