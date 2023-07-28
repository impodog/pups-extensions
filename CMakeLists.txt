cmake_minimum_required(VERSION 3.26)

set(PUPS_PATH "./PourUPScript")

include_directories(${PUPS_PATH})
link_directories(${PUPS_PATH}/build)

project(test)

add_executable(test ext_test.cpp)

target_link_libraries(test pups_Lib)
