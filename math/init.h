//
// Created by impodog on 7/31/2023.
//

#ifndef MATH_INIT_H
#define MATH_INIT_H

#include "pups.h"

#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdlib>

namespace pups_extension::math {
    using pups::library::pups_int;
    using pups::library::pups_float;

    void init_constants(pups::PackageHandler &handler);

    void init_func(pups::PackageHandler &handler);

    void init_conversion(pups::PackageHandler &handler);
}

#endif //MATH_INIT_H
