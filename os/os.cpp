//
// Created by impodog on 7/29/2023.
//

#include "init.h"

namespace pups_extension::os {
    OsError::OsError(const std::string &s) : pups::Error(s) {

    }

    std::string OsError::error_name() const noexcept {
        return "OsError";
    }
}

extern "C" void PUPS_Init_os(pups::PackageHandler &handler) {
    pups_extension::os::init_info(handler);
    pups_extension::os::init_path(handler);
    pups_extension::os::init_entry(handler);
    pups_extension::os::init_iter(handler);
    pups_extension::os::init_cmd(handler);
}