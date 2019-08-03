cmake_minimum_required(VERSION 3.10)

add_library(IceFairyEngine
    src/core/module.cpp
    src/core/utilities/icexception.cpp)

set_target_properties(IceFairyEngine PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 17)