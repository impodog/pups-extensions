cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)

project(alg)

set(PUPS_PATH D:/workspace/Cpp/Tool/pups-extensions/PourUPScript)

include_directories(${PUPS_PATH})
link_directories(${PUPS_PATH}/build)
add_library(math SHARED init.h math.cpp
        math_constants.cpp
        math_func.cpp
        math_conversion.cpp
)
target_link_libraries(math pups_Lib)