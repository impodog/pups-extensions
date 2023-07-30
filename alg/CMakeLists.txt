cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)

project(alg)

set(PUPS_PATH D:/workspace/Cpp/Tool/pups-extensions/PourUPScript)

include_directories(${PUPS_PATH})
link_directories(${PUPS_PATH}/build)
add_library(alg SHARED init.h alg.cpp
        alg_rand.cpp
        alg_iter.cpp
)
target_link_libraries(alg pups_Lib)