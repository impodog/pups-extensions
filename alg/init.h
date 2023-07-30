//
// Created by impodog on 7/30/2023.
//

#ifndef ALG_INIT_H
#define ALG_INIT_H

#include "pups.h"
#include <random>
#include <algorithm>

namespace pups_extension::alg {
    extern std::random_device rd;
    extern std::mt19937 gen;

    using namespace pups::library::utility;

    void init_rand(pups::PackageHandler &handler);

    void init_iter(pups::PackageHandler &handler);
}

#endif //ALG_INIT_H
