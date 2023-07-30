//
// Created by impodog on 7/30/2023.
//

#include "init.h"


extern "C" void PUPS_Init_alg(pups::PackageHandler &handler) {
    pups_extension::alg::init_rand(handler);
    pups_extension::alg::init_iter(handler);
}