//
// Created by impodog on 7/28/2023.
//

#ifndef SYS_INIT_H
#define SYS_INIT_H

#include "pups.h"

namespace pups_extension::sys {
    void init_map(pups::PackageHandler &handler);

    void init_ptr(pups::PackageHandler &handler);

    void init_ver(pups::PackageHandler &handler);
}

#endif //SYS_INIT_H
