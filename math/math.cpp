//
// Created by impodog on 7/31/2023.
//
#include "init.h"

namespace pups_extension::math {

}

extern "C" void PUPS_Init_math(pups::PackageHandler &handler) {
    pups_extension::math::init_constants(handler);
    pups_extension::math::init_func(handler);
    pups_extension::math::init_conversion(handler);
}
