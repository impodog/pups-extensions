cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)

project(os)

set(PUPS_PATH D:/workspace/Cpp/Tool/pups-extensions/PourUPScript)

include_directories(${PUPS_PATH})
link_directories(${PUPS_PATH}/build)
add_library(os SHARED init.h os.cpp os_info.cpp os_path.cpp
        os_entry.cpp
        os_iter.cpp
        os_cmd.cpp)
target_link_libraries(os pups_Lib)
