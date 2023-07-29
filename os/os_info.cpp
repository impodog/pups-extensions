//
// Created by impodog on 7/29/2023.
//
#include "init.h"

#ifdef _WIN32
#define OS_NAME "win32"
#elif __APPLE__
#define OS_NAME "darwin"
#elif __linux__
#define OS_NAME "linux"
#elif __unix__
#define OS_NAME "unix"
#else
#define OS_NAME "Unknown"
#endif

namespace pups_extension::os {
    pups::Id id_platform{"", "platform"};

    void init_info(pups::PackageHandler &handler) {
        handler.add(id_platform, std::make_shared<pups::String>(OS_NAME));
    }
}
