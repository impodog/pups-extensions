cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)

project(sys)

set(PUPS_PATH D:/workspace/Cpp/Tool/pups-extensions/PourUPScript)

include_directories(${PUPS_PATH})
link_directories(${PUPS_PATH}/build)
add_library(sys SHARED init.h sys.cpp sys_map.cpp sys_ptr.cpp sys_ver.cpp)
target_link_libraries(sys pups_Lib)