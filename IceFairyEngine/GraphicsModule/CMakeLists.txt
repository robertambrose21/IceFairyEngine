cmake_minimum_required(VERSION 3.10)

add_library(gmodule
    src/vulkan/vulkanmodule.cpp
    src/vulkan/shadermodule.cpp
    src/vulkan/vulkanexception.cpp)

set_target_properties(gmodule PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 17)