//
// Created by impodog on 7/28/2023.
//

#include "init.h"

extern "C" void PUPS_Init_sys(pups::PackageHandler &handler) {
    pups_extension::sys::init_map(handler);
    pups_extension::sys::init_ptr(handler);
    pups_extension::sys::init_ver(handler);
}